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
        size_t size() const;
        void purge(size_t nb);

        size_t maxPop = 0;
        size_t currentInNewGen = 0;

        std::unique_ptr<Network> getOffspring(const Settings &settings);
        void addToNewPop(Network *network);
        void endReproduction();

        private:
            void excludeNetwork(Network *networkToExclude, const Settings &settings);

            std::vector<Network *> _networks;
            Network *_representativeNetwork;
            std::vector<Network *> _newPop;
            Population *_population;
    };
}

#endif /* !SPECIES_HPP_ */
