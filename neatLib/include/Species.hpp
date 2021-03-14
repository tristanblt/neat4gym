#ifndef SPECIES_HPP_
#define SPECIES_HPP_

#include "Network.hpp"

namespace neat {
    struct Population;

    struct Species {
        Species(Population *population);
        ~Species() = default;

        /**
         * @return the best network of the species
         */
        Network *getBestNetwork() const;

        /**
         * Compute the similarity of each network in comparison with the representative network.
         * Exclude a network if he is to much different
         */
        void computeSpecies(const Settings &settings);

        /**
         * Compute the similarity between the representative network and a given network
         */
        float computeSpeciesSimilarity(Network *network, const Settings &settings);

        /**
         * Add a new network to this species
         */
        void addNetworkToSpecies(Network *network);

        /**
         * @return the number of networks inside this species
         */
        size_t size() const;

        /**
         * Kill worst networks in the species
         */
        void purge(size_t nb);

        size_t maxPop = 0;
        size_t currentInNewGen = 0;

        /**
         * Take two networks and create a crossover of them
         */

        std::unique_ptr<Network> getOffspring(const Settings &settings);

        /**
         * Add the network to the future population
         */
        void addToNewPop(Network *network);

        /**
         * Move the new population to the current population
         */
        void endReproduction();

        private:
            /**
             * Exclude a network of the species because he is to different
             */
            void excludeNetwork(Network *networkToExclude, const Settings &settings);

            std::vector<Network *> _networks;
            Network *_representativeNetwork;
            std::vector<Network *> _newPop;
            Population *_population;
    };
}

#endif /* !SPECIES_HPP_ */
