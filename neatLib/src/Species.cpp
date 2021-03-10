#include "Species.hpp"
#include "Population.hpp"
#include <algorithm>

using namespace neat;

Species::Species(Population *population) :
    _population(population)
{
}

Network *Species::getBestNetwork() const
{
    Network *bestNetwork;
    float bestFitness = std::numeric_limits<float>::min();

    for (auto &network : _networks) {
        if (network->fitness > bestFitness) {
            bestFitness = network->fitness;
            bestNetwork = network;
        }
    }
    return (bestNetwork);
}

void Species::computeSpecies(const Settings &settings)
{
    for (auto &network : _networks) {
        float similarity = Network::computeSimilarity(
            *_representativeNetwork,
            *network,
            settings
        );

        if (similarity < settings.similarity) {
            excludeNetwork(network, settings);
        }
    }
}

void Species::excludeNetwork(Network *networkToExclude, const Settings &settings)
{
    auto toExclude = std::find(
        _networks.begin(),
        _networks.end(),
        networkToExclude
    );

    _population->findOrCreateSpecies(networkToExclude, settings);
    _networks.erase(toExclude);
}

float Species::computeSpeciesSimilarity(Network *network, const Settings &settings)
{
    return (Network::computeSimilarity(*_representativeNetwork, *network, settings));
}

void Species::addNetworkToSpecies(Network *network)
{
    _networks.push_back(network);
}
