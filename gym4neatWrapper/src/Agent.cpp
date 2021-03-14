#include "Agent.hpp"

Agent::Agent(const std::string &env)
{
    _instanceId = _gr.createInstance(env);
}

Agent::~Agent()
{
}

void Agent::run()
{
    std::vector<float> inputs;
    neat::NEAT::Data data;
    GymRequests::StepData step;

    _gr.startMonitor(_instanceId);

    neat::NEAT neat(
        1000,
        _gr.observationSpace(_instanceId).n,
        _gr.actionSpace(_instanceId).n);

    for (int episode = 0; episode < 150; episode++) {
        std::cout << "Episode: " << episode << std::endl;
        inputs = _gr.reset(_instanceId);
        while (true) {
            data = neat.step(step.isOver, step.score, inputs);
            step = _gr.step(_instanceId, data.outputs);
            inputs = step.inputs;

            if (step.isOver)
                break;
        }
        std::cout << "Generation: " << data.currentGeneration << std::endl;
    }
    _gr.closeMonitor(_instanceId);
}