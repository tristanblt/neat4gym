#include "GymRequests.hpp"

GymRequests::GymRequests()
{
    _client = new CHTTPClient([](const std::string& a){
        std::cout << a << std::endl;
    });

    _client->InitSession();

    _headers.emplace("Content-Type", "application/json");
}

GymRequests::~GymRequests()
{
    _client->CleanupSession();
}

std::string GymRequests::createInstance(const std::string &env)
{
    json packet;

    packet["env_id"] = env;

    _client->Post(
        "http://127.0.0.1:12345/v1/envs",
        _headers,
        packet.dump(),
        _response
    );

    json response = json::parse(_response.strBody);
    return response["instance_id"];
}

void GymRequests::startMonitor(const std::string &instance_id)
{
    json packet;

    packet["video_callable"] = true;

    _client->Post(
        "http://127.0.0.1:12345/v1/envs/" + instance_id + "/monitor/start",
        _headers,
        packet.dump(),
        _response
    );
}

void GymRequests::closeMonitor(const std::string &instance_id)
{
    json packet;

    _client->Post(
        "http://127.0.0.1:12345/v1/envs/" + instance_id + "/monitor/close",
        _headers,
        packet.dump(),
        _response
    );
}

void GymRequests::step(const std::string &instance_id, const std::vector<float> &action)
{
    json packet;

    _client->Post(
        "http://127.0.0.1:12345/v1/envs/" + instance_id + "/step",
        _headers,
        packet.dump(),
        _response
    );
}

void GymRequests::reset(const std::string &instance_id)
{
    json packet;

    _client->Post(
        "http://127.0.0.1:12345/v1/envs/" + instance_id + "/reset",
        _headers,
        packet.dump(),
        _response
    );
}
