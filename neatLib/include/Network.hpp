#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <memory>
#include <list>

#include "Neuron.hpp"
#include "Genome.hpp"

namespace neat {
    struct Network {
        Network(int inputs, int outputs);
        ~Network() = default;

        std::vector<float> compute(const std::vector<float> &inputs) const;

        static Network crossover(const Network &a, const Network &b);
        static float computeSimilarity(const Network &a, const Network &b);

        void mutate();

        float fitness = 0;

        private:
            void rebuildNetwork();
            int getNextNeuronId();

            std::vector<std::unique_ptr<Neuron>> _inputs;
            std::vector<std::unique_ptr<Neuron>> _hiddens;
            std::vector<std::unique_ptr<Neuron>> _outputs;

            std::list<Genome> _innovations;

            int _nextNeuronId = 1;
    };
}

#endif /* !NETWORK_HPP_ */
