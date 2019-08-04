cmake_minimum_required(VERSION 3.9)
project(_pypat)

add_subdirectory(libpat)

find_package(PythonInterp 3.6 REQUIRED)
find_package(PythonLibs 3.6 REQUIRED)

add_library(_pypat SHARED
  src/_pypat.c
)
  
target_include_directories(_pypat PUBLIC include PRIVATE src)
target_include_directories(_pypat PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(_pypat pat ${PYTHON_LIBRARIES})
set_target_properties(_pypat PROPERTIES PREFIX "")
