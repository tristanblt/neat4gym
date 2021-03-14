
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
         * Step the population.
         * This will compute the next network and at the end of a generation, will create mutations inside of the networks
         * @param isOver true at the end of an episode
         * @param fitness the score gived to the previous compute
         * @param inputs the inputs of the previous observation
         * @return the Data structure, with the compute's outputs, and informations on the actual space
         */
        const Data &step(bool isOver, float fitness, const std::vector<float> &inputs);

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
            /**
             * Prepare the population to the next generation
             * This method will also compute the mutations and the
             * crossovers of the last generation
             */
            void prepareNextGeneration();

            Population _pop;
            Data _data;

    };
}

#endif
