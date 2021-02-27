
#ifndef _NEAT_HPP_
#define _NEAT_HPP_

#include "Population.hpp"
#include <functional>

namespace neat {

    struct Data {
        std::vector<float> outputs;
        float fitness = 0;
    };

    struct NEAT {
        NEAT(int pop, int outputs, int inputs);
        ~NEAT();
        void train(int generations);

        const Data &step(bool isOver, const std::vector<float> &inputs);

        void save(const std::string &filepath);
        void load(const std::string &filepath);

        void compute(const std::vector<float> &_inputs, std::vector<float> &_outputs);

        private:
            Population _pop;
            Data _data;

    };
}

#endif
