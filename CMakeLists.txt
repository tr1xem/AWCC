cmake_minimum_required(VERSION 3.16)
project(
  awcc
  LANGUAGES CXX
  VERSION 1.16.0)

set(APP_NAME awcc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Build type" FORCE)
endif()

add_compile_definitions(VERSION="${PROJECT_VERSION}")

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

include(FetchContent)
find_package(OpenGL REQUIRED)

if(UNIX AND NOT APPLE)
  find_package(X11 REQUIRED)
endif()

# ===================== Loguru =====================
FetchContent_Declare(
  loguru
  GIT_REPOSITORY https://github.com/emilk/loguru
  GIT_TAG master
  GIT_SHALLOW TRUE)
set(LOGURU_WITH_STREAMS
    TRUE
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(loguru)

# ===================== nlohmann_json =====================
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG master
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(json)

# ===================== GLFW =====================
set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS
    OFF
    CACHE BOOL "" FORCE)
set(GLFW_INSTALL
    OFF
    CACHE BOOL "" FORCE)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG master
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(glfw)

# ===================== ImGui =====================
FetchContent_Declare(
  imgui_src
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG master
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(imgui_src)

add_library(imgui STATIC)
target_sources(
  imgui
  PRIVATE ${imgui_src_SOURCE_DIR}/imgui.cpp
          ${imgui_src_SOURCE_DIR}/imgui_draw.cpp
          ${imgui_src_SOURCE_DIR}/imgui_tables.cpp
          ${imgui_src_SOURCE_DIR}/imgui_widgets.cpp
          ${imgui_src_SOURCE_DIR}/imgui_demo.cpp
          ${imgui_src_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
          ${imgui_src_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)

target_include_directories(imgui PUBLIC ${imgui_src_SOURCE_DIR}
                                        ${imgui_src_SOURCE_DIR}/backends)

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

if(UNIX AND NOT APPLE)
  target_link_libraries(imgui PUBLIC X11::X11 dl pthread)
endif()

# ===================== libusb =====================
set(LIBUSB_BUILD_SHARED
    OFF
    CACHE BOOL "" FORCE)
set(LIBUSB_BUILD_STATIC
    ON
    CACHE BOOL "" FORCE)

FetchContent_Declare(
  libusb
  GIT_REPOSITORY https://github.com/libusb/libusb-cmake.git
  GIT_TAG main
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(libusb)

# ===================== libevdev ============================================
find_package(PkgConfig REQUIRED)

include(FetchContent)

FetchContent_Declare(
  libevdev_src
  GIT_REPOSITORY https://gitlab.freedesktop.org/libevdev/libevdev.git
  GIT_TAG master
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(libevdev_src)

include(ExternalProject)

set(LIBEVDEV_INSTALL_DIR ${CMAKE_BINARY_DIR}/libevdev-install)

ExternalProject_Add(
  libevdev_ext
  SOURCE_DIR ${libevdev_src_SOURCE_DIR}
  BINARY_DIR ${CMAKE_BINARY_DIR}/libevdev-build
  CONFIGURE_COMMAND
    meson setup ${CMAKE_BINARY_DIR}/libevdev-build ${libevdev_src_SOURCE_DIR}
    --prefix=${LIBEVDEV_INSTALL_DIR} --default-library=static
    -Ddocumentation=false -Dtests=false -Dtools=false
  BUILD_COMMAND meson compile -C ${CMAKE_BINARY_DIR}/libevdev-build
  INSTALL_COMMAND meson install -C ${CMAKE_BINARY_DIR}/libevdev-build
  BUILD_BYPRODUCTS ${LIBEVDEV_INSTALL_DIR}/lib/libevdev.a)

add_library(libevdev::libevdev STATIC IMPORTED)

set_target_properties(
  libevdev::libevdev
  PROPERTIES IMPORTED_LOCATION ${LIBEVDEV_INSTALL_DIR}/lib/libevdev.a
             INTERFACE_INCLUDE_DIRECTORIES ${libevdev_src_SOURCE_DIR}/libevdev)

add_dependencies(libevdev::libevdev libevdev_ext)

# ===================== stb =====================
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(stb)

# ===================== App =====================
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${APP_NAME} ${SOURCES})

target_include_directories(${APP_NAME} PRIVATE include ${stb_SOURCE_DIR})

target_link_libraries(
  ${APP_NAME}
  PRIVATE glfw
          imgui
          OpenGL::GL
          loguru::loguru
          nlohmann_json::nlohmann_json
          usb-1.0
          libevdev::libevdev)

if(UNIX AND NOT APPLE)
  target_link_libraries(${APP_NAME} PRIVATE X11::X11 dl pthread)
endif()

# ===================== Install =====================
install(TARGETS ${APP_NAME} RUNTIME DESTINATION /usr/bin)

install(
  DIRECTORY app/
  DESTINATION /usr/share/applications
  FILES_MATCHING
  PATTERN "*.desktop")

install(
  DIRECTORY app/
  DESTINATION /usr/share/icons
  FILES_MATCHING
  PATTERN "*.png")

install(FILES app/70-awcc.rules DESTINATION /etc/udev/rules.d)

install(FILES app/awccd.service DESTINATION /etc/systemd/system)

install(FILES database.json DESTINATION /etc/awcc)
