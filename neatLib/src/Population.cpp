#include "Population.hpp"

using namespace neat;

Population::Population(int startPopulation, int outputs, int inputs)
{
    _networks.reserve(startPopulation);
    // TODO: create neurons
}

void Population::computeBest(const std::vector<float> &inputs, std::vector<float> &outputs) const
{

}

void Population::compute(size_t n, const std::vector<float> &inputs, std::vector<float> &outputs) const
{

}

void Population::setFitness(size_t n, float fitness) const
{

}

size_t Population::size() const
{

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
