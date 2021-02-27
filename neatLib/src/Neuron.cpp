
#include "Neuron.hpp"
#include <algorithm>

using namespace neat;

Neuron::Neuron() = default;

Neuron::~Neuron() = default;

void Neuron::link(Neuron &from, Neuron &to)
{
    from._links.emplace_back(from, to);
    to._links.emplace_back(from, to);
}

void Neuron::unlink(Neuron &from, Neuron &to)
{
    std::find_if(from._links.begin(), from._links.end(), [](const Link &link) {

    });
}
