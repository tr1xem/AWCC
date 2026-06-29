{ lib, pkgs, config, ... }:

let
  cfg = config.services.awcc;
  defaultPackage = (pkgs.callPackage ./package.nix { waylandScanner = pkgs."wayland-scanner"; }).overrideAttrs (old: {
    preConfigure = (old.preConfigure or "") + ''

      # NixOS patches

      # Font footer null dereference (--gui SIGSEGV)
      sed -i '/^         ImFont \&fontbold, int \&brightness, EffectController \&effects,$/a\         ImFont \&fontfooter,' include/Gui.h
      sed -i 's/EffectController \&effects, bool \&turbo)/EffectController \&effects, ImFont \&fontfooter, bool \&turbo)/' src/gui/Gui.cpp
      sed -i '/^        static ImGuiIO \&io = ImGui::GetIO();$/,/^        ImGui::PushFont(fontfooter);$/c\        ImGui::PushFont(\&fontfooter);' src/gui/Gui.cpp
      sed -i '/roboto_light, static_cast<int>(roboto_light_len), 17\.0F, \&cfg);/a\
        ImFont *fontfooter = io.Fonts->AddFontFromMemoryTTF(\
            roboto_medium, roboto_medium_len, 15.0F, \&cfg);' src/gui/Render.cpp
      sed -i 's/\*smallFont, \*fontbold, brightness, effects, turbo)/*smallFont, *fontbold, brightness, effects, *fontfooter, turbo)/' src/gui/Render.cpp

      # Strip pkexec/sudo from daemon-bound commands (daemon runs as root)
      sed -i 's|result = m_daemon\.executeFromDaemon(command\.c_str());|{ std::string dCmd = command; size_t pp = dCmd.find("pkexec "); if (pp != std::string::npos) dCmd.erase(pp, 7); result = m_daemon.executeFromDaemon(dCmd.c_str()); }|' src/AcpiUtils.cpp
      sed -i 's|m_daemon\.executeFromDaemon(cmd\.c_str());|{ auto p = cmd.find("sudo "); if (p != std::string::npos) cmd.erase(p, 5); m_daemon.executeFromDaemon(cmd.c_str()); }|' src/AcpiUtils.cpp
      sed -i 's|pkexec sh|(?:pkexec )?sh|' src/Daemon.cpp
      sed -i 's~\\| sudo tee~\\| (?:sudo )?tee~g' src/Daemon.cpp

      # Allow root (sudo) clients to connect to daemon
      # sed -i 's|cred.uid != EXPECTED_UID|cred.uid != EXPECTED_UID \&\& cred.uid != 0|' src/Daemon.cpp
    '';
  });
in {
  options.services.awcc = {
    enable = lib.mkEnableOption "AWCC daemon and udev rules";

    package = lib.mkOption {
      type = lib.types.package;
      default = defaultPackage;
      defaultText = lib.literalExpression "(pkgs.callPackage ./package.nix { }).overrideAttrs ...";
      description = "AWCC package to use (patched for NixOS compat)";
    };

    daemon = {
      enable = lib.mkOption {
        type = lib.types.bool;
        default = true;
        description = "Enable the AWCC systemd daemon";
      };
    };
  };

  config = lib.mkIf cfg.enable {
    environment.systemPackages = [ cfg.package ];

    services.udev.packages = [ cfg.package ];

    environment.etc."awcc/database.json".source = "${cfg.package}/share/awcc/database.json";

    systemd.services.awccd = lib.mkIf cfg.daemon.enable {
      description = "Alienware Command Centre Daemon";
      documentation = [ "https://github.com/tr1xem/AWCC" ];
      after = [ "multi-user.target" ];
      wants = [ "multi-user.target" ];
      wantedBy = [ "multi-user.target" ];
      path = with pkgs; [ bash coreutils-full ];
      serviceConfig = {
        Type = "simple";
        ExecStartPre = "-${pkgs.coreutils}/bin/rm -f /tmp/awcc.sock";
        ExecStart = "${cfg.package}/bin/awcc -d -v";
        ExecStop = "${pkgs.coreutils}/bin/kill -TERM $MAINPID";
        Restart = "always";
        RestartSec = "5";
        User = "root";
        Group = "root";
      };
    };
  };
}
