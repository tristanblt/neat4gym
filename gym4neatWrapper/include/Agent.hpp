#ifndef AGENT_HPP_
#define AGENT_HPP_

#include "GymRequests.hpp"
#include <string>
#include <vector>

class Agent {
    public:
        Agent(const std::string &env);
        ~Agent();

        void run();

    protected:
    private:
        GymRequests _gr;
        std::string _instance_id;
};

#endif /* !AGENT_HPP_ */
