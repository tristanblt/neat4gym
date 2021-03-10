#include "Population.hpp"
#include <algorithm>

using namespace neat;

Population::Population(int startPopulation, int outputs, int inputs):
    _startPopSize(startPopulation)
{
    auto newSpecies = _species.emplace_back(this);

    for (int i = 0; i < startPopulation; i++) {
        _networks.push_back(std::make_unique<Network>(inputs, outputs));
        newSpecies.addNetworkToSpecies(_networks[_networks.size() - 1].get());
    }
}

void Population::computeBest(const std::vector<float> &inputs, std::vector<float> &outputs, const Settings &settings) const
{
    Network *bestNetwork = nullptr;
    float bestFitness = std::numeric_limits<float>::min();

    for (auto &spec : _species) {
        auto bestSpeciesNetwork = spec.getBestNetwork();
        if (bestSpeciesNetwork->fitness > bestFitness) {
            bestFitness = bestSpeciesNetwork->fitness;
            bestNetwork = bestSpeciesNetwork;
        }
    }
    if (bestNetwork)
        outputs = bestNetwork->compute(inputs, settings);
}

void Population::compute(size_t n, const std::vector<float> &inputs, std::vector<float> &outputs, const Settings &settings) const
{
    outputs = _networks[n]->compute(inputs, settings);
}

void Population::setFitness(size_t n, float fitness) const
{
    _networks[n]->fitness = fitness;
}

size_t Population::size() const
{
    return (_networks.size());
}

void Population::computeSpecies(const Settings &settings)
{
    for (auto &spec : _species) {
        spec.computeSpecies(settings);
    }
}

void Population::purge(const Settings &settings)
{
    for (auto &specie: _species) {
        specie.maxPop = specie.size() * 1.2;
        if (specie.maxPop < 5)
            specie.maxPop += 2;
        specie.purge(settings.toKill * specie.size());
    }
    _networks.erase(
        std::remove_if(std::begin(_networks), std::end(_networks), [](const std::unique_ptr<Network> &network) {
            return network->dead;
        }),
        std::end(_networks)
    );
}

void Population::genOffsprings(const Settings &settings)
{
    int currentSpecie = 0;
    std::vector<std::unique_ptr<Network>> offsprings;
    while (offsprings.size() < _startPopSize) {
        if (Settings::doRand(settings.interspeciesCrossoverRate)) {
            offsprings.emplace_back(Network::crossover(
                                        *_networks[rand() % _networks.size()],
                                        *_networks[rand() % _networks.size()]
                                    ));
            continue;
        }

        offsprings.emplace_back(_species[currentSpecie].getOffspring(settings));
        while (_species[currentSpecie].maxPop >= _species[currentSpecie].currentInNewGen) {
            currentSpecie = (currentSpecie + 1) % _species.size();
        }
    }
}

void Population::findOrCreateSpecies(Network *network, const Settings &settings)
{
    float bestSimilarity = 0;
    Species *mostCompatibleSpecies = nullptr;

    for (auto &spec : _species) {
        float similarity = spec.computeSpeciesSimilarity(network, settings);

        if (similarity > bestSimilarity) {
            bestSimilarity = similarity;
            mostCompatibleSpecies = &spec;
        }
    }

    if (bestSimilarity >= settings.similarity && mostCompatibleSpecies) {
        mostCompatibleSpecies->addNetworkToSpecies(network);
    } else if (bestSimilarity < settings.similarity) {
        _species.emplace_back(this).addNetworkToSpecies(network);
    }
}
