#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include "Species.hpp"

namespace neat {
    struct Population {
        Population(int startPopulation);
        ~Population() = default;

        private:
            std::vector<Species> _species;
    };
}

#endif /* !POPULATION_HPP_ */
