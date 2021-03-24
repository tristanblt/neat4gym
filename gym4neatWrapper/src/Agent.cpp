#include "Agent.hpp"
#include <signal.h>

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

    _neat = std::make_unique<neat::NEAT>(
        population,
        _gr.observationSpace(_instanceId).n,
        _outputs,
        _settings
        );

}

Agent::~Agent() = default;

const Agent::RunData &Agent::runOne(int runs, bool render)
{
    std::vector<float> inputs;
    GymRequests::StepData step;
    float fitness = 0;

    receivedSigint = false;
    _data = RunData();
    for (size_t i = 0; i < _neat->size(); i++) {
        if (receivedSigint)
            return _data;

        for (int i = 0; i < runs; i++) {
            inputs = _gr.reset(_instanceId);
            while (!receivedSigint) {
                const auto &outputs = _neat->compute(i, inputs);
                step = _gr.step(_instanceId, outputs, render);
                inputs = step.inputs;
                fitness += step.score;
                if (step.isOver)
                    break;
            }
        }
        fitness = fitness / static_cast<float>(runs);
        _data.fitnesses.push_back(fitness);
        _neat->setFitness(i, fitness);
        fitness = 0;
    }

    _data.best = _data.fitnesses.front();
    for (auto &a : _data.fitnesses) {
        _data.average += a;
        if (a > _data.best)
            _data.best = a;
    }
    _data.average /= _data.fitnesses.size();
    _neat->nextGeneration();
    _data.complete = true;
    return _data;
}

// void Agent::run(int population, int runs)
// {
//     std::vector<float> inputs;
//     GymRequests::StepData step;
//     float fitness = 0;
//     std::vector<float> fitnesses;

//     _gr.startMonitor(_instanceId);

//     neat::NEAT neat(
//         population,
//         _gr.observationSpace(_instanceId).n,
//         _outputs
//         );

//     int generation = 0;
//     while(!receivedSigint) {
//         for (int episode = 0; episode < population; episode++) {
//             for (int i = 0; i < runs; i++) {
//                 inputs = _gr.reset(_instanceId);
//                 while (!receivedSigint) {
//                     const auto &outputs = neat.compute(episode, inputs);
//                     step = _gr.step(_instanceId, outputs);
//                     inputs = step.inputs;

//                     fitness += step.score;

//                     if (step.isOver)
//                         break;
//                 }
//             }
//             fitness = fitness / static_cast<float>(runs);
//             fitnesses.push_back(fitness);
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
