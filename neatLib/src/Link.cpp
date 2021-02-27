
#include "Link.hpp"

using namespace neat;

Link::Link(Neuron &f, Neuron &t):
    from(&f),
    to(&t)
{}
