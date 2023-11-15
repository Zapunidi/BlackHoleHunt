cmake_minimum_required(VERSION 3.0.0)
project(black_hole_hunt VERSION 0.8.0)

include_directories(include)
link_directories(lib)

add_executable(black_hole_hunt main.cpp)
target_link_libraries(black_hole_hunt libraylib.a)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
