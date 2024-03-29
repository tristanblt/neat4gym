#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <limits>

#include "Species.hpp"
#include "Settings.hpp"

namespace neat {

    struct Population {
        Population(int startPopulation, int outputs, int inputs, const Settings &settings);
        Population(std::vector<std::unique_ptr<Network>> *networks, int outputs, int inputs, const Settings &settings);
        ~Population() = default;

        /**
         * Compute the best network of the population
         */
        void computeBest(const std::vector<float> &inputs, std::vector<float> &outputs, const Settings &settings) const;

        /**
         * Compute a given network
         * @param n the index of the network in the population
         */
        const std::vector<float> &compute(size_t n, const std::vector<float> &inputs, const Settings &settings) const;

        /**
         * Set the fitness of a given network
         * @param n the index of the network in the population
         */
        void setFitness(size_t n, float fitness) const;

        /**
         * @return the size of the population
         */
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

        /**
         * Try to find a compatible species for a network,
         * and create a new species if there is no results
         */
        void findOrCreateSpecies(Network *network, const Settings &settings);

        /**
         * Get the population's network
         */
        std::vector<std::unique_ptr<Network>> &getNetworks();

        int networksInputs;
        int networksOutputs;

    private:
        /**
         * Mutate all networks of the population
         */
        void mutateNetworks(const Settings &settings);

        /**
         * Mutate networks by adding a new link
         */
        void addLink(const std::unique_ptr<Network> &target, const Settings &settings);

        /**
         * Mutate networks by modify link's weight
         */
        void mutateLink(const Settings &settings, const std::unique_ptr<Network> &target);

        /**
         * Mutate networks by adding a new node
         */
        void addNode(const std::unique_ptr<Network> &target, const Settings &settings);

        std::vector<std::unique_ptr<Network>> _networks;
        std::vector<Species> _species;

        size_t _size;

        int _innovationId = 0;
        int _nextNeuronId;
    };
}

#endif /* !POPULATION_HPP_ */
