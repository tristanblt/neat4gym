#include "Agent.hpp"
#include "GymShell.hpp"

#include <Python.h>

int main(int, char const *[])
{
    GymShell shell(2, 150);
    shell.run();
    return 0;

    // if (argc != 4) {
    //     std::cout << argv[0] << " <population> <endpoint> <env name>" << std::endl;
    //     return 1;
    // }

    // Agent cartPole(argv[3], argv[2]);
    // int population;

    // population = atoi(argv[1]);
    // if (population) {
    //     cartPole.run(population);
    // }
    // return 0;
}
