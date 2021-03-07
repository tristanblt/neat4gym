#include "Network.hpp"
#include "Link.hpp"

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

    return 0;
}

bool Network::mutateWeight(int from, int to, float delta, bool set)
{
    Neuron *n1 = getNeuron(from);

    if (!n1)
        return false;
    auto *link = n1->getLinkTo(to);
    if (!link)
        return false;
    if (set) {
        link->weight = delta;
    }
    return true;
}

bool Network::hasLink(int from, int to) const
{
    Neuron *n1 = getNeuron(from), *n2 = nullptr;

    if (!n1)
        return false;
    n2 = n1->getNeuronTo(to);
    if (!n2)
        return false;
    return true;
}

bool Network::addLink(int from, int to, int innovationId, float weight)
{
    Neuron *n1 = getNeuron(from), *n2 = getNeuron(to);

    if (!n1 || !n2 || n1->layer < n2->layer || (n1->layer == 0 && n2->layer == 0))
        return false;
    Neuron::link(n1, n2);
    _innovations.emplace_back(innovationId, from, to, weight);
    return true;
}

int Network::createNode()
{
    int neuronId = getNextNeuronId();
    _hiddens.emplace_back(std::make_unique<Neuron>(neuronId));
    return neuronId;
}

bool Network::canAddLink(int from, int to) const
{
    int layer1 = -1, layer2 = -1;

    for (auto &ptr: _inputs) {
        if (ptr->id == from) {
            if (layer2 != -1)
                return false; // cannot link input links together
            layer1 = ptr->layer;
        } else if (ptr->id == to) {
            if (layer1 != -1)
                return false; // cannot link input links together
            layer2 = ptr->layer;
        }
    }
    for (auto &ptr: _hiddens) {
        if (ptr->id == from) {
            if (layer2 != -1)
                return layer1 <= layer2;
            layer1 = ptr->layer;
        } else if (ptr->id == to) {
            if (layer1 != -1)
                return layer1 <= layer2;
            layer2 = ptr->layer;
        }
    }
    for (auto &ptr: _outputs) {
        if (ptr->id == from) {
            if (layer2 != -1)
                return layer1 <= layer2;
            layer1 = ptr->layer;
        } else if (ptr->id == to) {
            if (layer1 != -1)
                return layer1 <= layer2;
            layer2 = ptr->layer;
        }
    }
    return false;
}

void Network::rebuildNetwork()
{
}

int Network::getNextNeuronId()
{
    return _nextNeuronId++;
}

void Network::computeLayers() const
{
    for (auto &ptr: _inputs) {
        ptr->computeLayersRec();
    }
}

Neuron *Network::getNeuron(int id) const
{
    for (auto &ptr: _inputs) {
        if (ptr->id == id)
            return ptr.get();
    }
    for (auto &ptr: _hiddens) {
        if (ptr->id == id)
            return ptr.get();
    }
    for (auto &ptr: _outputs) {
        if (ptr->id == id)
            return ptr.get();
    }
    return nullptr;
}
