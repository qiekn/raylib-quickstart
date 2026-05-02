cmake_minimum_required(VERSION 3.24...3.30)
project(game)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release MinSizeRel RelWithDebInfo)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------: dependencies

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libraries as shared" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Submodule sanity checks
foreach(_dep IN ITEMS
    "deps/raylib/CMakeLists.txt"
    "deps/imgui/imgui.h"
    "deps/json/single_include/nlohmann/json.hpp"
    "deps/raygui/src/raygui.h")
  if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_dep}")
    message(FATAL_ERROR
      "Submodule missing: ${_dep}. Run: git submodule update --init --recursive")
  endif()
endforeach()

add_subdirectory(deps/raylib)

# -----------------------------------------------------------------------------: project

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HDRS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")

add_compile_definitions(ASSETS="${CMAKE_SOURCE_DIR}/assets")

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE
  include
  deps/raygui/src
  deps/json/single_include
)

target_link_libraries(${PROJECT_NAME}
    raylib
)

# -----------------------------------------------------------------------------: extra settings

# Checks if OSX and links appropriate frameworks (only required on macOS)
if (APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE
      "-framework IOKit"
      "-framework Cocoa"
      "-framework OpenGL"
  )
endif()

# Web
if ("${PLATFORM}" STREQUAL "Web")
  set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

  add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    # Emscripten '--preload-file assets' use this fold
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/../assets
  )
else()
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  )
endif()

# web configurations
if ("${PLATFORM}" STREQUAL "Web")
  # Tell Emscripten to build an example.html file.
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
  target_link_options(${PROJECT_NAME} PUBLIC
    -sUSE_GLFW=3
    -sEXPORTED_RUNTIME_METHODS=['HEAPF32']
    -sEXPORTED_RUNTIME_METHODS=['ccall']
    -sEXPORTED_RUNTIME_METHODS=['requestFullscreen']
    --shell-file ${CMAKE_SOURCE_DIR}/src/shell.html
    --preload-file assets)
endif()
