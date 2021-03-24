#include "Network.hpp"
#include "Link.hpp"
#include "Settings.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace neat;

Network::Network(int inputs, int outputs)
{
    _inputs.reserve(inputs);
    _outputs.reserve(outputs);

    int neuronId = 0;
    for (int i = 0; i < inputs; i++) {
        _inputs.push_back(std::make_unique<Neuron>(neuronId++));
    }
    for (int i = 0; i < outputs; i++)
        _outputs.push_back(std::make_unique<Neuron>(neuronId++));
}

Network::Network(std::vector<Genome> innovations, int inputs, int outputs)
{
    _innovations = innovations;
    rebuildNetwork(inputs, outputs);
}


std::unique_ptr<Network> Network::copy() const
{
    std::unique_ptr<Network> n = std::make_unique<Network>(_inputs.size(), _outputs.size());

    n->_innovations = _innovations;
    n->rebuildNetwork();
    return n;
}

const std::vector<float> &Network::compute(const std::vector<float> &inputs, const Settings &settings)
{
    _values.clear();
    for (size_t i = 0; i < _inputs.size(); ++i ) {
        _inputs[i]->setValue(inputs[i]);
    }
    _values.reserve(_outputs.size());
    unsigned turn = _outputs.front()->_turn + 1;
    for (auto &output: _outputs) {
        _values.push_back(output->computeValue(turn, settings));
        if (_values.back() > 1.0)
            _values.back() = 1;
        else if (_values.back() < 0.0)
            _values.back() = 0;
    }
    return _values;
}

std::unique_ptr<Network> Network::crossover(const Network &a, const Network &b)
{
    const Network &best = a.fitness > b.fitness ? a : b;
    const Network &worst = a.fitness > b.fitness ? b : a;
    std::unique_ptr<Network> child = std::make_unique<Network>(a._inputs.size(), a._outputs.size());

    int maxInnovatoinIdWorst = worst._innovations.back().innovationId;
    for (const auto &elem: best._innovations) {
        if (elem.innovationId > maxInnovatoinIdWorst) {
            // excess
            child->_innovations.push_back(elem);
            continue;
        }
        auto other = std::find_if(std::begin(worst._innovations), std::end(worst._innovations), [&elem](const Genome &genome) {
            return genome.innovationId == elem.innovationId;
        });
        if (other == std::end(worst._innovations)) {
            // disjoint
            child->_innovations.push_back(elem);
            continue;
        }
        // matching
        if (rand() % 2) {
            child->_innovations.push_back(elem);
        } else {
            child->_innovations.push_back(*other);
        }
    }
    child->rebuildNetwork();
    return child;
}

float Network::computeSimilarity(const Network &a, const Network &b, const Settings &settings)
{
    size_t n = (a._innovations.size() > b._innovations.size()) ?
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
    Neuron *n2 = getNeuron(to);

    if (!n2)
        return false;
    auto *link = n2->getLinkFrom(from);
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
    // if (!canAddLink(from, to))
    //     return false;

    if (from == to)
        return false;
    Neuron *n1 = getNeuron(from), *n2 = getNeuron(to);
    if (n1 == nullptr || n2 == nullptr)
        return false;
    Neuron::link(n1, n2, weight);
    if (n1->hasLoop(n1)) {
        Neuron::unlink(n1, n2);
        return false;
    }
    if (innovationId != -1)
        _innovations.emplace_back(innovationId, from, to, weight);
    // n1->computeLayersRec();
    return true;
}

int Network::createNode(int id)
{
    _hiddens.emplace_back(std::make_unique<Neuron>(id));
    return id;
}

// bool Network::canAddLink(int from, int to) const
// {
    // if (from == to)
    //     return false;
    // int layer1 = -2, layer2 = -2;

    // for (auto &ptr: _inputs) {
    //     if (ptr->id == from) {
    //         if (layer2 != -2)
    //             return false; // cannot link input links together
    //         // layer1 = ptr->layer;
    //     } else if (ptr->id == to) {
    //         if (layer1 != -2)
    //             return false; // cannot link input links together
    //         // layer2 = ptr->layer;
    //     }
    // }
    // for (auto &ptr: _hiddens) {
    //     if (ptr->id == from) {
    //         layer1 = ptr->layer;
    //         if (layer1 == -1)
    //             return true;
    //         if (layer2 != -2)
    //             return layer1 <= layer2 && !hasLink(from, to);
    //     } else if (ptr->id == to) {
    //         layer2 = ptr->layer;
    //         if (layer2 == -1)
    //             return true;
    //         if (layer1 != -2)
    //             return layer1 <= layer2 && !hasLink(from, to);
    //     }
    // }
    // // both are outputs
    // if (layer1 == -2 && layer2 == -2)
    //     return false;
    // for (auto &ptr: _outputs) {
    //     if (ptr->id == from) {
    //         layer1 = ptr->layer;
    //         if (layer1 == -1)
    //             return true;
    //         if (layer2 != -2)
    //             return layer1 <= layer2 && !hasLink(from, to);
    //     } else if (ptr->id == to) {
    //         layer2 = ptr->layer;
    //         if (layer2 == -1)
    //             return true;
    //         if (layer1 != -2)
    //             return layer1 <= layer2 && !hasLink(from, to);
    //     }
    // }
    // return false;
//     return false;
// }

void Network::rebuildNetwork()
{
    rebuildNetwork(_inputs.size(), _outputs.size());
}

void Network::rebuildNetwork(int inputsSize, int outputsSize)
{
    _inputs.clear();
    _hiddens.clear();
    _outputs.clear();
    int nextNeuronId = 0;

    for (int i = 0; i < inputsSize; i++) {
        _inputs.push_back(std::make_unique<Neuron>(nextNeuronId++));
    }
    for (int i = 0; i < outputsSize; i++)
        _outputs.push_back(std::make_unique<Neuron>(nextNeuronId++));

    for (auto &genome : _innovations) {
        if (!genome.enabled)
            continue;
        if (getNeuron(genome.neuronFromId) == nullptr)
            createNode(genome.neuronFromId);
        if (getNeuron(genome.neuronToId) == nullptr)
            createNode(genome.neuronToId);
        addLink(genome.neuronFromId, genome.neuronToId, -1, genome.linkWeight);
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

const Genome &Network::getRandomLink() const
{
    return _innovations[rand() % _innovations.size()];
}

Genome &Network::getRandomLink()
{
    return _innovations[rand() % _innovations.size()];
}

void Network::getTwoNeuronIds(int &n1, int &n2) const
{
    // Chose randomly between inputs and hiddens (given their size)
    if (Settings::doRand((float)(_inputs.size()) / (float)(_inputs.size() + _hiddens.size()))) {
        auto &neur = _inputs[rand() % _inputs.size()];
        n1 = neur->id;
    } else {
        auto &neur = _hiddens[rand() % _hiddens.size()];
        n1 = neur->id;
    }
    // Chose randomly between outputs and hiddens (given their size)
    if (Settings::doRand((float)(_outputs.size()) / (float)(_outputs.size() + _hiddens.size()))) {
        n2 = _outputs[rand() % _outputs.size()]->id;
    } else {
        auto *neur = &_hiddens[rand() % _hiddens.size()];
        n2 = neur->get()->id;
    }
}

std::vector<Genome> &Network::getInnovations()
{
    return _innovations;
}
