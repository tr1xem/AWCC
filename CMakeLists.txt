cmake_minimum_required(VERSION 3.16)

# Project name and language
project(awcc LANGUAGES CXX)
set(APP_NAME main)

set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(LoguruGitRepo
	GIT_REPOSITORY "https://github.com/emilk/loguru"
	GIT_TAG        "master"
)
# set any loguru compile-time flags before calling MakeAvailable()
set(LOGURU_WITH_STREAMS TRUE)
FetchContent_MakeAvailable(LoguruGitRepo)


FetchContent_Declare(
  json
  GIT_REPOSITORY "https://github.com/nlohmann/json.git"
  GIT_TAG "master"
)
FetchContent_MakeAvailable(json)



find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVDEV REQUIRED libevdev)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(${APP_NAME} ${SOURCES})
target_include_directories(${APP_NAME} PRIVATE include)
target_link_libraries(${APP_NAME} PRIVATE loguru::loguru nlohmann_json::nlohmann_json)
target_include_directories(${APP_NAME} PRIVATE ${LIBUSB_INCLUDE_DIRS})
target_link_libraries(${APP_NAME} PRIVATE ${LIBUSB_LIBRARIES})
target_compile_options(${APP_NAME} PRIVATE ${LIBUSB_CFLAGS_OTHER})
target_include_directories(${APP_NAME} PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(${APP_NAME} PRIVATE ${LIBEVDEV_LIBRARIES})
target_compile_options(${APP_NAME} PRIVATE ${LIBEVDEV_CFLAGS_OTHER})
