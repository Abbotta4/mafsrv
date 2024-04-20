#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include "restserver.h"
#include "pgdb.h"

void RESTEndpoint::init(size_t thr) {
    auto opts = Pistache::Http::Endpoint::options().threads(static_cast<int>(thr));
    httpEndpoint->init(opts);
    setupRoutes();
}

void RESTEndpoint::start() {
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void RESTEndpoint::setupRoutes() {
    Pistache::Rest::Routes::Post(router, "/users/create", Pistache::Rest::Routes::bind(&RESTEndpoint::createUser, this));
    Pistache::Rest::Routes::Get(router, "/users/:uid", Pistache::Rest::Routes::bind(&RESTEndpoint::getUser, this));
    Pistache::Rest::Routes::Get(router, "/threads/:tid", Pistache::Rest::Routes::bind(&RESTEndpoint::getThread, this));
    //Pistache::Rest::Routes::Get(router, "/ready", Pistache::Rest::Routes::bind(&Generic::handleReady));
    //Pistache::Rest::Routes::Get(router, "/auth", Pistache::Rest::Routes::bind(&StatsEndpoint::doAuth, this));
}

void RESTEndpoint::getUser(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    try {
	auto uid = request.param(":uid").as<std::string>();
	std::string user = queryUsers(std::stoi(uid));
	response.send(Pistache::Http::Code::Ok, user, MIME(Application, Json));
    } catch (const std::runtime_error &bang) {
	response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
	response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void RESTEndpoint::createUser(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    try {
	int code = insertUsers(request.body());
	if (code == 0) //CREATE_OK)
	    response.send(Pistache::Http::Code::Created, "nice" , MIME(Text, Plain));
	else // error code
	    response.send(Pistache::Http::Code::Bad_Request, "Could not create user: " + std::to_string(code) , MIME(Text, Plain));
    } catch (const std::runtime_error &bang) {
	response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
	response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void RESTEndpoint::getThread(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {}
void RESTEndpoint::doAuth(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {}
