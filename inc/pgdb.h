#pragma once

#include <sqlpp11/postgresql/connection.h>

sqlpp::postgresql::connection getDB();
std::string selectFromUsers(int uid);
int insertIntoThreads(std::string body);
std::string selectAllThreads();
std::string selectFromThreads(int tid);
int updateIntoThreads(int tid, std::string body);
int deleteFromThreads(int tid);
int insertIntoUsers(std::string body);
int updateIntoUsers(int uid, std::string body);
int deleteFromUsers(int uid);
std::string tryLogin(std::string body);
