
#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <memory>

#include "Neuron.hpp"
#include "Genome.hpp"
#include "Settings.hpp"

namespace neat {
    struct Network {
        /**
         * @param inputs the number of inputs in the network
         * @param outputs the number of outputs in the network
         */
        Network(int inputs, int outputs);
        Network(std::vector<Genome> innovations, int inputs, int outputs);
        ~Network() = default;

        Network(const Network &network) = delete;
        Network &operator=(const Network &rhs) = delete;

        /**
         * Creates a copy of the Neurons only.
         * @return a unique pointer of the new network
         */
        std::unique_ptr<Network> copy() const;

        /**
         * Computes the result of the Network given inputs and settings.
         * The settings are used for the sigmoid function.
         * @param inputs the initial inputs to compute the outputs
         * @param settings the settings of the algoritm
         * @return the computed outputs
         */
        [[nodiscard]] const std::vector<float> &compute(const std::vector<float> &inputs, const Settings &settings);

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
         * Creates a new node with a provided ID and returns it's ID.
         */
        int createNode(int id);

        /**
         * Disables a link if the link exists.
         */
        void disableLink(int from, int to) const;

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

        /**
         * Returns a random link between two neurons. Will divide by 0 if there are no innovations.
         */
        Genome &getRandomLink();

        /**
         * Returns a random link between two neurons. Will divide by 0 if there are no innovations.
         */
        std::vector<Genome> &getInnovations();

        float fitness = 0;
        bool dead = false;
        int mutations = 0;

    private:

        /**
         * Rebuilds the network using _innovations.
         */
        void rebuildNetwork();
        void rebuildNetwork(int inputs, int outputs);

        std::vector<std::unique_ptr<Neuron>> _inputs;
        std::vector<std::unique_ptr<Neuron>> _hiddens;
        std::vector<std::unique_ptr<Neuron>> _outputs;

        std::vector<Genome> _innovations;

        std::vector<float> _values;

    };
}

#endif /* !NETWORK_HPP_ */
