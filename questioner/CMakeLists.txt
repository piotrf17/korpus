# Build mongoose HTTP server library
add_subdirectory(mongoose)

# Build questioner web app
set(QUESTIONER_SRCS
    http_server.cc
    question_builder.cc
    question_server.cc
    questioner.cc)

add_executable(questioner ${QUESTIONER_SRCS})
target_link_libraries(questioner mongoose pthread dl ${APP_LIBS})
