{
  description = "AWCC - Unofficial Alienware Command Centre for Linux";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      forAllSystems = nixpkgs.lib.genAttrs [ "x86_64-linux" "aarch64-linux" ];
    in {
      packages = forAllSystems (system: let
        pkgs = import nixpkgs { inherit system; };
      in {
        awcc = pkgs.callPackage ./nix/package.nix {
          waylandScanner = pkgs."wayland-scanner";
        };
        default = self.packages.${system}.awcc;
      });

      devShells = forAllSystems (system: let
        pkgs = import nixpkgs { inherit system; };
      in {
        default = pkgs.mkShell {
          name = "awcc-dev";
          packages = with pkgs; [
            cmake ninja meson pkg-config wayland wayland-scanner wayland-protocols
            libusb1 libevdev udev
            libglvnd libx11 libxrandr libxinerama libxcursor libxi
            libxkbcommon
          ];
        };
      });

      nixosModules = {
        awcc = import ./nix/module.nix;
        default = self.nixosModules.awcc;
      };
    };
}
