
#ifndef _NEAT_HPP_
#define _NEAT_HPP_

#include "Population.hpp"
#include <functional>
#include <iostream>

namespace neat {

    struct NEAT {

        struct Data {
            std::vector<float> outputs;
            size_t currentGeneration = 0;
            size_t currentIndividual = 0;
        };

        /**
         * Construct NEAT structure.
         * @param pop the number of networks in the population
         * @param outputs the number of outputs in the networks
         * @param inputs the number of inputs in the networks
         */
        NEAT(int pop, int outputs, int inputs);
        ~NEAT();

        /**
         * Get the result of a network
         */
        const std::vector<float> &compute(size_t network, const std::vector<float> &inputs);

        /**
         * Sets the fitness for a given network
         */
        void setFitness(size_t network, float fitness);

        /**
         * Mutates, generates offsprings and replaces the old population.
         */
        void nextGeneration();

        /**
         * Returns the population size
         */
        size_t size() const;

        /**
         * Save the current population in a file.
         * @param filepath the file to save the population
         */
        void save(const std::string &filepath);

        /**
         * Load the file, and replace the actual population by the file's population.
         * @param filepath the file to load the new population
         */
        void load(const std::string &filepath);

        /**
         * Computes the output using the best Network available
         * @param inputs the inputs to give to the best network
         * @param outputs the ouputs to fill with the computed outputs
         */
        void compute(const std::vector<float> &inputs, std::vector<float> &outputs) const;

        /**
         * The current settings of the algoritm
         */
        Settings settings;

        private:
            Population _pop;
            Data _data;

    };
}

#endif
