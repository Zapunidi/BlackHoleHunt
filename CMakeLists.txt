cmake_minimum_required(VERSION 3.0.0)
project(black_hole_hunt VERSION 0.8.0)

include_directories(include)
link_directories(lib)

add_executable(black_hole_hunt main.cpp menu.cpp credits.cpp game.cpp)
target_link_libraries(black_hole_hunt libraylib.a ${CMAKE_DL_LIBS})

# Options to include pthreads. Use both because just the first one do not work
add_compile_options(-pthread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
