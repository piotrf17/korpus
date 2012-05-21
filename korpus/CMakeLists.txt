# build proto files
include_directories(${PROJECT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS lexeme.proto)

# korpus library
set(SRCS
    corpus.cc
    document.cc
    index.cc
    lexeme.cc
    result.cc
    ${PROTO_SRCS}
    ${PROTO_HDRS})
add_library(korpus ${SRCS})

# tests
if(BUILD_TESTING)
  add_executable(document_test document_test.cc)
  target_link_libraries(document_test gtest_main ${TEST_LIBS})
  add_test(DocumentTest ${PROJECT_BINARY_DIR}/document_test)

  add_executable(lexeme_test lexeme_test.cc)
  target_link_libraries(lexeme_test gtest_main ${TEST_LIBS})
  add_test(LexemeTest ${PROJECT_BINARY_DIR}/lexeme_test)
endif(BUILD_TESTING)