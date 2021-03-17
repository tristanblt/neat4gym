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
    float fitness = 0;

    _gr.startMonitor(_instanceId);

    neat::NEAT neat(
        5,
        _gr.observationSpace(_instanceId).n,
        _gr.actionSpace(_instanceId).n);

    while(true) {
        for (int episode = 0; episode < 5; episode++) {
            inputs = _gr.reset(_instanceId);
            while (true) {
                data = neat.step(step.isOver, fitness, inputs);
                step = _gr.step(_instanceId, data.outputs);
                inputs = step.inputs;

                fitness += step.score;

                if (step.isOver) {
                    std::cout << data.currentGeneration << "-" << (episode + 1) << "-> fitness: " << fitness << std::endl;
                    fitness = 0;
                    break;
                }
            }
        }
    }
    _gr.closeMonitor(_instanceId);
}