# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Chess960Tests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Chess960Tests_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\Chess960_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Chess960_autogen.dir\\ParseCache.txt"
  "Chess960Tests_autogen"
  "Chess960_autogen"
  "_deps\\googletest-build\\googlemock\\CMakeFiles\\gmock_autogen.dir\\AutogenUsed.txt"
  "_deps\\googletest-build\\googlemock\\CMakeFiles\\gmock_autogen.dir\\ParseCache.txt"
  "_deps\\googletest-build\\googlemock\\CMakeFiles\\gmock_main_autogen.dir\\AutogenUsed.txt"
  "_deps\\googletest-build\\googlemock\\CMakeFiles\\gmock_main_autogen.dir\\ParseCache.txt"
  "_deps\\googletest-build\\googlemock\\gmock_autogen"
  "_deps\\googletest-build\\googlemock\\gmock_main_autogen"
  "_deps\\googletest-build\\googletest\\CMakeFiles\\gtest_autogen.dir\\AutogenUsed.txt"
  "_deps\\googletest-build\\googletest\\CMakeFiles\\gtest_autogen.dir\\ParseCache.txt"
  "_deps\\googletest-build\\googletest\\CMakeFiles\\gtest_main_autogen.dir\\AutogenUsed.txt"
  "_deps\\googletest-build\\googletest\\CMakeFiles\\gtest_main_autogen.dir\\ParseCache.txt"
  "_deps\\googletest-build\\googletest\\gtest_autogen"
  "_deps\\googletest-build\\googletest\\gtest_main_autogen"
  )
endif()
