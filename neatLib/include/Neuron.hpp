
#ifndef _NEURON_HPP_
#define _NEURON_HPP_

#include <vector>
#include "Link.hpp"

namespace neat {

    struct Neuron {
        Neuron(int id);
        ~Neuron() = default;

        static void link(Neuron *from, Neuron *to);
        static void unlink(Neuron *from, Neuron *to);

        private:
            std::vector<Link> _from;
            std::vector<Link> _to;
            int _layer = 0;
            float _value;

            int _id;
    };

}

#endif /* _NEURON_HPP_ */
