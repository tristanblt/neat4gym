
#ifndef __LINK_H_
#define __LINK_H_

namespace neat {

    struct Neuron;

    struct Link {
        Link(Neuron *from, Neuron *to);

        Neuron *from;
        Neuron *to;

        float weight = 1;
    };

}

#endif // __LINK_H_
