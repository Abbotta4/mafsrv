include(FetchContent)

FetchContent_Declare(sqlpp11
  GIT_REPOSITORY  https://github.com/rbock/sqlpp11
  GIT_TAG         0.64
)

FetchContent_Declare(libpistache
  GIT_REPOSITORY  https://github.com/pistacheio/pistache
  GIT_TAG         origin/master
)

add_subdirectory(sqlpp11)
add_subdirectory(pistache)
