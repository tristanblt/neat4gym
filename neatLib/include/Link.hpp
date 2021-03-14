
#ifndef __LINK_H_
#define __LINK_H_

namespace neat {

    struct Neuron;

    struct Link {
        Link(Neuron *from, Neuron *to);

        /**
         * The "from" neuron of the link
         */
        Neuron *from;

        /**
         * The "to" neuron of the link
         */
        Neuron *to;

        /**
         * The weight of the link, to compute the network
         */
        float weight = 1;
    };

}

#endif // __LINK_H_
