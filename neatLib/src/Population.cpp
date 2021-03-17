#include "Population.hpp"
#include <algorithm>

using namespace neat;

Population::Population(int startPopulation, int outputs, int inputs):
    _size(startPopulation)
{
    auto newSpecies = _species.emplace_back(this);
    for (int i = 0; i < startPopulation; i++) {
        _networks.push_back(std::make_unique<Network>(inputs, outputs));
        newSpecies.addNetworkToSpecies(_networks[_networks.size() - 1].get());
    }
    for (int i = 0; i < inputs; i++) {
        for (int j = 0; j < outputs; j++) {
            int innovationId = _innovationId++;
            for (auto &network: _networks) {
                network->addLink(i, j + inputs, innovationId, 1);
            }
        }
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
    offsprings.reserve(_size);
    while (offsprings.size() < _size) {
        if (Settings::doRand(settings.interspeciesCrossoverRate)) {
            offsprings.emplace_back(Network::crossover(
                                        *_networks[rand() % _networks.size()],
                                        *_networks[rand() % _networks.size()]
                                    ));
            _species[0].addToNewPop(offsprings.back().get()); // will be added to new species next time
            continue;
        }

        offsprings.emplace_back(_species[currentSpecie].getOffspring(settings));
        while (_species[currentSpecie].maxPop >= _species[currentSpecie].currentInNewGen) {
            currentSpecie = (currentSpecie + 1) % _species.size();
        }
    }
    _networks.swap(offsprings);
    for (auto &sp: _species)
        sp.endReproduction();
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

void Population::addLink(const std::unique_ptr<Network> &target, const Settings &settings) const
{
    int n1 = 0, n2 = 0;

    target->getTwoNeuronIds(n1, n2);
    if (target->canAddLink(n1, n2))
        return addNode(target, settings);
    for (auto &network: _networks) {
        if (network->mutations > settings.maxMutation || !Settings::doRand(settings.doMutation))
            continue;
        if (network->addLink(n1, n2, _innovationId, 1))
            network->mutations++;
    }
}

void Population::mutateLink(const Settings &settings, const std::unique_ptr<Network> &target) const
{
    bool set = Settings::doRand(settings.mutationChangeWeight);
    float delta = 0;
    if (set) {
        delta = (float)(rand() % 1000) / 1000 * settings.maxMutation * 10.0;
    } else {
        delta = (float)(rand() % 1000) / 1000 * settings.maxMutation;
    }

    const auto &link = target->getRandomLink();
    for (auto &network: _networks) {
        if (network->mutations > settings.maxMutation || !Settings::doRand(settings.doMutation))
            continue;
        if (network->mutateWeight(link.neuronFromId, link.neuronToId, delta, set)) {
            network->mutations++;
        }
    }
}

void Population::addNode(const std::unique_ptr<Network> &target, const Settings &settings) const
{
    // TODO
    (void)target;
    (void)settings;
}


void Population::mutateNetworks(const Settings &settings) const
{
    bool didMutation = false;
    for (auto &network: _networks) {
        if (network->mutations < settings.minMutations) {
            didMutation = true;
            if (Settings::doRand(settings.addLinkRate)) {
                addLink(network, settings);
            } else if (Settings::doRand(settings.addNodeRate)) {
                addNode(network, settings);
            } else {
                mutateLink(settings, network);
            }
            break;
        }
    }
    if (didMutation)
        mutateNetworks(settings);
}
