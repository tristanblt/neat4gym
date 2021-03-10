#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <limits>

#include "Species.hpp"
#include "Settings.hpp"

namespace neat {

    struct Population {
        Population(int startPopulation, int outputs, int inputs);
        ~Population() = default;

        void computeBest(const std::vector<float> &inputs, std::vector<float> &outputs, const Settings &settings) const;

        void compute(size_t n, const std::vector<float> &inputs, std::vector<float> &outputs, const Settings &settings) const;
        void setFitness(size_t n, float fitness) const;

        size_t size() const;

        /**
         * Regroup individuals in species
         */
        void computeSpecies(const Settings &settings);

        /**
         * This function lets you kill off members of species
         */
        void purge(const Settings &settings);

        void genOffsprings(const Settings &settings);

        void findOrCreateSpecies(Network *network, const Settings &settings);

        private:
            void mutateNetworks(const Settings &settings) const;
            void addLink(const std::unique_ptr<Network> &target, const Settings &settings) const;
            void mutateLink(const Settings &settings, const std::unique_ptr<Network> &target) const;
            void addNode(const std::unique_ptr<Network> &target, const Settings &settings) const;

            std::vector<std::unique_ptr<Network>> _networks;
            std::vector<Species> _species;

            size_t _startPopSize;

            int _innovationId = 0;
    };
}

#endif /* !POPULATION_HPP_ */
