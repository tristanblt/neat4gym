#include "HTTPClient.h"
#include <nlohmann/json.hpp>
#include <iostream>

#ifndef GYMREQUESTS_HPP_
#define GYMREQUESTS_HPP_

using json = nlohmann::json;

class GymRequests {
    public:
        GymRequests();
        ~GymRequests();

        struct StepData {
            std::vector<float> inputs;
            float score;
            bool isOver;
        };

        struct Space {
            std::string name;
            int n;
        };

        std::string createInstance(const std::string &env);
        void startMonitor(const std::string &instanceId);
        void closeMonitor(const std::string &instanceId);
        Space actionSpace(const std::string &instanceId);
        Space observationSpace(const std::string &instanceId);
        StepData step(const std::string &instanceId, const std::vector<float> &action);
        std::vector<float> reset(const std::string &instanceId);

        void setEndpoint(const std::string &endpoint);

    protected:
    private:
        CHTTPClient::HeadersMap _headers;
        CHTTPClient *_client;
        std::string _endpoint;
};

#endif /* !GYMREQUESTS_HPP_ */
