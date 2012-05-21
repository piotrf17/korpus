cmake_minimum_required(VERSION 2.6)
project(KORPUS)

# all header files are specified relative to base
include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -g -Wall")

##############################################
#  Configure required libraries

# gtest
if(BUILD_TESTING)
  add_subdirectory("/usr/src/gtest/" "${CMAKE_BINARY_DIR}/gtest")
endif(BUILD_TESTING)
include(CTest)

# Protobuf library
include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

# Libraries that applications must link against
set(APP_LIBS 
    korpus
    pugixml
    ${PROTOBUF_LIBRARIES})

set(TEST_LIBS
    ${APP_LIBS}
    pthread)

#############################################
#  Build targets

# xml helper library
add_subdirectory(pugixml)

# main korpus library
add_subdirectory(korpus)

# questioner web app
add_subdirectory(questioner)

# tool to build a corpus
add_executable(build_corpus build_corpus.cc)
target_link_libraries(build_corpus ${APP_LIBS})

if(BUILD_TESTING)
  enable_testing()
endif(BUILD_TESTING)

