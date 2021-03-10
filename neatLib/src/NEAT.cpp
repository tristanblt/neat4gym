#include "NEAT.hpp"

using namespace neat;

NEAT::NEAT(int pop, int inputs, int outputs):
    _pop(pop, outputs, inputs)
{
}

NEAT::~NEAT() = default;

const NEAT::Data &NEAT::step(bool isOver, float fitness, const std::vector<float> &inputs)
{
    if (isOver) {
        _pop.setFitness(_data.currentIndividual, fitness);
        _data.currentIndividual++;
        if (_data.currentIndividual >= _pop.size()) {
            prepareNextGeneration();
        }
    }
    _pop.compute(_data.currentIndividual, inputs, _data.outputs, settings);
    return _data;
}

void NEAT::prepareNextGeneration()
{
    _data.currentGeneration++;
    _data.currentIndividual = 0;
    _pop.computeSpecies(settings);
    _pop.purge(settings);
    _pop.genOffsprings(settings);
}

void NEAT::save(const std::string &filepath)
{}

void NEAT::load(const std::string &filepath)
{}

void NEAT::compute(const std::vector<float> &inputs, std::vector<float> &outputs) const
{
}
