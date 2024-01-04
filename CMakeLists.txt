cmake_minimum_required(VERSION 3.0.0)
project(black_hole_hunt VERSION 0.8.0)

include_directories(include)
link_directories(lib)

add_executable(black_hole_hunt main.c menu.c credits.c game.c)

find_library(raylib NAMES libraylib.a raylib.lib PATHS ${PROJECT_SOURCE_DIR}/lib NO_DEFAULT_PATH)
message(STATUS "raylib=${raylib}")
target_link_libraries(black_hole_hunt ${raylib} ${CMAKE_DL_LIBS})

# Options to include pthreads. Use both because just the first one do not work
if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
	target_link_libraries(black_hole_hunt winmm.lib)
else()
	add_compile_options(-pthread)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
