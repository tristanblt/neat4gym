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

void Agent::run(int population, int runs)
{
    signal(SIGINT, sigint_handler);
    std::vector<float> inputs;
    // neat::NEAT::Data data;
    GymRequests::StepData step;
    float fitness = 0;
    std::vector<float> fitnesses;

    _gr.startMonitor(_instanceId);

    neat::NEAT neat(
        population,
        _gr.observationSpace(_instanceId).n,
        _outputs
        );

    neat.load("ez.json");

    int generation = 0;
    while(!receivedSigint) {
        for (int episode = 0; episode < population; episode++) {
            for (int i = 0; i < runs; i++) {
                inputs = _gr.reset(_instanceId);
                while (!receivedSigint) {
                    const auto &outputs = neat.compute(episode, inputs);
                    step = _gr.step(_instanceId, outputs);
                    inputs = step.inputs;

                    fitness += step.score;

                    if (step.isOver)
                        break;
                }
            }
            fitness = fitness / static_cast<float>(runs);
            fitnesses.push_back(fitness);
            neat.setFitness(episode, fitness);
            fitness = 0;
        }
        neat.nextGeneration();
        generation++;

        float average = 0;
        float best = fitnesses.front();
        for (auto &a : fitnesses) {
            average += a;
            if (a > best)
                best = a;
        }
        average /= fitnesses.size();
        fitnesses.clear();
        std::cout << "Generation: " << generation << " -> average: " << average << ", best: " << best << std::endl;
    }
    neat.save("ez.json");
    _gr.closeMonitor(_instanceId);
}
