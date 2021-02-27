
#ifndef _NEURON_HPP_
#define _NEURON_HPP_

#include <vector>
#include "Link.hpp"

namespace neat {

    struct Neuron {
        Neuron();
        ~Neuron();

        static void link(Neuron &from, Neuron &to);
        static void unlink(Neuron &from, Neuron &to);

        private:
            std::vector<Link> _links;
            int _layer = 0;
    };

}

#endif /* _NEURON_HPP_ */
