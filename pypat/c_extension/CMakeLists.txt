cmake_minimum_required(VERSION 3.9)
project(pat_python)

add_subdirectory(libpat)

find_package(PythonInterp 3.6 REQUIRED)
find_package(PythonLibs 3.6 REQUIRED)

add_library(_pat SHARED
  src/_pat.c
)
  
target_include_directories(_pat PUBLIC include PRIVATE src)
target_include_directories(_pat PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(_pat pat ${PYTHON_LIBRARIES})
set_target_properties(_pat PROPERTIES PREFIX "")
