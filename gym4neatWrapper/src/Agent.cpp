#include "Agent.hpp"
#include <signal.h>
#include <Python.h>

static bool receivedSigint = false;

static void sigint_handler(int)
{
    receivedSigint = true;
}

Agent::Agent(const std::string &env, const std::string &endpoint, int outputs, int population, const neat::Settings &settings):
    _outputs(outputs),
    _settings(settings)
{
    _gr.setEndpoint(endpoint);
    _instanceId = _gr.createInstance(env);
    signal(SIGINT, sigint_handler);

    _gr.startMonitor(_instanceId);

    neat = std::make_unique<neat::NEAT>(
        population,
        _gr.observationSpace(_instanceId).n,
        _outputs,
        _settings
        );
    // TEMP
    PyObject *gymModule, *pModule, *pDict, *pFunc;
    std::cout << "DEBUT PYTHON" << std::endl;

    Py_Initialize();
    // gymModule = PyString_FromString("gym");
    // pModule = PyImport_Import(gymModule);
    PyRun_SimpleString("import gym");
    std::cout << "u" << std::endl;

    pDict = PyModule_GetDict(pModule);
    pFunc = PyDict_GetItemString(pDict, "caca");

    if (PyCallable_Check(pFunc)) {
        std::cout << "CHECK" << std::endl;
        PyObject_CallObject(pFunc, NULL);
    } else {
        PyErr_Print();
    }

    Py_DECREF(pModule);
    Py_DECREF(gymModule);

    Py_Finalize();
    std::cout << "FIN PYTHON" << std::endl;
    // END TEMP
}

Agent::~Agent() = default;

const Agent::RunData &Agent::runOne(int runs, bool render)
{
    std::vector<float> inputs;
    GymRequests::StepData step;
    float fitness = 0;

    receivedSigint = false;
    _data = RunData();
    for (size_t i = 0; i < neat->size(); i++) {
        if (receivedSigint)
            return _data;

        for (int i = 0; i < runs; i++) {
            inputs = _gr.reset(_instanceId);
            while (!receivedSigint) {
                const auto &outputs = neat->compute(i, inputs);
                step = _gr.step(_instanceId, outputs, render);
                inputs = step.inputs;
                fitness += step.score;
                if (step.isOver)
                    break;
            }
        }
        fitness = fitness / static_cast<float>(runs);
        _data.fitnesses.push_back(fitness);
        neat->setFitness(i, fitness);
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

struct Neat4Gym
{
    
};

// void Agent::run(int population, int runs)
// {
//     std::vector<float> inputs;
//     GymRequests::StepData step;
//     float fitness = 0;
//     std::vector<float> fitnesses;

//     _gr.startMonitor(_instanceId);

//                         break;
//                 }
//             }
//             fitness = fitness / static_cast<float>(runs);
//             neat.setFitness(episode, fitness);
//             fitness = 0;
//         }
//         neat.nextGeneration();
//         generation++;

//         float average = 0;
//         float best = fitnesses.front();
//         for (auto &a : fitnesses) {
//             average += a;
//             if (a > best)
//                 best = a;
//         }
//         average /= fitnesses.size();
//         fitnesses.clear();
//         std::cout << "Generation: " << generation << " -> average: " << average << ", best: " << best << std::endl;
//     }
//     _gr.closeMonitor(_instanceId);
// }
