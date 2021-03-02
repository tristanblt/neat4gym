#ifndef SPECIES_HPP_
#define SPECIES_HPP_

#include "Network.hpp"

namespace neat {
    class Species {
        public:
            Species();
            ~Species() = default;

            Network &getBestNetwork() const;

        private:
            std::vector<Network *> _networks;
    };
}

#endif /* !SPECIES_HPP_ */
