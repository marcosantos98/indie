cmake_minimum_required(VERSION 3.20)

include(FetchContent)

project(untitled C)

add_executable(${PROJECT_NAME} main.c)

if(UNIX)
    set(RAYLIB_PATH "./raylib/src")
    set(RAYLIB_INCLUDE_PATH "./raylib/src")
    set(RAYLIB_LIBS_DIR "./raylib/src")
    set(TO_LINK m raylib lua54)
else()
    set(RAYLIB_PATH "./raylib/build/raylib")
    set(RAYLIB_INCLUDE_PATH "./raylib/src")
    set(RAYLIB_LIBS_DIR "./raylib/src")
    set(TO_LINK raylib lua54 opengl32 winmm gdi32)
endif()

# lua
FetchContent_declare(
    lua
    URL https://www.lua.org/ftp/lua-5.4.6.tar.gz
)

FetchContent_MakeAvailable(lua)

file(COPY_FILE "./Makefile" "${lua_SOURCE_DIR}/src/Makefile")

if(NOT EXISTS "${lua_SOURCE_DIR}/src/lua54.dll")
    execute_process(COMMAND "mingw32-make"
                    WORKING_DIRECTORY "${lua_SOURCE_DIR}/src"
    )
endif()

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

set(LUA_PATH "${lua_SOURCE_DIR}/src")
set(STB_PATH "./stb")
set(ARENA_PATH "./arena")

target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_PATH} ${LUA_PATH} ${STB_PATH} ${ARENA_PATH})
target_link_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_LIBS_DIR} ${LUA_PATH})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TO_LINK})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy 
	"${LUA_PATH}/lua54.dll"
	"${CMAKE_BINARY_DIR}/lua54.dll")
