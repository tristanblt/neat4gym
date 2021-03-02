#include "NEAT.hpp"

using namespace neat;

NEAT::NEAT(int pop, int outputs, int inputs):
    _pop(pop, outputs, inputs)
{
}

NEAT::~NEAT() = default;
