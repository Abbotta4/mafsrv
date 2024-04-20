#pragma once

#include <sqlpp11/postgresql/connection.h>

sqlpp::postgresql::connection getDB();
std::string getFromUsers(int uid);
int insertUsers(std::string body);
int deleteFromUsers(int uid);
