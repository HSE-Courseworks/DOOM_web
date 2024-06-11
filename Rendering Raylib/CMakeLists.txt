cmake_minimum_required(VERSION 3.10...3.27) # FetchContent is available in 3.11+
project(DOOM_web)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

# Generate compile_commands.json
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#SFML constants
set(SFML_BUILD_EXAMPLES False)
set(SFML_BUILD_DOC False)
set(SFML_BUILD_AUDIO False)
set(SFML_BUILD_GRAPHICS False)
set(SFML_BUILD_WINDOW False)
set(SFML_BUILD_NETWORK True)
set(BUILD_SHARED_LIBS False)
set(SFML_STATIC_LIBRARIES TRUE)

# Dependencies
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
find_package(SFML COMPONENTS network REQUIRED)

# Our Project
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src "C:/Program Files/SFML-2.6.1/include")
file(GLOB all_SRCS "${PROJECT_SOURCE_DIR}/include/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(main ${all_SRCS})
target_link_libraries(main raylib sfml-network)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an main.html file.
    set_target_properties(main PROPERTIES SUFFIX ".html")
endif()