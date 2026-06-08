# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Chess960_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Chess960_autogen.dir\\ParseCache.txt"
  "Chess960_autogen"
  )
endif()
