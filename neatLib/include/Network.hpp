#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <memory>
#include <list>

#include "Neuron.hpp"
#include "Genome.hpp"
#include "Settings.hpp"

namespace neat {
    struct Network {
        Network(int inputs, int outputs);
        ~Network() = default;

        [[nodiscard]] std::vector<float> compute(const std::vector<float> &inputs) const;

        [[nodiscard]] static Network crossover(const Network &a, const Network &b);
        [[nodiscard]] static float computeSimilarity(const Network &a, const Network &b);

        /**
         * Mutates a weight if possible, will return false if not.
         */
        bool mutateWeight(int linkFrom, int linkTo, float delta, bool set);
        /**
         * Should only be used when there is no intent to mutate a weight.
         * It is otherwise more efficient to call mutateWeight directly.
         */
        [[nodiscard]] bool hasLink(int from, int to) const;
        /**
         * Adds a link if possible, will return false if not.
         */
        bool addLink(int from, int to, int innovationId, float weight);
        /**
         * Should only be used when there is no intent to add a link.
         * It is otherwise more efficient to call addLink directly.
         */
        [[nodiscard]] bool canAddLink(int from, int to) const;

        [[nodiscard]] int createNode();
        void disableLink(int from, int to) const;

        void computeLayers() const;

        float fitness = 0;

        Neuron *getNeuron(int id) const;

        private:
            void rebuildNetwork();
            [[nodiscard]] int getNextNeuronId();

            std::vector<std::unique_ptr<Neuron>> _inputs;
            std::vector<std::unique_ptr<Neuron>> _hiddens;
            std::vector<std::unique_ptr<Neuron>> _outputs;

            std::list<Genome> _innovations;

            int _nextNeuronId = 1;
    };
}

#endif /* !NETWORK_HPP_ */
