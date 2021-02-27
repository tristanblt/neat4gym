#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include "Species.hpp"

namespace neat {
    struct Population {
        Population(int startPopulation, int outputs, int inputs);
        ~Population() = default;

        private:
            std::vector<std::unique_ptr<Network>> _networks;
            std::vector<Species> _species;
    };
}

#endif /* !POPULATION_HPP_ */
