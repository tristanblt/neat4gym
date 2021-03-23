#include "NEAT.hpp"

using namespace neat;

NEAT::NEAT(int pop, int inputs, int outputs):
    _pop(pop, outputs, inputs, settings)
{
    srand(0);
}

NEAT::~NEAT() = default;

const std::vector<float> &NEAT::compute(size_t network, const std::vector<float> &inputs)
{
    return _pop.compute(network, inputs, settings);
}

void NEAT::setFitness(size_t network, float fitness)
{
    _pop.setFitness(network, fitness);
}

void NEAT::nextGeneration()
{
    _data.currentGeneration++;
    _data.currentIndividual = 0;
    _pop.computeSpecies(settings);
    _pop.purge(settings);
    _pop.genOffsprings(settings);
}

void NEAT::save(const std::string &filepath)
{}

void NEAT::load(const std::string &filepath)
{}

void NEAT::compute(const std::vector<float> &inputs, std::vector<float> &outputs) const
{
}

// extern "C" {
//     NEAT* NEAT_new(int pop, int inputs, int outputs)
//     {
//         return new NEAT(pop, inputs, outputs);
//     }

//     PyObject *NEAT_compute(NEAT *neat, size_t network, PyObject *inputs)
//     {
//         std::vector<float> data;

//         std::cout << PyList_Size(inputs) << std::endl;
//         if (PyList_Check(inputs)) {
//             std::cout << "t" << std::endl;
//             for(Py_ssize_t i = 0; i < PyList_Size(inputs); i++) {
//                 std::cout << "i" << std::endl;
//                 PyObject *value = PyList_GetItem(inputs, i);
//                 // PyObject *value = PyFloat_FromDouble(1.0);
//                 std::cout << value << std::endl;
//                 std::cout << PyFloat_AsDouble(value) << std::endl;
//                 // data.push_back(PyFloat_AsDouble(value));
//                 std::cout << "er" << std::endl;
//                 // std::cout << data.back() << std::endl;
//             }
//         }

//         std::cout << data.size() << std::endl;
//         // std::vector<float> outputs;// = neat->compute(network, inputs);
//         // return outputs;
//         PyObject *result = PyList_New(0);
//         PyList_Append(result, Py_BuildValue("f", 1.0));
//         // for (auto &elem : outputs)
//         //     PyList_Append(result, PyLong_FromLong(elem));
//         std::cout << "a" << std::endl;
//         return result;
//     }

//     void NEAT_setFitness(NEAT *neat, size_t network, float fitness)
//     {
//         neat->setFitness(network, fitness);
//     }

//     void NEAT_nextGeneration(NEAT *neat)
//     {
//         neat->nextGeneration();
//     }
// }