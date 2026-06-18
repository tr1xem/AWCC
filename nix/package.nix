{ lib, stdenv, cmake, ninja, pkg-config
, libusb1, libevdev, udev
, libglvnd, libx11, libxrandr, libxinerama, libxcursor, libxi
, libxkbcommon, wayland, wayland-protocols
, waylandScanner
, loguru, nlohmann_json, glfw, stb
, fetchFromGitHub
}:

let
  imgui-src = fetchFromGitHub {
    owner = "ocornut";
    repo = "imgui";
    rev = "v1.91.8";
    hash = "sha256-+BuSAXvLvOYOmENzxd1pGDE6llWhTGVu7C3RnoVLVzg=";
  };
in stdenv.mkDerivation {
  pname = "awcc";
  version = "1.19.0";

  src = lib.cleanSource ./..;

  nativeBuildInputs = [ cmake ninja pkg-config waylandScanner wayland-protocols ];

  buildInputs = [
    libusb1 libevdev udev
    libglvnd libx11 libxrandr libxinerama libxcursor libxi
    libxkbcommon wayland
    loguru nlohmann_json glfw stb
  ];

  cmakeFlags = [
    "-DFETCHCONTENT_SOURCE_DIR_IMGUI=${imgui-src}"
    "-DUDEV_RULES_DIR=${placeholder "out"}/lib/udev/rules.d"
    "-DAWCC_CONFIG_DIR=${placeholder "out"}/share/awcc"
    "-DSYSTEMD_UNIT_DIR=${placeholder "out"}/etc/systemd/system"
  ];

  NIX_CFLAGS_COMPILE = [ "-I${stb}/include/stb" ];

  preConfigure =
    let
      libusbBlock = ''
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
FetchContent_MakeAvailable(libusb)'';
      libusbReplace = ''
# ===================== libusb =====================
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0 IMPORTED_TARGET)
add_library(usb-1.0 INTERFACE IMPORTED)
target_link_libraries(usb-1.0 INTERFACE PkgConfig::LIBUSB)'';
      libevdevBlock = ''
# ===================== libevdev ============================================
include(ExternalProject)

ExternalProject_Add(
  libevdev
  GIT_REPOSITORY https://gitlab.freedesktop.org/libevdev/libevdev.git
  GIT_TAG master
  CONFIGURE_COMMAND
    meson setup <BINARY_DIR> <SOURCE_DIR> --default-library=static
    -Ddocumentation=disabled -Dtests=disabled -Dtools=disabled
  BUILD_COMMAND meson compile -C <BINARY_DIR>
  INSTALL_COMMAND "" # CMAKE NEEDS A INSTALL COMMAND or it will run make
  BUILD_BYPRODUCTS <BINARY_DIR>/libevdev.a)

add_library(libevdev::libevdev STATIC IMPORTED)

ExternalProject_Get_Property(libevdev source_dir binary_dir)

set_target_properties(
  libevdev::libevdev PROPERTIES IMPORTED_LOCATION ''${binary_dir}/libevdev.a
                                INTERFACE_INCLUDE_DIRECTORIES ''${source_dir})

add_dependencies(libevdev::libevdev libevdev)'';
      libevdevReplace = ''
# ===================== libevdev ============================================
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVDEV REQUIRED libevdev>=1.0 IMPORTED_TARGET)
add_library(libevdev::libevdev INTERFACE IMPORTED)
target_link_libraries(libevdev::libevdev INTERFACE PkgConfig::LIBEVDEV)'';
      loguruBlock = ''
FetchContent_Declare(
  loguru
  GIT_REPOSITORY https://github.com/emilk/loguru
  GIT_TAG master
  GIT_SHALLOW TRUE)
set(LOGURU_WITH_STREAMS
    TRUE
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(loguru)'';
      jsonBlock = ''
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG master
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(json)'';
      glfwBlock = ''
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
FetchContent_MakeAvailable(glfw)'';
      stbBlock = ''
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(stb)'';
    in ''
    # Make install destinations relative so they respect CMAKE_INSTALL_PREFIX
    substituteInPlace CMakeLists.txt \
      --replace-fail 'RUNTIME DESTINATION /usr/bin' 'RUNTIME DESTINATION bin' \
      --replace-fail 'DESTINATION /usr/share/applications' 'DESTINATION share/applications' \
      --replace-fail 'DESTINATION /usr/share/icons' 'DESTINATION share/icons'

    # Remove forced /usr install prefix
    substituteInPlace CMakeLists.txt \
      --replace-fail $'if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)\n  set(CMAKE_INSTALL_PREFIX\n      "/usr"\n      CACHE PATH "Install path prefix" FORCE)\nendif()' ""

    # Use system libraries instead of FetchContent
    substituteInPlace CMakeLists.txt \
      --replace-fail '${loguruBlock}' 'find_package(loguru REQUIRED)'
    substituteInPlace CMakeLists.txt \
      --replace-fail '${jsonBlock}' 'find_package(nlohmann_json REQUIRED)'
    substituteInPlace CMakeLists.txt \
      --replace-fail '${glfwBlock}' 'find_package(glfw3 REQUIRED)'
    substituteInPlace CMakeLists.txt \
      --replace-fail '${stbBlock}' '# stb provided by nixpkgs'
    substituteInPlace CMakeLists.txt \
      --replace-fail '${libusbBlock}' '${libusbReplace}'
    substituteInPlace CMakeLists.txt \
      --replace-fail '${libevdevBlock}' '${libevdevReplace}'

    # Fix link targets
    substituteInPlace CMakeLists.txt \
      --replace-fail $'\n          usb-1.0\n          libevdev::libevdev)' $'\n          PkgConfig::LIBUSB\n          PkgConfig::LIBEVDEV)'

    # Remove stb_SOURCE_DIR from include path
    substituteInPlace CMakeLists.txt \
      --replace-fail 'include ''${stb_SOURCE_DIR}' 'include'
  '';

  meta = {
    description = "Unofficial Alienware Command Centre for Linux";
    homepage = "https://github.com/randomboi404/AWCC";
    license = lib.licenses.gpl3Plus;
    platforms = lib.platforms.linux;
    maintainers = [ ];
  };
}
