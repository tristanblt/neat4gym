#include "Genome.hpp"

using namespace neat;

Genome::Genome(int id, int from, int to, float w):
    neuronFromId(from),
    neuronToId(to),
    linkWeight(w),
    innovationId(id)
{}
