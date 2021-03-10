
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


        NEAT(int pop, int outputs, int inputs);
        ~NEAT();

        const Data &step(bool isOver, float fitness, const std::vector<float> &inputs);

        void save(const std::string &filepath);
        void load(const std::string &filepath);

        /**
         * Computes the output using the best Network available
         */
        void compute(const std::vector<float> &inputs, std::vector<float> &outputs) const;

        Settings settings;

        private:
            void prepareNextGeneration();

            Population _pop;
            Data _data;

    };
}

#endif
