#ifndef SPECIES_HPP_
#define SPECIES_HPP_

#include "Network.hpp"

namespace neat {
    struct Population;

    struct Species {
        Species(Population *population);
        ~Species() = default;

        Network *getBestNetwork() const;
        void computeSpecies(const Settings &settings);
        float computeSpeciesSimilarity(Network *network, const Settings &settings);
        void addNetworkToSpecies(Network *network);

        private:
            void excludeNetwork(Network *networkToExclude, const Settings &settings);

            std::vector<Network *> _networks;
            Network *_representativeNetwork;
            Population *_population;
    };
}

#endif /* !SPECIES_HPP_ */
