
#ifndef _NEURON_HPP_
#define _NEURON_HPP_

#include <vector>
#include "Link.hpp"

namespace neat {

    struct Neuron {
        Neuron();
        ~Neuron();

        void link(Neuron &other);
        void unlink(Neuron &other);

        private:
            std::vector<Link> _links;
    };

}

#endif /* _NEURON_HPP_ */
