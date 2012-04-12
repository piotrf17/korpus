cmake_minimum_required(VERSION 2.6)
project(KORPUS)

set(LIBS pugixml)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -g -Wall")

# xml library
add_subdirectory(pugixml)

# korpus library
set(SRCS
    corpus.cc
    document.cc
    index.cc
    lexeme.cc
    result.cc)
add_library(korpus ${SRCS})  

# main executable
set(SRCS
    korpus.cc)
add_executable(korpusfe ${SRCS})
target_link_libraries(korpusfe korpus ${LIBS})

# tests
include(CTest)
if(BUILD_TESTING)
  enable_testing()
  add_executable(document_test document_test.cc)
  target_link_libraries(document_test korpus pugixml pthread gtest_main)
  add_test(DocumentTest ${PROJECT_BINARY_DIR}/document_test)
endif(BUILD_TESTING)