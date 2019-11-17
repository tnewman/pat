cmake_minimum_required(VERSION 3.12)
project(_nodepat)

add_subdirectory(src/libpat)

add_library(_nodepat SHARED
  src/_nodepat.c
)

target_include_directories(_nodepat PRIVATE ${CMAKE_JS_INC})
set_target_properties(_nodepat PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(_nodepat pat ${CMAKE_JS_LIB})
