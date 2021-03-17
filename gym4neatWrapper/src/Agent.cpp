#include "Agent.hpp"

Agent::Agent(const std::string &env, const std::string &endpoint)
{
    _gr.setEndpoint(endpoint);
    _instanceId = _gr.createInstance(env);
}

Agent::~Agent()
{
}

void Agent::run(int population)
{
    std::vector<float> inputs;
    neat::NEAT::Data data;
    GymRequests::StepData step;
    float fitness = 0;
    std::vector<float> fitnesses;

    _gr.startMonitor(_instanceId);

    neat::NEAT neat(
        population,
        _gr.observationSpace(_instanceId).n,
        _gr.actionSpace(_instanceId).n);

    while(true) {
        for (int episode = 0; episode < population; episode++) {
            inputs = _gr.reset(_instanceId);
            while (true) {
                data = neat.step(step.isOver, fitness, inputs);
                step = _gr.step(_instanceId, data.outputs);
                inputs = step.inputs;

                fitness += step.score;

                if (step.isOver) {
                    // std::cout << data.currentGeneration << "-" << (episode + 1) << " -> fitness: " << fitness << std::endl;
                    fitnesses.push_back(fitness);
                    fitness = 0;
                    break;
                }
            }
        }
        float average = 0;
        for (auto &a : fitnesses)
            average += a;
        average /= fitnesses.size();
        fitnesses.clear();
        std::cout << "Generation: " << data.currentGeneration << " -> average: " << average << std::endl;
    }
    _gr.closeMonitor(_instanceId);
}