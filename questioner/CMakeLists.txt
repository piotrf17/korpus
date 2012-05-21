# Build mongoose HTTP server library
add_subdirectory(mongoose)

set(SRCS
    http_server.cc
    question_builder.cc
    question_server.cc
    questioner.cc)
add_executable(questioner ${SRCS})
target_link_libraries(questioner mongoose pthread dl korpus pugixml ${PROTOBUF_LIBRARY})
