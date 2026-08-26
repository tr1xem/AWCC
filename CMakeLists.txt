cmake_minimum_required(VERSION 3.16)
project(
  awcc
  LANGUAGES CXX
  VERSION 2.00.0)

set(APP_NAME awcc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(VERSION="${PROJECT_VERSION}")
set(CMAKE_BUILD_TYPE Debug)
set(ALIENFX_BUILD_CLI
    OFF
    CACHE BOOL "" FORCE)
set(ALIENFX_BUILD_EXAMPLE
    OFF
    CACHE BOOL "" FORCE)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      "/usr"
      CACHE PATH "Install path prefix" FORCE)
endif()

# compile_commands.json symlink
if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  add_custom_target(
    link_compile_commands ALL
    COMMAND
      ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_SOURCE_DIR}/compile_commands.json
    BYPRODUCTS ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()

add_subdirectory(external/alienfx-linux ${CMAKE_BINARY_DIR}/alienfx)
# ===================== Loguru =====================
# TODO: Use loguru header directly

# FetchContent_Declare( loguru GIT_REPOSITORY https://github.com/emilk/loguru
# GIT_TAG master GIT_SHALLOW TRUE) set(LOGURU_WITH_STREAMS TRUE CACHE BOOL ""
# FORCE) FetchContent_MakeAvailable(loguru)

# ===================== nlohmann_json =====================
# FetchContent_Declare( json GIT_REPOSITORY https://github.com/nlohmann/json.git
# GIT_TAG master GIT_SHALLOW TRUE) FetchContent_MakeAvailable(json)

# ===================== App =====================
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${APP_NAME} ${SOURCES})

target_include_directories(${APP_NAME} PRIVATE include)
target_link_libraries(awcc PRIVATE AlienFX_SDK AlienFan_SDK)
