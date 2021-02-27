#ifndef GENOME_HPP_
#define GENOME_HPP_

#include "Neuron.hpp"

namespace neat {
    struct Genome {
        Genome(int innovationId);
        ~Genome() = default;

        int neuronFromId;
        int neuronToId;

        float linkWeight;

        int innovationId;

        bool enabled;
    };
}

#endif /* !GENOME_HPP_ */
