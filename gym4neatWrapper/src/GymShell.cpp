#include "GymShell.hpp"
#include <sstream>

static std::vector<std::string> split(const std::string &str, const std::string &delim)
{
    std::vector<std::string> split;
    size_t i = str.find_first_not_of(delim);
    size_t j = str.find_first_of(delim, i);
    while (i != std::string::npos) {
        split.push_back(str.substr(i, j - i));
        i = str.find_first_not_of(delim, j);
        j = str.find_first_of(delim, i);
    }
    return split;
}

GymShell::GymShell(int challenge, int population)
{
    std::string env;
    int outputs = 0;
    switch (challenge) {
    case 1: {
        env = "CartPole-v0";
        outputs = 2;
        settings.minMutations = 3;
        break;
    }
    case 2: {
        env = "LunarLander-v2";
        outputs = 4;
        break;
    }
    case 3: {
        env = "MountainCar-v0";
        outputs = 3;
        break;
    }
    case 4: {
        env = "Acrobot-v1";
        outputs = 3;
        break;
    }
    case 5: {
        env = "Pendulum-v0";
        outputs = 2;
        break;
    }
    default:
        throw std::exception();
    }
    _agent = std::make_unique<Agent>(env, "http://127.0.0.1:12345/v1/", outputs, population, settings);
}

void GymShell::exit(const std::vector<std::string> &)
{
    _running = false;
}

void GymShell::save(const std::vector<std::string> &args)
{
    if (args.size() <= 1) {
        std::cout << "USAGE: save [filepath]" << std::endl;
        return;
    }
}

void GymShell::load(const std::vector<std::string> &args)
{
    if (args.size() <= 1) {
        std::cout << "USAGE: load [filepath]" << std::endl;
        return;
    }
}

void GymShell::train(const std::vector<std::string> &args)
{
    if (args.size() < 3) {
        std::cout << "USAGE: train [render: 0 or 1] [runs: int > 0]" << std::endl;
        return;
    }
    int render = 0;
    int runs = 0;
    try {
        render = std::stoi(args[1]);
        runs = std::stoi(args[2]);
    } catch(...) {
        std::cout << "ERROR: arguments must be ints" << std::endl;
        return;
    }

    if (render != 0 && render != 1) {
        std::cout << "ERROR: render must be 0 or 1" << std::endl;
        return;
    } else if (runs <= 0) {
        std::cout << "ERROR: runs must be positive" << std::endl;
        return;
    }
    while (true) {
        auto &data = _agent->runOne(runs, render);
        if (!data.complete)
            break;
        std::cout << "Generation " << _generation++ << ": average " << data.average << ", best " << data.best << std::endl;
    }
}

void GymShell::help(const std::vector<std::string> &)
{
    std::cout << "Commands: exit, load, save, train." << std::endl;
}

static const std::map<std::string, void (GymShell::*)(const std::vector<std::string> &)> commands =
{
    {"exit", &GymShell::exit},
    {"load", &GymShell::load},
    {"save", &GymShell::save},
    {"train", &GymShell::train},
    {"help", &GymShell::help},
};

void GymShell::run()
{
    std::string line;
    _running = true;
    std::cout << "> ";
    while (_running && getline(std::cin, line)) {
        auto args = split(line, " ");
        if (args.size() == 0) {
            std::cout << "> ";
            continue;
        }
        if (!commands.count(args[0])) {
            std::cout << "INVALID COMMAND" << std::endl << "> ";
            continue;
        }
        (this->*commands.at(args[0]))(args);
        std::cout << "> ";
    }
}
