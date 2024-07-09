#pragma once

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

class RESTEndpoint {
public:
    explicit RESTEndpoint(Pistache::Address addr)
	: httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr)) {}
    void init(size_t thr);
    void start();
private:
    void setupRoutes();
    void readUser(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response);
    void createUser(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response);
    void updateUser(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response);
    void deleteUser(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response);
    void getThread(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response);
    void login(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response);
    //void doAuth(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response);

    //using Lock  = std::mutex;
    //using Guard = std::lock_guard<Lock>;
    //Lock metricsLock;

    std::shared_ptr<Pistache::Http::Endpoint> httpEndpoint;
    Pistache::Rest::Router router;
};
