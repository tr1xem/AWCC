# AWCC for Dell G series

Alienware Command Center for Dell G series with keybords USB 187c:0550 and USB 187c:0551 that auto detects if ur using intel or amd and with this you could set manual fan speeds too

# Building And Installation

For Arch Based Distros

```bash
paru -S awcc-git
```

For other Distros see below

## Manual Installation

Depnedencies:

- `acpi_call-dkms`
- `libpusb`
- `pcrpe2`
- `sysptemd-libs`
- `glipbc`
- `libpcap`
- `gccp-libs`
- `gitp`
- `make`

```bash
git clone https://github.com/tr1xem/AWCC
cd  AWCC
sudo make install
```

Make a udev rule for it.(make sure to replace 0551 by 0550 if ur using that)

```bash
# /etc/udev/rules.d/99-awcc.rules
SUBSYSTEM=="usb", ATTRS{idVendor}=="187c", ATTRS{idProduct}=="0551", MODE="0660",GROUP="plugdev"

```

Then execute these cmds to add urself in plugdev

```bash
 sudo groupadd plugdev
 sudo usermod -aG plugdev $USER
```

> [!NOTE]
> If you dont want to run it as root you can use the `awccd` daemon
> that will run it as a user and will not require root privileges
> to run it.

# Usage

```bash
Alienware Command Center for Dell G Series
==========================================

Usage:
  awcc [command] [arguments]...
  awcc --test-mode [command] [arguments]...

Lighting Controls:
  brightness <0-100>   Set keyboard brightness
  static <color_hex>   Set static color
  spectrum <duration_ms> Color spectrum effect
  breathe <color_hex>  Breathing effect
  rainbow <duration_ms> Rainbow wave effect
  wave <color_hex>     Wave effect
  bkf <color_hex>      Back and forth effect
  defaultblue          Default blue color

Fan Controls (Run as root):
  qm                     Query current fan mode
  modes                  List available thermal modes
  quiet        (q )       Quiet mode for minimal noise
  battery      (bs)       Battery saving mode
  balance      (b )       Balanced performance and power
  performance  (p )       High performance mode
  gmode        (g )       Gaming mode (G-Mode)
  gt                     Toggle G-Mode (useful for keybinds)

Fan Boost Controls (Run as root):
  cb                      Get CPU fan boost
  scb <value>             Set CPU fan boost (1-100)
  gb                      Get GPU fan boost
  sgb <value>             Set GPU fan boost (1-100)

Fan RPM/Info Controls (Run as root):
  cr                      Get CPU fan RPM
  gr                      Get GPU fan RPM
  cfn                     Get CPU fan name
  gfn                     Get GPU fan name
  fans                    Show all fans status (RPM, boost, names)

Advanced Controls (Run as root):
  autoboost               Start automatic thermal management daemon

System Information:
  device-info             Show detected device model and supported features

Options:
  --test-mode             Skip device detection and feature validation
                          (for testing on unsupported devices)
```

# Device Tested

Tested on:
Dell G15 5530
Dell G16 7630

Should Work in all Dell G15 models and some G16 too
Feel Free to test and give suggestions!

# FAQ and TIPS

Q: This works on my device I want to add it to support officially?

Ans: Yes you can just run `sudo awcc device-info` and open pull request with the output

Q: How do a keybind for Light Toggle ?

Ans : Install it and make a script under `~/.local/share/bin` as follows

```bash
#!/bin/bash
STATE_FILE="rotate_state.txt"
VALUES=(0 50 100)
CURRENT_INDEX=$(cat "$STATE_FILE" 2>/dev/null || echo -1)
NEXT_INDEX=$(( (CURRENT_INDEX + 1) % ${#VALUES[@]} ))
echo $NEXT_INDEX > "$STATE_FILE"
ARG=${VALUES[$NEXT_INDEX]}
echo "Executing command with argument: $ARG"
awcc brightness "$ARG"
```

and Bind it to a key preferrably F5

### Got a suggestion or a feature request? link it up in [discussions](https://github.com/tr1xem/AWCC/discussions)

# TODO

- [x] CLI
- [ ] GUI

# Credits

- [GasparVardanyan](https://github.com/GasparVardanyan)
- [humanfx](https://github.com/tiagoporsch/humanfx)
- @meduk0
- Arch Wiki

“Intelligence is the ability to avoid doing work, yet getting the work done.”

_~Linus Torvalds_
