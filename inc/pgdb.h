#pragma once

#include <sqlpp11/postgresql/connection.h>

sqlpp::postgresql::connection getDB();
std::string selectFromUsers(int uid);
int insertIntoUsers(std::string body);
int updateIntoUsers(int uid, std::string body);
int deleteFromUsers(int uid);
std::string tryLogin(std::string body);
