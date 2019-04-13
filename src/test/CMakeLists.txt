include_directories(
  /home/scding/code/googletest/googletest/include/
 )

 link_directories(
  /home/scding/code/googletest/googletest/mybuild/lib
 )

 add_executable(test main.cpp)
 target_link_libraries(test libgtest.so)