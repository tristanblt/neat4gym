
#ifndef _NEURON_HPP_
#define _NEURON_HPP_

#include <vector>
#include "Link.hpp"
#include "Settings.hpp"

namespace neat {

    struct Neuron {
        Neuron(int id);
        ~Neuron() = default;

        static void link(Neuron *from, Neuron *to);
        static void unlink(Neuron *from, Neuron *to);

        const int id;
        int layer = 0;

        void computeLayersRec(size_t i = 0);
        float computeValue(const Settings &settings);

        Neuron *getNeuronTo(int id) const;
        Link *getLinkTo(int id);

        void setValue(float v);

        private:
            std::vector<Link> _from;
            std::vector<Link> _to;
            float _value;
    };

}

#endif /* _NEURON_HPP_ */
