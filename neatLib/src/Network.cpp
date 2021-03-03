#include "Network.hpp"

using namespace neat;

Network::Network(int inputs, int outputs)
{
    _inputs.reserve(inputs);
    _outputs.reserve(outputs);

    for (int i = 0; i < inputs; i++)
        _inputs.push_back(std::make_unique<Neuron>(getNextNeuronId()));
    for (int i = 0; i < outputs; i++)
        _outputs.push_back(std::make_unique<Neuron>(getNextNeuronId()));
}

std::vector<float> Network::compute(const std::vector<float> &inputs) const
{

}

Network Network::crossover(const Network &a, const Network &b)
{

}

float Network::computeSimilarity(const Network &a, const Network &b)
{
    size_t N = a._innovations.size() > b._innovations.size() ?
        a._innovations.size() : b._innovations.size();


}

void Network::mutate()
{

}

void Network::rebuildNetwork()
{

}

int Network::getNextNeuronId()
{
    return _nextNeuronId++;
}
