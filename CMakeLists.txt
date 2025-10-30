cmake_minimum_required(VERSION 3.16)
project(awcc LANGUAGES CXX VERSION 1.12.0)
set(APP_NAME awcc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    add_custom_target(link_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
        BYPRODUCTS ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Updating symlink: compile_commands.json → project root"
    )
endif()
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Release)
add_compile_definitions(VERSION="${PROJECT_VERSION}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Install path prefix" FORCE)
endif()

include(FetchContent)
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
endif()

# ----------- Loguru ------------
pkg_check_modules(LOGURU loguru)
if(NOT LOGURU_FOUND)
    message(STATUS "loguru not found, fetching from GitHub...")
    FetchContent_Declare(LoguruGitRepo
        GIT_REPOSITORY "https://github.com/emilk/loguru"
        GIT_TAG        "master"
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    set(LOGURU_WITH_STREAMS TRUE)
    FetchContent_MakeAvailable(LoguruGitRepo)
    set(LOGURU_LIBRARIES loguru::loguru)
    set(LOGURU_INCLUDE_DIRS ${LoguruGitRepo_SOURCE_DIR})
else()
    message(STATUS "Found system loguru: ${LOGURU_LIBRARIES}")
    set(LOGURU_LIBRARIES ${LOGURU_LIBRARIES})
    set(LOGURU_INCLUDE_DIRS ${LOGURU_INCLUDE_DIRS})
endif()

# ----------- nlohmann_json ------------
pkg_check_modules(NLOHMANN_JSON nlohmann_json)
if(NOT NLOHMANN_JSON_FOUND)
    message(STATUS "nlohmann_json not found, fetching from GitHub...")
    FetchContent_Declare(json
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
        GIT_TAG "master"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(json)
    set(NLOHMANN_JSON_LIBRARIES nlohmann_json::nlohmann_json)
    set(NLOHMANN_JSON_INCLUDE_DIRS ${json_SOURCE_DIR}/single_include)
else()
    message(STATUS "Found system nlohmann_json: ${NLOHMANN_JSON_LIBRARIES}")
    set(NLOHMANN_JSON_LIBRARIES ${NLOHMANN_JSON_LIBRARIES})
    set(NLOHMANN_JSON_INCLUDE_DIRS ${NLOHMANN_JSON_INCLUDE_DIRS})
endif()

# ----------- ImGui ------------
pkg_check_modules(IMGUI imgui)
if(NOT IMGUI_FOUND)
    message(STATUS "imgui not found, fetching from GitHub...")
    FetchContent_Declare(imgui
        GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
        GIT_TAG "master"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(imgui)
    set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui SYSTEM PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )
    target_link_libraries(imgui PUBLIC OpenGL::GL)
    if(UNIX AND NOT APPLE)
        target_link_libraries(imgui PUBLIC ${X11_LIBRARIES} dl pthread)
    endif()
    set(IMGUI_LIBRARIES imgui)
    set(IMGUI_INCLUDE_DIRS ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
else()
    message(STATUS "Found system imgui: ${IMGUI_LIBRARIES}")
    set(IMGUI_LIBRARIES ${IMGUI_LIBRARIES})
    set(IMGUI_INCLUDE_DIRS ${IMGUI_INCLUDE_DIRS})
endif()

# ----------- GLFW ------------
pkg_search_module(GLFW REQUIRED glfw3 glfw)

# ----------- LIBUSB ------------
pkg_check_modules(LIBUSB libusb-1.0)
if(NOT LIBUSB_FOUND)
    message(STATUS "libusb-1.0 not found, fetching from GitHub...")
    FetchContent_Declare(libusb
        GIT_REPOSITORY "https://github.com/libusb/libusb.git"
        GIT_TAG "master"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(libusb)
    set(LIBUSB_LIBRARIES libusb-1.0)
    set(LIBUSB_INCLUDE_DIRS ${libusb_SOURCE_DIR}/libusb)
    set(LIBUSB_CFLAGS_OTHER "")
else()
    message(STATUS "Found system libusb-1.0: ${LIBUSB_LIBRARIES}")
    set(LIBUSB_LIBRARIES ${LIBUSB_LIBRARIES})
    set(LIBUSB_INCLUDE_DIRS ${LIBUSB_INCLUDE_DIRS})
    set(LIBUSB_CFLAGS_OTHER ${LIBUSB_CFLAGS_OTHER})
endif()

# ----------- EVDEV ------------
pkg_check_modules(LIBEVDEV REQUIRED libevdev)

# ----------- STB IMAGE (no system search, always fetch) ------------
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(stb)
set(STB_IMAGE_INCLUDE_DIR ${stb_SOURCE_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${APP_NAME} ${SOURCES})
target_include_directories(${APP_NAME} PRIVATE
    include
    ${STB_IMAGE_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
    ${LIBUSB_INCLUDE_DIRS}
    ${LIBEVDEV_INCLUDE_DIRS}
    ${LOGURU_INCLUDE_DIRS}
    ${NLOHMANN_JSON_INCLUDE_DIRS}
    ${IMGUI_INCLUDE_DIRS}
)
target_link_libraries(${APP_NAME}
    PRIVATE
        ${LOGURU_LIBRARIES}
        ${NLOHMANN_JSON_LIBRARIES}
        ${IMGUI_LIBRARIES}
        ${GLFW_LIBRARIES}
        OpenGL::GL
        ${LIBUSB_LIBRARIES}
        ${LIBEVDEV_LIBRARIES}
)
if(UNIX AND NOT APPLE)
    target_link_libraries(${APP_NAME} PRIVATE ${X11_LIBRARIES} dl pthread)
endif()
target_compile_options(${APP_NAME} PRIVATE
    ${GLFW_CFLAGS_OTHER}
    ${LIBUSB_CFLAGS_OTHER}
    ${LIBEVDEV_CFLAGS_OTHER}
)

install(TARGETS ${APP_NAME}
    RUNTIME DESTINATION /usr/bin
)

install(DIRECTORY app/
    DESTINATION /usr/share/applications
    FILES_MATCHING PATTERN "*.desktop"
)

# Install icons (recursively)
install(DIRECTORY app/
    DESTINATION /usr/share/icons
    FILES_MATCHING PATTERN "*.png"
)

install(FILES app/70-awcc.rules
    DESTINATION /etc/udev/rules.d
)

# Install systemd service file
install(FILES app/awccd.service
    DESTINATION /etc/systemd/system
)


# Install systemd service file
install(FILES database.json
    DESTINATION /etc/awcc
)

