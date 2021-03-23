#ifndef AGENT_HPP_
#define AGENT_HPP_

#include "GymRequests.hpp"
#include <string>
#include <vector>
#include "NEAT.hpp"

class Agent {
public:
    Agent(const std::string &env, const std::string &endpoint, int outputs);
    ~Agent();

    void run(int population, int runs = 1);

protected:
private:
    int _outputs;
    GymRequests _gr;
    std::string _instanceId;
};

#endif /* !AGENT_HPP_ */
