#include "Agent.hpp"
#include <signal.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/ndarrayobject.h>

static bool receivedSigint = false;

static void sigint_handler(int)
{
    receivedSigint = true;
}

static std::vector<float> listTupleToVector_Float(PyObject* incoming) {
    std::vector<float> data;
    for(Py_ssize_t i = 0; i < PyList_Size(incoming); i++) {
        PyObject *value = PyList_GetItem(incoming, i);
        data.push_back( PyFloat_AsDouble(value) );
    }
    return data;
}

static PyObject* vectorToList_Float(const std::vector<float> &data) {
    PyObject* listObj = PyList_New( data.size() );
    if (!listObj) throw std::logic_error("Unable to allocate memory for Python list");
    for (unsigned int i = 0; i < data.size(); i++) {
        PyObject *num = PyFloat_FromDouble( (double) data[i]);
        if (!num) {
            Py_DECREF(listObj);
            throw std::logic_error("Unable to allocate memory for Python list");
        }
        PyList_SET_ITEM(listObj, i, num);
    }
    return listObj;
}

Agent::Agent(const std::string &env, int inputs, int outputs, bool discrete, int population, const neat::Settings &settings):
    _outputs(outputs),
    _discrete(discrete),
    _settings(settings)
{
    signal(SIGINT, sigint_handler);

    neat = std::make_unique<neat::NEAT>(
        population,
        inputs,
        _outputs,
        _settings
        );

    Py_Initialize();
    []()->void *{import_array(); return 0;}();
    PyObject* myModuleString = PyUnicode_FromString("gym");
    PyObject* numpyModuleString = PyUnicode_FromString("numpy");
    _module = PyImport_Import(myModuleString);
    _numpy = PyImport_Import(numpyModuleString);
    _gym_make = PyObject_GetAttrString(_module, (char*)"make");
    _numpy_array = PyObject_GetAttrString(_numpy, (char*)"array");
    PyObject* args = PyTuple_Pack(1, PyUnicode_FromString(env.c_str()));
    _env = PyObject_CallObject(_gym_make, args);
    if (!_env) {
        PyErr_Print();
        exit(1);
    }

}

Agent::~Agent()
{
    Py_Finalize();
}

std::vector<float> Agent::reset()
{
    PyObject *value = PyObject_CallMethod(_env, "reset", "()");
    value = PyArray_ToList((PyArrayObject *)value);
    return listTupleToVector_Float(value);
}

void Agent::step(const std::vector<float> &action, std::vector<float> &observation, bool &isover, float &fitness)
{
    observation.clear();
    PyObject *value;

    if (_discrete) {
        int a = 0;
        float max = 0;
        for (size_t i = 0; i < action.size(); i++) {
            if (action[i] > max) {
                max = action[i];
                a = i;
            }
        }
        value = PyObject_CallMethod(_env, "step", "i", a);
    } else {
        std::vector<float> values;
        for (size_t i = 0; i + 1 < action.size(); i+=2) {
            // if (action[i] > action[i + 1]) {
            //     values.push_back(action[i]);
            // } else {
            //     values.push_back(-action[i + 1]);
            // }
            values.push_back(action[i] - action[i + 1]);
        }
        PyObject *list = vectorToList_Float(values);
        PyObject *args = PyTuple_Pack(1, list);
        PyObject *retValue = PyObject_CallObject(_numpy_array, args);
        value = PyObject_CallMethodOneArg(_env, PyUnicode_FromString("step"), retValue);
    }
    if (value) {
        PyObject *list = PyArray_ToList((PyArrayObject *)PyTuple_GetItem(value, 0));
        observation = listTupleToVector_Float(list);
        fitness = PyFloat_AsDouble(PyTuple_GetItem(value, 1));
        isover = PyObject_IsTrue(PyTuple_GetItem(value, 2));
    } else {
        PyErr_Print();
        exit(1);
    }
}

void Agent::render()
{
    PyObject_CallMethod(_env, "render", "()");
}

const Agent::RunData &Agent::runOne(int runs, bool rend)
{
    std::vector<float> inputs;
    float fitness = 0;

    receivedSigint = false;
    _data = RunData();
    for (size_t i = 0; i < neat->size(); i++) {
        if (receivedSigint)
            return _data;

        for (int run = 0; run < runs; run++) {
            inputs = reset();
            while (!receivedSigint) {
                const auto &outputs = neat->compute(i, inputs);
                float stepFitness = 0;
                bool isover = false;
                step(outputs, inputs, isover, stepFitness);
                fitness += stepFitness;
                if (rend)
                    render();
                if (isover)
                    break;
            }
        }
        fitness = fitness / static_cast<float>(runs);
        _data.fitnesses.push_back(fitness);
        neat->setFitness(i, fitness);
        if (fitness > _bestFitness)
            _best = neat->network(i).copy();
        fitness = 0;
    }

    _data.best = _data.fitnesses.front();
    for (auto &a : _data.fitnesses) {
        _data.average += a;
        if (a > _data.best)
            _data.best = a;
    }

    _data.average /= _data.fitnesses.size();
    neat->nextGeneration();
    _data.complete = true;
    return _data;
}

const Agent::RunData &Agent::runBest()
{
    receivedSigint = false;
    _data = RunData();
    if (!_best)
        return _data;
    reset();
    bool isover = false;
    float fitness = 0;
    std::vector<float> action;
    std::vector<float> observation;
    observation = reset();
    while (!isover) {
        if (receivedSigint)
            return _data;
        action = _best->compute(observation, _settings);
        step(action, observation, isover, fitness);
        render();
    }
    _data.complete = true;
    return _data;
}
