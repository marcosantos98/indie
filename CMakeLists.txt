cmake_minimum_required(VERSION 3.20)

include(FetchContent)

project(untitled C)

add_executable(${PROJECT_NAME} main.c)

find_package(Lua REQUIRED)

set(TO_LINK raylib ${LUA_LIBRARIES})

# raylib
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
	GIT_REPOSITORY https://github.com/raysan5/raylib.git
	GIT_TAG 5.0
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

set(LUA_PATH "${LUA_INCLUDE_DIR}")
set(STB_PATH "./stb")
set(ARENA_PATH "./arena")

target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_PATH} ${LUA_PATH} ${STB_PATH} ${ARENA_PATH})
target_link_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_LIBS_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TO_LINK})
