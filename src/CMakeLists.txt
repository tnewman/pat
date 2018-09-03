add_library(pat SHARED src/pat.c)
target_include_directories(pat PUBLIC include PRIVATE src)
target_include_directories(pat PRIVATE ${FFMPEG_INCLUDE_DIR} ${SDL2_INCLUDE_DIR})
target_link_libraries(pat ${FFMPEG_LIBRARIES} ${SDL2_LIBRARY})

message(INFO ${FFMPEG_INCLUDE_DIR})

add_executable(pattest src/pat_test.c)
target_link_libraries(pattest pat)
