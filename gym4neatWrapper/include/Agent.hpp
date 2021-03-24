#ifndef AGENT_HPP_
#define AGENT_HPP_

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

    Agent(const std::string &env, int outputs, int population, const neat::Settings &settings);
    ~Agent();
    // void run(int population, int runs = 1);

    const RunData &runOne(int runs, bool render);

    std::unique_ptr<neat::NEAT> neat;

private:
    int _outputs;
    RunData _data;
    const neat::Settings &_settings;
};

#endif /* !AGENT_HPP_ */
