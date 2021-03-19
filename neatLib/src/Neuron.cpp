
#include "Neuron.hpp"
#include <algorithm>
#include <cmath>

using namespace neat;

Neuron::Neuron(int _id):
    id(_id)
{}

void Neuron::link(Neuron *from, Neuron *to, float w)
{
    from->_to.emplace_back(from, to);
    to->_from.emplace_back(from, to);
    from->_to.back().weight = w;
    to->_from.back().weight = w;
}

void Neuron::unlink(Neuron *from, Neuron *to)
{
    from->_to.erase(std::find_if(from->_to.begin(), from->_to.end(), [&to](Link &link) {
        return link.to == to;
    }));
    to->_from.erase(std::find_if(to->_from.begin(), to->_from.end(), [&from](Link &link) {
        return link.from == from;
    }));
}

void Neuron::computeLayersRec(size_t i)
{
    if (i >= _to.size())
        return;
    bool changed = false;
    if (_to[i].to->layer < layer + 1) {
        changed = true;
        _to[i].to->layer = layer + 1;
    }
    computeLayersRec(i + 1);
    if (changed)
        _to[i].to->computeLayersRec();
}

static float sigmoid(float x, const Settings &settings)
{
    return 1.0 / (1.0 + exp(-x * settings.sigmoidMult));
}

#include <iostream>

float Neuron::computeValue(unsigned turn, const Settings &settings)
{
    if (_turn == turn || _from.size() == 0) {
        // if (_from.size()) {
            // std::cout << _value << std::endl;
        // }
        return _value;
    }
    _value = 0;
    for (auto &from: _from) {
        _value += from.from->computeValue(turn, settings) * from.weight;
    }
    _value = sigmoid(_value, settings);
    _turn = turn;
    if (std::isnan(_value) || _value < 0.0 || _value > 1.0 || (int)_value < 0 || (int)_value > 1) {
        std::cout << _value << std::endl;
        throw "connard";
        // exit('T');
    }
    // std::cout << _value << std::endl;
    return _value;
}

Neuron *Neuron::getNeuronTo(int id) const
{
    for (auto &link: _to) {
        if (link.to->id == id)
            return link.to;
    }
    return nullptr;
}

Link *Neuron::getLinkTo(int id)
{
    for (auto &link: _to) {
        if (link.to->id == id)
            return &link;
    }
    return nullptr;
}

Link *Neuron::getLinkFrom(int id)
{
    for (auto &link: _from) {
        if (link.from->id == id)
            return &link;
    }
    return nullptr;
}

void Neuron::setValue(float v)
{
    _value = v;
}
