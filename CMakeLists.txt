cmake_minimum_required(VERSION 3.20)

project(untitled C)

add_executable(${PROJECT_NAME} main.c)

set(RAYLIB_PATH "./raylib/src")
set(LUA_PATH "./lua")
set(STB_PATH "./stb")
set(ARENA_PATH "./arena")

target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_PATH} ${LUA_PATH} ${STB_PATH} ${ARENA_PATH})
target_link_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_PATH} ${LUA_PATH})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib m lua)
