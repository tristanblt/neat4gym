#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <string>
#include <vector>
#include "NEAT.hpp"
#include <Python.h>
#include "Network.hpp"

class Agent {
public:

    struct RunData {
        std::vector<float> fitnesses;
        float average = 0;
        float best = 0;
        bool complete = false;
    };

    Agent(const std::string &env, int inputs, int outputs, bool discrete, int population, const neat::Settings &settings);
    ~Agent();

    const RunData &runOne(int runs, bool render);

    std::unique_ptr<neat::NEAT> neat;

    std::vector<float> reset();
    void step(const std::vector<float> &action, std::vector<float> &observations, bool &isover, float &fitness);
    void render();

    const Agent::RunData &runBest();

private:
    int _outputs;
    RunData _data;
    bool _discrete;
    const neat::Settings &_settings;
    PyObject *_module;
    PyObject *_numpy;
    PyObject *_gym_make;
    PyObject *_numpy_array;
    PyObject *_env;

    std::unique_ptr<neat::Network> _best = nullptr;
    float _bestFitness = std::numeric_limits<float>().lowest();
};

#endif /* !AGENT_HPP_ */
