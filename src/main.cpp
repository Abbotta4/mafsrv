#include <iostream>
#include <sqlpp11/postgresql/connection.h>
#include <sqlpp11/select.h>
#include "users.h"

int main() {
    mafsrv::PublicUsers users;
    auto config = std::make_shared<sqlpp::postgresql::connection_config>();
    config->host = "localhost";
    config->user = "postgres";
    config->password = "";
    config->dbname = "postgres";
    sqlpp::postgresql::connection db(config);

    // selecting zero or more results, iterating over the results
    for (const auto& row : db(select(users.uid, users.name).from(users).unconditionally())) {
        std::string name = row.name;
        int uid = row.uid;
        std::cout << "uid: " << uid << " name: " << name << std::endl;
    }

    return 0;
}
