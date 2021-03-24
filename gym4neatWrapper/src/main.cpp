#include "Agent.hpp"
#include "GymShell.hpp"

#include <Python.h>

int main(int, char const *[])
{
    GymShell shell(2, 150);
    shell.run();
    return 0;
}
