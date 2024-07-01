cmake_minimum_required(VERSION 3.29)
project(DOOM_WEB_SERVER)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

set(SFML_BUILD_EXAMPLES False)
set(SFML_BUILD_DOC False)
set(SFML_BUILD_AUDIO False)
set(SFML_BUILD_GRAPHICS False)
set(SFML_BUILD_WINDOW False)
set(SFML_BUILD_NETWORK True)
set(BUILD_SHARED_LIBS False)
set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")

#list(APPEND CMAKE_MODULE_PATH "C:/Program Files/raylib/cmake")
#add_subdirectory("C:/Program Files/raylib/src" "C:/Program Files/raylib/src/build")
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
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
find_package(Threads REQUIRED)
find_package(SFML COMPONENTS network REQUIRED)
if (NOT SFML_FOUND)
    message(SFML NOT FOUND)
endif()

add_library(Tools src/Tools.cpp)
add_library(Lobby src/Lobby.cpp)
add_library(Server src/Server.cpp)
#add_library(Player src/Player.cpp)
add_library(PlayerInfo src/PlayerInfo.cpp)
file(GLOB Game_SRCS "${PROJECT_SOURCE_DIR}/src/client/*.cpp")
add_library(Game ${Game_SRCS})
add_library(World src/World.cpp)

add_executable(DOOM_WEB_SERVER src/main.cpp)

target_include_directories(DOOM_WEB_SERVER PUBLIC include "C:/Program Files/SFML-2.6.1/include")
target_include_directories(Tools PUBLIC include "C:/Program Files/SFML-2.6.1/include")
target_include_directories(Lobby PUBLIC include "C:/Program Files/raylib/src" "C:/Program Files/SFML-2.6.1/include")
target_include_directories(Server PUBLIC include "C:/Program Files/SFML-2.6.1/include")
#target_include_directories(Player PUBLIC include "C:/Program Files/SFML-2.6.1/include" "C:/Program Files/raylib/src")
target_include_directories(PlayerInfo PUBLIC include "C:/Program Files/SFML-2.6.1/include")
target_include_directories(Game PUBLIC "include/client" "C:/Program Files/raylib/src" "C:/Program Files/SFML-2.6.1/include")
target_include_directories(World PUBLIC include "C:/Program Files/raylib/src" "C:/Program Files/SFML-2.6.1/include")

target_link_libraries(DOOM_WEB_SERVER sfml-network Lobby Server Threads::Threads raylib)
target_link_libraries(Server Lobby Tools sfml-network Threads::Threads)
#target_link_libraries(Lobby Tools PlayerInfo sfml-network Threads::Threads)
target_link_libraries(Lobby Tools World PlayerInfo sfml-network Threads::Threads) #Player
target_link_libraries(Tools Lobby sfml-network Threads::Threads)
#target_link_libraries(PlayerInfo Lobby Player sfml-network Threads::Threads)
target_link_libraries(PlayerInfo Lobby sfml-network Threads::Threads)
target_link_libraries(Game raylib sfml-network)
#target_link_libraries(Player FromRender sfml-network)
target_link_libraries(World Game raylib sfml-network)
