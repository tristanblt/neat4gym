#ifndef AGENT_HPP_
#define AGENT_HPP_

#include "GymRequests.hpp"
#include <string>
#include <vector>
#include "NEAT.hpp"

class Agent {
public:

    struct RunData {
        std::vector<float> fitnesses;
        float average = 0;
        float best = 0;
        bool complete = false;
    };

    Agent(const std::string &env, const std::string &endpoint, int outputs, int population, const neat::Settings &settings);
    ~Agent();
    // void run(int population, int runs = 1);

    const RunData &runOne(int runs, bool render);

private:
    int _outputs;
    RunData _data;
    GymRequests _gr;
    std::string _instanceId;
    std::unique_ptr<neat::NEAT> _neat;
    const neat::Settings &_settings;
};

#endif /* !AGENT_HPP_ */
