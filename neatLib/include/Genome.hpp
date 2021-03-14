#ifndef GENOME_HPP_
#define GENOME_HPP_

#include "Neuron.hpp"

namespace neat {
    /**
     * A link in the innovation
     * representation of the network
     */
    struct Genome {
        Genome(int innovationId, int from, int to, float w);
        ~Genome() = default;

        Genome(const Genome &) = default;
        Genome &operator=(const Genome &) = default;

        /**
         * The ID of the "from" neuron
         */
        int neuronFromId;
        /**
         * The ID of the "to" neuron
         */
        int neuronToId;

        /**
         * The weight of the link.
         * Important to rebuild the graph from all genomes
         */
        float linkWeight;

        /**
         * The innovation ID, which allow to create the whole history
         * of the networks
         */
        int innovationId;

        /**
         * We doesn't destroy links, we disable them with this parameter
         */
        bool enabled = true;
    };
}

#endif /* !GENOME_HPP_ */
