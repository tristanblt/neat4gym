#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <string>
#include <vector>
#include "NEAT.hpp"
#include <Python.h>

class Agent {
public:

    struct RunData {
        std::vector<float> fitnesses;
        float average = 0;
        float best = 0;
        bool complete = false;
    };

    Agent(const std::string &env, int inputs, int outputs, int population, const neat::Settings &settings);
    ~Agent();
    // void run(int population, int runs = 1);

    const RunData &runOne(int runs, bool render);

    std::unique_ptr<neat::NEAT> neat;

    std::vector<float> reset();
    void step(const std::vector<float> &action, std::vector<float> &observations, bool &isover, float &fitness);
    void render();

private:
    int _outputs;
    RunData _data;
    const neat::Settings &_settings;
    PyObject *_module;
    PyObject *_gym_make;
    PyObject *_env;
};

#endif /* !AGENT_HPP_ */
