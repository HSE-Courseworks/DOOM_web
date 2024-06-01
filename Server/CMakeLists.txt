cmake_minimum_required(VERSION 3.10)
project(DOOM_WEB_SERVER)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

find_package(raylib REQUIRED)
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

add_library(Data src/Data.cpp)
target_include_directories(Data
    PUBLIC "include")

add_library(LobbyNode src/LobbyNode.cpp)
target_include_directories(LobbyNode 
    PUBLIC "include" )
target_link_libraries(LobbyNode ws2_32.lib)

add_library(Server src/Server.cpp)
target_include_directories(Server 
    PUBLIC "include")
target_link_libraries(Server LobbyNode Data raylib ws2_32.lib)

add_executable(DOOM_WEB_SERVER src/main.cpp)
target_include_directories(DOOM_WEB_SERVER 
    PUBLIC "include")
target_link_libraries(DOOM_WEB_SERVER Server)
