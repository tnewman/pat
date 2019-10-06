cmake_minimum_required(VERSION 3.9)
project(_pypat)

set(_PYPAT_OUTPUT_NAME "" CACHE STRING "_pypat library output name")

add_subdirectory(libpat)

find_package(PythonInterp 3.6 REQUIRED)
find_package(PythonLibs 3.6 REQUIRED)

add_library(_pypat SHARED
  _pypat.c
)
  
target_include_directories(_pypat PUBLIC include PRIVATE src)
target_include_directories(_pypat PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(_pypat pat ${PYTHON_LIBRARIES})

IF(NOT "${_PYPAT_OUTPUT_NAME}" STREQUAL "")
  set_target_properties(_pypat PROPERTIES PREFIX "")
  set_target_properties(_pypat PROPERTIES OUTPUT_NAME "${_PYPAT_OUTPUT_NAME}")
  set_target_properties(_pypat PROPERTIES SUFFIX "")
ENDIF()