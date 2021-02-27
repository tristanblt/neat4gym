#include "Network.hpp"

using namespace neat;

Network::Network(int inputs, int outputs)
{
    _inputs.reserve(inputs);
    _outputs.reserve(outputs);

    for (int i = 0; i < inputs; i++)
        _inputs.emplace_back(getNextNeuronId());
    for (int i = 0; i < outputs; i++)
        _outputs.emplace_back(getNextNeuronId());
}

int Network::getNextNeuronId()
{
    return _nextNeuronId++;
}
