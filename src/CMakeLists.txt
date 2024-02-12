add_executable(mafsrv)

set(count 0)
if(BUILD_MYSQL_CONNECTOR)
  MATH(EXPR count "${count}+1")
  target_link_libraries(mafsrv PRIVATE sqlpp11::mysql)
endif()
if(BUILD_MARIADB_CONNECTOR)
  MATH(EXPR count "${count}+1")
  target_link_libraries(mafsrv PRIVATE sqlpp11::mariadb)
endif()
if(BUILD_POSTGRESQL_CONNECTOR)
  MATH(EXPR count "${count}+1")
  target_link_libraries(mafsrv PRIVATE sqlpp11::postgresql)
endif()
if(BUILD_SQLITE3_CONNECTOR)
  MATH(EXPR count "${count}+1")
  target_link_libraries(mafsrv PRIVATE sqlpp11::sqlite3)
endif()
if(BUILD_SQLCIPHER_CONNECTOR)
  MATH(EXPR count "${count}+1")
  target_link_libraries(mafsrv PRIVATE sqlpp11::sqlcipher)
endif()

if(count GREATER 1)
  message(FATAL_ERROR "You must choose only one connector")
endif()

target_sources(mafsrv
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/users.h
)
