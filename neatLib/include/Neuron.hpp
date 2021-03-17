
#ifndef _NEURON_HPP_
#define _NEURON_HPP_

#include <vector>
#include "Link.hpp"
#include "Settings.hpp"

namespace neat {

    struct Neuron {
        Neuron(int id);
        ~Neuron() = default;

        /**
         * Link two given neurons in the graph representation
         */
        static void link(Neuron *from, Neuron *to);

        /**
         * Unlink two given neurons in the graph representation
         */
        static void unlink(Neuron *from, Neuron *to);

        /**
         * The unique neuron ID
         */
        const int id;

        /**
         * The layer of the neuron in the graph
         */
        int layer = -1;

        /**
         * Compute the layer of the neuron
         */
        void computeLayersRec(size_t i = 0);

        /**
         * Compute the neuron value
         */
        float computeValue(const Settings &settings);

        /**
         * @return the linked "to" neuron
         */
        Neuron *getNeuronTo(int id) const;

        /**
         * @return the "to" link
         */
        Link *getLinkTo(int id);

        /**
         * Set the new value
         */
        void setValue(float v);

        private:
            std::vector<Link> _from;
            std::vector<Link> _to;
            float _value;
    };

}

#endif /* _NEURON_HPP_ */
