#include "Agent.hpp"

int main(int argc, char const *argv[])
{
    Agent cartPole("CartPole-v0", "http://127.0.0.1:12345/v1/");
    cartPole.run(150);

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