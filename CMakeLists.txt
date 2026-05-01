cmake_minimum_required(VERSION 3.16)
project(awcc LANGUAGES CXX)
set(APP_NAME awcc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Release)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Build type" FORCE)
endif()
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      "/usr"
      CACHE PATH "Install path prefix" FORCE)
endif()

include(FetchContent)
find_package(PkgConfig REQUIRED)

# ----------- Loguru ------------
FetchContent_Declare(
  LoguruGitRepo
  GIT_REPOSITORY "https://github.com/emilk/loguru"
  GIT_TAG "master"
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE)
set(LOGURU_WITH_STREAMS TRUE)
FetchContent_MakeAvailable(LoguruGitRepo)
set(LOGURU_LIBRARIES loguru::loguru)
set(LOGURU_INCLUDE_DIRS ${LoguruGitRepo_SOURCE_DIR})

# ----------- nlohmann_json ------------
FetchContent_Declare(
  json
  GIT_REPOSITORY "https://github.com/nlohmann/json.git"
  GIT_TAG "master"
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(json)
set(NLOHMANN_JSON_LIBRARIES nlohmann_json::nlohmann_json)
set(NLOHMANN_JSON_INCLUDE_DIRS ${json_SOURCE_DIR}/single_include)

# ----------- LIBUSB ------------
FetchContent_Declare(
  libusb
  GIT_REPOSITORY https://github.com/libusb/libusb-cmake.git
  GIT_TAG main
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(libusb)

# ----------- EVDEV ------------
pkg_check_modules(LIBEVDEV REQUIRED libevdev)

# -----------ALINEFX ------------
FetchContent_Declare(
  alienfx
  GIT_REPOSITORY https://github.com/tr1xem/alienfx-linux.git
  GIT_TAG main
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(alienfx)
set(ALIENFX_INCLUDE_DIR ${alienfx_SOURCE_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${APP_NAME} ${SOURCES})
target_include_directories(
  ${APP_NAME} PRIVATE include ${ALIENFX_INCLUDE_DIR} ${LIBEVDEV_INCLUDE_DIRS}
                      ${LOGURU_INCLUDE_DIRS} ${NLOHMANN_JSON_INCLUDE_DIRS})
target_link_libraries(
  ${APP_NAME} PRIVATE ${LOGURU_LIBRARIES} ${NLOHMANN_JSON_LIBRARIES} usb-1.0
                      hidapi::libusb ${LIBEVDEV_LIBRARIES})

target_compile_options(${APP_NAME} PRIVATE ${LIBEVDEV_CFLAGS_OTHER})

install(TARGETS ${APP_NAME} RUNTIME DESTINATION /usr/bin)
