
#ifndef _GYMSHELL_HPP_
#define _GYMSHELL_HPP_

#include "Agent.hpp"

class GymShell {
public:
    GymShell(int challenge, int population);
    ~GymShell() = default;

    GymShell(const GymShell &rhs) = delete;
    GymShell &operator=(const GymShell &rhs) = delete;

    void run();

    void exit(const std::vector<std::string> &args);
    void train(const std::vector<std::string> &args);
    void best(const std::vector<std::string> &args);
    void save(const std::vector<std::string> &args);
    void load(const std::vector<std::string> &args);
    void help(const std::vector<std::string> &args);

private:
    neat::Settings settings;
    std::unique_ptr<Agent> _agent;
    bool _running = true;
    int _generation = 0;
};

#endif // _GYMSHELL_HPP_
