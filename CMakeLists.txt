cmake_minimum_required(VERSION 2.6)
project(KORPUS)

set(LIBS pugixml)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -g -Wall")

add_subdirectory(pugixml)

set(SRCS
    corpus.cc
    document.cc
    index.cc
    lexeme.cc
    korpus.cc)

add_executable(korpus ${SRCS})

target_link_libraries(korpus ${LIBS})
