
#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <memory>

#include "Neuron.hpp"
#include "Genome.hpp"
#include "Settings.hpp"

namespace neat {
    struct Network {
        Network(int inputs, int outputs);
        ~Network() = default;

        Network(const Network &network) = delete;
        Network &operator=(const Network &rhs) = delete;

        /**
         * Creates a copy of the Neurons only.
         */
        std::unique_ptr<Network> copy() const;

        /**
         * Computes the result of the Network given inputs and settings.
         * The settings are used for the sigmoid function.
         */
        [[nodiscard]] std::vector<float> compute(const std::vector<float> &inputs, const Settings &settings) const;

        /**
         * Makes a crossover child given two networks.
         * The child is an allocated new Network wrapped in a unique_ptr.
         */
        [[nodiscard]] static std::unique_ptr<Network> crossover(const Network &a, const Network &b);

        /**
         * Computes the similarity between two given networks and settings.
         */
        [[nodiscard]] static float computeSimilarity(const Network &a, const Network &b, const Settings &settings);

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

        /**
         * Creates a new node and returns it's ID.
         */
        int createNode();

        /**
         * Disables a link if the link exists.
         */
        void disableLink(int from, int to) const;

        /**
         * Compute the layers recursively for all the nodes in the Network.
         * Should be used after adding each link.
         */
        void computeLayers() const;

        /**
         * Returns the neuron if it exists and nullptr if it doesn't.
         */
        Neuron *getNeuron(int id) const;

        /**
         * Returns two random neuron IDs by reference. The neurons can be linked (from: n1, to: n2).
         */
        void getTwoNeuronIds(int &n1, int &n2) const;

        /**
         * Returns a random link between two neurons. Will divide by 0 if there are no innovations.
         */
        const Genome &getRandomLink() const;

        float fitness = 0;
        bool dead = false;
        int mutations = 0;

    private:

        /**
         * Rebuilds the network using _innovations.
         */
        void rebuildNetwork();

        /**
         * Returns and increments _nextNeuronId.
         */
        [[nodiscard]] int getNextNeuronId();

        std::vector<std::unique_ptr<Neuron>> _inputs;
        std::vector<std::unique_ptr<Neuron>> _hiddens;
        std::vector<std::unique_ptr<Neuron>> _outputs;

        std::vector<Genome> _innovations;

        int _nextNeuronId = 1;
    };
}

#endif /* !NETWORK_HPP_ */
