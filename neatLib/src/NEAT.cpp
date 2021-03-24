#include "NEAT.hpp"

using namespace neat;

using json = nlohmann::json;

NEAT::NEAT(int pop, int inputs, int outputs, const Settings &settings):
    _settings(settings)
{
    _pop = new Population(pop, outputs, inputs, settings);
    srand(0);
}

NEAT::~NEAT() = default;

const std::vector<float> &NEAT::compute(size_t network, const std::vector<float> &inputs)
{
    return _pop->compute(network, inputs, _settings);
}

void NEAT::setFitness(size_t network, float fitness)
{
    _pop->setFitness(network, fitness);
}

size_t NEAT::size() const
{
    return _pop->size();
}

void NEAT::nextGeneration()
{
    _data.currentGeneration++;
    _data.currentIndividual = 0;

    _pop->computeSpecies(_settings);
    _pop->purge(_settings);
    _pop->genOffsprings(_settings);
}

void NEAT::save(const std::string &filepath)
{
    json output;
    std::fstream fs;

    for (auto &net : _pop->getNetworks()) {
        json object;

        if (net->dead) {
            continue;
        }

        object["innovations"] = json::array();
        for (auto &innov : net->getInnovations()) {
            json innovation;

            innovation["enabled"] = innov.enabled;
            innovation["neuronFromId"] = innov.neuronFromId;
            innovation["neuronToId"] = innov.neuronToId;
            innovation["linkWeight"] = innov.linkWeight;
            innovation["innovationId"] = innov.innovationId;
            object["innovations"].push_back(innovation);
        }

        output["networks"].push_back(object);
    }
    output["inputs"] = _pop->networksInputs;
    output["outputs"] = _pop->networksOutputs;
    // output["generations"] = _pop->;


    fs.open(filepath, std::fstream::out | std::fstream::trunc);
    fs << output.dump();
    fs.close();
}

void NEAT::load(const std::string &filepath)
{
    json input;
    std::fstream fs;
    std::string in;
    std::vector<std::unique_ptr<Network>> networks;

    fs.open(filepath, std::fstream::in);
    fs >> in;
    input = json::parse(in);

    int inputsNb = input["inputs"];
    int outputsNb = input["outputs"];

    for (auto &net : input["networks"]) {
        std::vector<neat::Genome> innovations;

        for (auto &innov : net["innovations"]) {
            neat::Genome innovation(
                innov["innovationId"],
                innov["neuronFromId"],
                innov["neuronToId"],
                innov["linkWeight"]);

            innovation.enabled = innov["enabled"];
            innovations.push_back(innovation);
        }
        networks.push_back(std::make_unique<Network>(innovations, inputsNb, outputsNb));
    }

    Population *newPop = new Population(&networks, outputsNb, inputsNb, _settings);
    _pop = newPop;

    fs.close();
}

void NEAT::compute(const std::vector<float> &, std::vector<float> &) const
{
}
