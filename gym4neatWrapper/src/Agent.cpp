#include "Agent.hpp"

Agent::Agent(const std::string &env)
{
    _instance_id = _gr.createInstance(env);
}

Agent::~Agent()
{
}

void Agent::run()
{
    _gr.startMonitor(_instance_id);
    // while (true) {
    //     std::vector<float> a;
    //     _gr.step(_instance_id, a);
    // }
    _gr.closeMonitor(_instance_id);
}