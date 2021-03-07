
#include "Neuron.hpp"
#include <algorithm>

using namespace neat;

Neuron::Neuron(int _id):
    id(_id)
{}

void Neuron::link(Neuron *from, Neuron *to)
{
    from->_to.emplace_back(from, to);
    to->_from.emplace_back(from, to);
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
    if (_to[i].to->layer < layer + 1)
        _to[i].to->layer = layer + 1;
    computeLayersRec(i + 1);
    _to[i].to->computeLayersRec();
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
