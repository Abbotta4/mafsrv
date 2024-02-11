add_executable(mafsrv)
if(BUILD_MYSQL_CONNECTOR)
  target_link_libraries(mafsrv PRIVATE sqlpp11::mysql)
endif()
if(BUILD_MARIADB_CONNECTOR)
  target_link_libraries(mafsrv PRIVATE sqlpp11::mariadb)
endif()
if(BUILD_POSTGRESQL_CONNECTOR)
  target_link_libraries(mafsrv PRIVATE sqlpp11::postgresql)
endif()
if(BUILD_SQLITE3_CONNECTOR)
  target_link_libraries(mafsrv PRIVATE sqlpp11::sqlite3)
endif()
if(BUILD_SQLCIPHER_CONNECTOR)
  target_link_libraries(mafsrv PRIVATE sqlpp11::sqlcipher)
endif()

target_sources(mafsrv
    PRIVATE 
        main.cpp 
)
