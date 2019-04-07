cmake_minimum_required(VERSION 2.6)

project(test C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/tools)

set(CXX_FLAGS
 -g
 # -DVALGRIND
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 # -DVALGRIND
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++11
 -rdynamic
 )

 include_directories(
   ./
  src/
  src/msgbag/
  src/pbmsg/
  src/play/
  src/record/
  src/log/
 )

 link_directories(
 )

 add_executable(record 
        src/record/record.cpp
        src/record/recorder.cpp
        # src/pbmsg/msgbag_msgs.pb.cc
        )
target_link_libraries(record protobuf)

# add_subdirectory(src/test)
