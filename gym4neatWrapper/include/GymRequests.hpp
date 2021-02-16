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

        std::string createInstance(const std::string &env);
        void startMonitor(const std::string &instance_id);
        void closeMonitor(const std::string &instance_id);
        void step(const std::string &instance_id, const std::vector<float> &action);
        void reset(const std::string &instance_id);

    protected:
    private:
        CHTTPClient::HeadersMap _headers;
        CHTTPClient::HttpResponse _response;
        CHTTPClient *_client;
};

#endif /* !GYMREQUESTS_HPP_ */
