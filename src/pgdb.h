#pragma once

#include <sqlpp11/postgresql/connection.h>

sqlpp::postgresql::connection getDB();
std::string queryUsers(int uid);
int insertUsers(std::string body);
