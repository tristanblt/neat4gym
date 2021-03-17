#include "GymRequests.hpp"

GymRequests::GymRequests()
{
    _client = new CHTTPClient([](const std::string& a){});

    _client->InitSession();

    _headers.emplace("Content-Type", "application/json");
}

GymRequests::~GymRequests()
{
    _client->CleanupSession();
}

std::string GymRequests::createInstance(const std::string &env)
{
    CHTTPClient::HttpResponse httpResponse;
    json packet;

    packet["env_id"] = env;
    _client->Post(
        _endpoint + "/envs",
        _headers,
        packet.dump(),
        httpResponse
    );

    json response = json::parse(httpResponse.strBody);
    return response["instance_id"];
}

void GymRequests::startMonitor(const std::string &instanceId)
{
    CHTTPClient::HttpResponse httpResponse;
    json packet;

    _client->Post(
        _endpoint + "/envs/" + instanceId + "/monitor/start",
        _headers,
        packet.dump(),
        httpResponse
    );
}

void GymRequests::closeMonitor(const std::string &instanceId)
{
    CHTTPClient::HttpResponse httpResponse;
    json packet;

    _client->Post(
        _endpoint + "/envs/" + instanceId + "/monitor/close",
        _headers,
        packet.dump(),
        httpResponse
    );
}

GymRequests::Space GymRequests::actionSpace(const std::string &instanceId)
{
    CHTTPClient::HttpResponse httpResponse;
    Space space;

    _client->Get(
        _endpoint + "/envs/" + instanceId + "/action_space",
        _headers,
        httpResponse
    );

    json response = json::parse(httpResponse.strBody);
    space.name = response["info"]["name"];

    if (space.name == "Discrete")
        space.n = response["info"]["n"];
    else
        space.n = response["info"]["shape"][0];
    return (space);
}

GymRequests::Space GymRequests::observationSpace(const std::string &instanceId)
{
    CHTTPClient::HttpResponse httpResponse;
    Space space;

    _client->Get(
        _endpoint + "/envs/" + instanceId + "/observation_space",
        _headers,
        httpResponse
    );
    // std::cout << httpResponse.strBody << std::endl;

    json response = json::parse(httpResponse.strBody);

    space.name = response["info"]["name"];
    if (space.name == "Discrete")
        space.n = response["info"]["n"];
    else
        space.n = response["info"]["shape"][0];
    return (space);
}

GymRequests::StepData GymRequests::step(const std::string &instanceId, const std::vector<float> &action)
{
    CHTTPClient::HttpResponse httpResponse;
    json packet;
    StepData data;
    Space space = actionSpace(instanceId);

    if (space.name == "Discrete") {
        packet["action"] = (int)action[0];
    } else {
        packet["action"] = action;
    }


    packet["render"] = true;
    std::cout << packet << std::endl;
    _client->Post(
        _endpoint + "/envs/" + instanceId + "/step",
        _headers,
        packet.dump(),
        httpResponse
    );

    std::cout << httpResponse.strBody << std::endl;

    json response = json::parse(httpResponse.strBody);

    for (size_t i = 0; i < response["observation"].size(); i++)
        data.inputs.push_back(response["observation"][i]);
    data.isOver = response["done"];
    data.score = response["reward"];
    return data;
}

std::vector<float> GymRequests::reset(const std::string &instanceId)
{
    CHTTPClient::HttpResponse httpResponse;
    json packet;

    _client->Post(
        _endpoint + "/envs/" + instanceId + "/reset",
        _headers,
        packet.dump(),
        httpResponse
    );

    json response = json::parse(httpResponse.strBody);
    return response["observation"];
}

void GymRequests::setEndpoint(const std::string &endpoint)
{
    _endpoint = endpoint;
}