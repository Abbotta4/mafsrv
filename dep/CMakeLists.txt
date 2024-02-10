include(FetchContent)

FetchContent_Declare(sqlpp11
    GIT_REPOSITORY  https://github.com/rbock/sqlpp11
    GIT_TAG         7f04435576036fc3f06d929760b5c7623485f069 # v0.64
)

add_subdirectory(sqlpp11)
