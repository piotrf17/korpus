cmake_minimum_required(VERSION 2.6)
project(KORPUS)

# gtest
if(BUILD_TESTING)
  add_subdirectory("/usr/src/gtest/" "${CMAKE_CURRENT_BINARY_DIR}/gtest")
endif(BUILD_TESTING)

# Protobuf library
include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

set(LIBS 
    pugixml
    ${PROTOBUF_LIBRARY})
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -g -Wall")

# build xml library
add_subdirectory(pugixml)

# korpus library
set(SRCS
    corpus.cc
    document.cc
    index.cc
    lexeme.pb.cc
    lexeme.cc
    result.cc)
add_library(korpus ${SRCS})  

# korpus front end
set(SRCS
    korpus.cc)
add_executable(korpusfe ${SRCS})
target_link_libraries(korpusfe korpus ${LIBS})

# tests
include(CTest)
if(BUILD_TESTING)
  enable_testing()

  add_executable(document_test document_test.cc)
  target_link_libraries(document_test korpus pthread gtest_main ${LIBS})
  add_test(DocumentTest ${PROJECT_BINARY_DIR}/document_test)

  add_executable(lexeme_test lexeme_test.cc)
  target_link_libraries(lexeme_test korpus pthread gtest_main ${LIBS})
  add_test(LexemeTest ${PROJECT_BINARY_DIR}/lexeme_test)

endif(BUILD_TESTING)

# questioner web app
add_subdirectory(questioner)