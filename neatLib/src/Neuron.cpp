
#include "Neuron.hpp"
#include <algorithm>

using namespace neat;

Neuron::Neuron(int id) : _id(id) {

};

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
