#include "Agent.hpp"
#include <signal.h>

Agent::Agent(const std::string &env, const std::string &endpoint, int outputs):
    _outputs(outputs)
{
    _gr.setEndpoint(endpoint);
    _instanceId = _gr.createInstance(env);
}

Agent::~Agent()
{
}

static bool receivedSigint = false;

static void sigint_handler(int)
{
    receivedSigint = true;
}

void Agent::run(int population)
{
    signal(SIGINT, sigint_handler);
    std::vector<float> inputs;
    neat::NEAT::Data data;
    GymRequests::StepData step;
    float fitness = 0;
    std::vector<float> fitnesses;

    _gr.startMonitor(_instanceId);

    neat::NEAT neat(
        population,
        _gr.observationSpace(_instanceId).n,
        _outputs
        );

    while(!receivedSigint) {
        for (int episode = 0; episode < population; episode++) {
            inputs = _gr.reset(_instanceId);
            while (!receivedSigint) {
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
        float best = fitnesses.front();
        for (auto &a : fitnesses) {
            average += a;
            if (a > best)
                best = a;
        }
        average /= fitnesses.size();
        fitnesses.clear();
        std::cout << "Generation: " << data.currentGeneration << " -> average: " << average << ", best: " << best << std::endl;
    }
    _gr.closeMonitor(_instanceId);
}
