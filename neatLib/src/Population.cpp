#include "Population.hpp"

using namespace neat;

Population::Population(int startPopulation, int outputs, int inputs)
{
    _networks.reserve(startPopulation);
    // TODO: create neurons
    // TODO: create initial species
}

void Population::computeBest(const std::vector<float> &inputs, std::vector<float> &outputs) const
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
        outputs = bestNetwork->compute(inputs);
}

void Population::compute(size_t n, const std::vector<float> &inputs, std::vector<float> &outputs) const
{
    outputs = _networks[n]->compute(inputs);
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

}

void Population::purge(const Settings &settings)
{

}

void Population::genOffsprings(const Settings &settings)
{

}

void Population::findOrCreateSpecies(Network *network, const Settings &settings)
{
    float bestSimilarity = 0;
    Species *mostCompatibleSpecies = nullptr;

    for (auto &spec : _species) {
        float similarity = spec.computeSpeciesSimilarity(network);

        if (similarity > bestSimilarity) {
            bestSimilarity = similarity;
            mostCompatibleSpecies = &spec;
        }
    }

    if (bestSimilarity >= settings.similarity && mostCompatibleSpecies) {
        // TODO: add network to species
    } else if (bestSimilarity < settings.similarity) {
        // TODO: create new species for this network
    }
}
