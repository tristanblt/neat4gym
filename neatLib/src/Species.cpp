#include "Species.hpp"
#include "Population.hpp"

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
            *network
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

float Species::computeSpeciesSimilarity(Network *network)
{
    return (Network::computeSimilarity(*_representativeNetwork, *network));
}

void Species::addNetworkToSpecies(Network *network)
{
    _networks.push_back(network);
}