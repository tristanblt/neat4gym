#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <memory>
#include <list>

#include "Neuron.hpp"
#include "Genome.hpp"

namespace neat {
    struct Network {
        Network();
        ~Network() = default;

        void mutate();
        std::vector<float> compute(const std::vector<float> &inputs) const;

        static Network crossover(const Network &a, const Network &b);
        static float computeSimilarity(const Network &a, const Network &b);

        private:
            void rebuildNetwork();

            std::vector<std::unique_ptr<Neuron>> _inputs;
            std::vector<std::unique_ptr<Neuron>> _hiddens;
            std::vector<std::unique_ptr<Neuron>> _outputs;

            std::list<Genome> _innovations;
    };
}

#endif /* !NETWORK_HPP_ */
