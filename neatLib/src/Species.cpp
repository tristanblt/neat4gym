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

void Species::addToNewPop(Network *network)
{
    _newPop.push_back(network);
}

void Species::purge(size_t nb)
{
    std::sort(_networks.begin(), _networks.end(), [](const Network *n1, const Network *n2) {
        return n1->fitness < n2->fitness;
    });
    for (size_t i = _networks.size() - 1; i > _networks.size() - nb && i > 0; i--) {
        _networks[i]->dead = true;
    }
    _networks.erase(std::remove_if(_networks.begin(), _networks.end(), [](Network *elem) {
        return elem->dead;
    }), _networks.end());
}

size_t Species::size() const
{
    return _networks.size();
}

std::unique_ptr<Network> Species::getOffspring(const Settings &settings)
{
    std::unique_ptr<Network> offspring;
    if (_networks.size() > 1 && Settings::doRand(settings.crossoverRate)) {
        int first = currentInNewGen % _networks.size();
        int second = 0;
        do {
            second = rand() % _networks.size();
        } while (second == first);
        offspring = Network::crossover(*_networks[first], *_networks[second]);
    } else {
        offspring = _networks[currentInNewGen % _networks.size()]->copy();
    }
    currentInNewGen++;
    _newPop.push_back(offspring.get());
    return offspring;
}

void Species::endReproduction()
{
    _networks = _newPop;
    _newPop.clear();
}
