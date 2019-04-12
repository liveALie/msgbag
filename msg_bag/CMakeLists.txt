SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/tool_box/msg_bag)
message("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

include_directories(
  ${INC_THIRD_PARTY_DIR}
  src/
  src/msgbag/
  src/pbmsg/
  src/play/
  src/record/
  src/log/
  src/conf/
  src/storage/
  ${PROJECT_SOURCE_DIR}/src/common/timer/
 )

 link_directories(
     ${LIB_THIRD_PARTY_DIR}
 )

add_executable(record
    src/record/record.cpp
    src/conf/msgbag_conf.cpp
    src/record/recorder.cpp
    src/common/util.cpp
    src/common/thread_pool.cpp
    src/storage/bag.cpp
    src/common/buffer.cpp
    ${PROTOCOL_SRC})
IF($ENV{CROSS_COMPILING_TYPE} MATCHES "qnx")
    TARGET_LINK_LIBRARIES(record libshmmq.a libprotobuf.a -lsocket)
ELSE()
    TARGET_LINK_LIBRARIES(record libshmmq.a libprotobuf.a -ldl -lpthread)
ENDIF()

add_executable(play
    src/play/play.cpp
    src/conf/msgbag_conf.cpp
    src/common/pub_sub.cpp
    src/play/player.cpp
    src/storage/bag.cpp
    src/common/buffer.cpp
    src/common/util.cpp
    ${PROTOCOL_SRC}
    )
IF($ENV{CROSS_COMPILING_TYPE} MATCHES "qnx")
    TARGET_LINK_LIBRARIES(play libshmmq.a libprotobuf.a -lsocket)
ELSE()
    TARGET_LINK_LIBRARIES(play libshmmq.a libprotobuf.a -ldl -lpthread)
ENDIF()