#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <iostream>
#include <restserver.h>
#include <pgdb.h>

void RESTEndpoint::init(size_t thr) {
    auto opts = Pistache::Http::Endpoint::options().threads(static_cast<int>(thr));
    httpEndpoint->init(opts);
    setupRoutes();
}

void RESTEndpoint::start() {
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
    std::cout << "Successfully started endpoints" << std::endl;
}

void RESTEndpoint::setupRoutes() {
    Pistache::Rest::Routes::Post(router, "/users/create", Pistache::Rest::Routes::bind(&RESTEndpoint::createUser, this));
    Pistache::Rest::Routes::Get(router, "/users/:uid", Pistache::Rest::Routes::bind(&RESTEndpoint::readUser, this));
    Pistache::Rest::Routes::Patch(router, "/users/:uid", Pistache::Rest::Routes::bind(&RESTEndpoint::updateUser, this));
    Pistache::Rest::Routes::Delete(router, "/users/:uid", Pistache::Rest::Routes::bind(&RESTEndpoint::deleteUser, this));
    Pistache::Rest::Routes::Post(router, "/threads/create", Pistache::Rest::Routes::bind(&RESTEndpoint::createThread, this));
    Pistache::Rest::Routes::Get(router, "/threads/:tid", Pistache::Rest::Routes::bind(&RESTEndpoint::readThread, this));
    Pistache::Rest::Routes::Get(router, "/threads", Pistache::Rest::Routes::bind(&RESTEndpoint::readAllThreads, this));
    Pistache::Rest::Routes::Patch(router, "/threads/:tid", Pistache::Rest::Routes::bind(&RESTEndpoint::updateThread, this));
    Pistache::Rest::Routes::Delete(router, "/threads/:tid", Pistache::Rest::Routes::bind(&RESTEndpoint::deleteThread, this));
    Pistache::Rest::Routes::Post(router, "/login", Pistache::Rest::Routes::bind(&RESTEndpoint::login, this));
    //Pistache::Rest::Routes::Get(router, "/ready", Pistache::Rest::Routes::bind(&Generic::handleReady));
    //Pistache::Rest::Routes::Get(router, "/auth", Pistache::Rest::Routes::bind(&StatsEndpoint::doAuth, this));
    std::cout << "Successfully set routes" << std::endl;
}

void RESTEndpoint::readUser(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    std::cout << "received readUser" << std::endl;
    try {
        auto uid = request.param(":uid").as<std::string>();
        std::string user = selectFromUsers(std::stoi(uid));
        if (user == "")
            throw std::runtime_error("User not found");
        else
            response.send(Pistache::Http::Code::Ok, user, MIME(Application, Json));
    } catch (const std::runtime_error &bang) {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void RESTEndpoint::deleteUser(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    std::cout << "received deleteUser" << std::endl;
    try {
        auto uid = request.param(":uid").as<std::string>();
        int code = deleteFromUsers(std::stoi(uid));
        if (code == 0) // DELETE_OK
            response.send(Pistache::Http::Code::Ok, "Deleted " + uid, MIME(Text, Plain));
        else // error code
            response.send(Pistache::Http::Code::Bad_Request, "Could not delete user: " + std::to_string(code), MIME(Text, Plain));
    } catch (const std::runtime_error &bang) {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void RESTEndpoint::createUser(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    std::cout << "received createUser" << std::endl;
    try {
        int code = insertIntoUsers(request.body());
        if (code == 0) // CREATE_OK
            response.send(Pistache::Http::Code::Created, "Created" , MIME(Text, Plain));
        else // error code
            response.send(Pistache::Http::Code::Bad_Request, "Could not create user: " + std::to_string(code) , MIME(Text, Plain));
    } catch (const std::runtime_error &bang) {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void RESTEndpoint::updateUser(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    std::cout << "received updateUser" << std::endl;
    try {
        auto uid = request.param(":uid").as<int>();
        int code = updateIntoUsers(uid, request.body());
        if (code == 0) // UPDATE_OK
            response.send(Pistache::Http::Code::Ok, "Updated" , MIME(Text, Plain));
        else // error code
            response.send(Pistache::Http::Code::Bad_Request, "Could not update user: " + std::to_string(code) , MIME(Text, Plain));
    } catch (const std::runtime_error &bang) {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void RESTEndpoint::createThread(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    std::cout << "received createThread" << std::endl;
    try {
        int code = insertIntoThreads(request.body());
        std::cout << "got past insertIntoThreads" << std::endl;
        if (code == 0) // CREATE_OK
            response.send(Pistache::Http::Code::Created, "Created" , MIME(Text, Plain));
        else // error code
            response.send(Pistache::Http::Code::Bad_Request, "Could not create thread: " + std::to_string(code) , MIME(Text, Plain));
    } catch (const std::runtime_error &bang) {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void RESTEndpoint::readThread(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    std::cout << "received readThread" << std::endl;
    try {
        auto tid = request.param(":tid").as<std::string>();
        std::string thread = selectFromThreads(std::stoi(tid));
        if (thread == "")
            throw std::runtime_error("Thread not found");
        else
            response.send(Pistache::Http::Code::Ok, thread, MIME(Application, Json));
    } catch (const std::runtime_error &bang) {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void RESTEndpoint::readAllThreads(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    std::cout << "received readAllThreads" << std::endl;
    try {
        std::string thread = selectAllThreads();
        if (thread == "")
            throw std::runtime_error("Threads not found");
        else
            response.send(Pistache::Http::Code::Ok, thread, MIME(Application, Json));
    } catch (const std::runtime_error &bang) {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}
//void RESTEndpoint::doAuth(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {}

void RESTEndpoint::login(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    std::cout << "received login" << std::endl;
    try {
        std::string status = tryLogin(request.body());
        if (status != "") // UPDATE_OK
            response.send(Pistache::Http::Code::Ok, status , MIME(Text, Plain));
        else // error code
            response.send(Pistache::Http::Code::Unauthorized, status , MIME(Text, Plain));
    } catch (const std::runtime_error &bang) {
        response.send(Pistache::Http::Code::Not_Found, std::string("Runtime: ").append(bang.what()), MIME(Text, Plain));
    } catch (...) {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void RESTEndpoint::updateThread(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    std::cout << "received updateThread" << std::endl;
    try {
        auto tid = request.param(":tid").as<int>();
        int code = updateIntoThreads(tid, request.body());
        if (code == 0) // UPDATE_OK
            response.send(Pistache::Http::Code::Ok, "Updated" , MIME(Text, Plain));
        else // error code
            response.send(Pistache::Http::Code::Bad_Request, "Could not update thread: " + std::to_string(code) , MIME(Text, Plain));
    } catch (const std::runtime_error &bang) {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}

void RESTEndpoint::deleteThread(Pistache::Rest::Request const& request, Pistache::Http::ResponseWriter response) {
    std::cout << "received deleteThread" << std::endl;
    try {
        auto tid = request.param(":uid").as<std::string>();
        int code = deleteFromThreads(std::stoi(tid));
        if (code == 0) // DELETE_OK
            response.send(Pistache::Http::Code::Ok, "Deleted " + tid, MIME(Text, Plain));
        else // error code
            response.send(Pistache::Http::Code::Bad_Request, "Could not delete thread: " + std::to_string(code), MIME(Text, Plain));
    } catch (const std::runtime_error &bang) {
        response.send(Pistache::Http::Code::Not_Found, bang.what(), MIME(Text, Plain));
    } catch (...) {
        response.send(Pistache::Http::Code::Internal_Server_Error, "Internal error", MIME(Text, Plain));
    }
}