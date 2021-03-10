#include "Network.hpp"
#include "Link.hpp"
#include <algorithm>

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

std::vector<float> Network::compute(const std::vector<float> &inputs, const Settings &settings) const
{
    std::vector<float> values;
    for (size_t i = 0; i < inputs.size(); ++i )
        _inputs[i]->setValue(inputs[i]);
    values.reserve(_outputs.size());
    for (auto &output: _outputs) {
        values.push_back(output->computeValue(settings));
    }
    return values;
}

Network Network::crossover(const Network &a, const Network &b)
{

}

float Network::computeSimilarity(const Network &a, const Network &b, const Settings &settings)
{
    size_t n = a._innovations.size() > b._innovations.size() ?
        a._innovations.size() : b._innovations.size();
    const auto &listBig = a._innovations.back().innovationId > b._innovations.back().innovationId ?
        a._innovations : b._innovations;
    const auto &listSmall = a._innovations.back().innovationId <= b._innovations.back().innovationId ?
        a._innovations : b._innovations;
    int excess = 0;
    int disjoint = 0;
    int similar = 0;
    float weightDiff = 0;

    int maxInnovatoinIdSmall = listSmall.back().innovationId;

    for (const auto &elem: listBig) {
        if (elem.innovationId > maxInnovatoinIdSmall) {
            excess++;
            continue;
        }
        auto pos = std::find_if(std::begin(listSmall), std::end(listSmall), [&elem](const Genome &genome) {
            return genome.innovationId == elem.innovationId;
        });
        if (pos == std::end(listSmall))
            disjoint++;
    }

    for (const auto &elem: listSmall) {
        auto pos = std::find_if(std::begin(listBig), std::end(listBig), [&elem](const Genome &genome) {
            return genome.innovationId == elem.innovationId;
        });
        if (pos == std::end(listBig)) {
            disjoint++;
            continue;
        }
        similar++;
        weightDiff += pos->linkWeight - elem.linkWeight;
    }

    float averageWeightDiff = weightDiff / (float)similar;

    return (settings.similarityCoefExcess * excess) / n +
        (settings.similarityCoefDisjoint * disjoint) / n +
        averageWeightDiff * settings.similarityCoefWeight;
}

bool Network::mutateWeight(int from, int to, float delta, bool set)
{
    Neuron *n1 = getNeuron(from);

    if (!n1)
        return false;
    auto *link = n1->getLinkTo(to);
    if (!link)
        return false;
    if (set)
        link->weight = delta;
    else
        link->weight += delta;
    for (auto &genome : _innovations)
        if (genome.neuronFromId == from && genome.neuronToId == to) {
            genome.linkWeight = link->weight;
            break;
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
    if (innovationId != -1)
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
    size_t inputsSize = _inputs.size();
    size_t outputsSize = _outputs.size();

    _inputs.clear();
    _hiddens.clear();
    _outputs.clear();
    _nextNeuronId = 1;

    for (size_t i = 0; i < inputsSize; i++)
        _inputs.push_back(std::make_unique<Neuron>(getNextNeuronId()));
    for (size_t i = 0; i < outputsSize; i++)
        _outputs.push_back(std::make_unique<Neuron>(getNextNeuronId()));

    for (auto &genome : _innovations) {
        if (getNeuron(genome.neuronFromId) == nullptr)
            createNode();
        if (getNeuron(genome.neuronToId) == nullptr)
            createNode();
        if (!genome.enabled)
            continue;
        addLink(genome.neuronFromId, genome.neuronToId, -1, genome.linkWeight);
    }
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

void Network::disableLink(int from, int to) const
{
    auto fromNeuron = getNeuron(from);

    if (!fromNeuron)
        return;
    auto *link = fromNeuron->getLinkTo(to);
    if (link)
        Neuron::unlink(fromNeuron, link->to);
}
