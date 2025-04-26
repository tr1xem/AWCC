# AWCC for Dell G series

Alienware Command Center for Dell G series with keybords USB 187c:0550 and USB 187c:0551 that auto detects if ur using intel or amd and with this you could set manual fan speeds too

# Building  And  Installation

-  Dependencies : libnotify

```bash
git clone https://github.com/tr1xem/AWCC
cd  AWCC
make
make install
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

# Usage
```bash
Alienware Command Center for Dell G Series
==========================================

Usage:
  awcc [command] [arguments]...

Lighting Controls:
  brightness <value>     Set keyboard brightness (0-100)
  static <color>         Set static color (hex RGB)
  breathe <color>        Breathing color effect
  wave <color>           Wave color effect
  bkf <color>            Back-and-forth color effect
  rainbow <duration>     Rainbow spectrum cycle (ms)
  spectrum <duration>    Full color cycle (ms)
  defaultblue            Set default static blue color

Fan Controls (Run as root):
  qm                     Query current fan mode
  g                      Set G-Mode
  q                      Set Quiet Mode
  p                      Set Performance Mode
  b                      Set Balanced Mode
  bs                     Set Battery Saver Mode
  gt                     Toggle G-Mode (useful for keybinds)

Fan Boost Controls (Run as root):
  cb                      Get CPU fan boost
  gb                      Get GPU fan boost
  scb <value>             Set CPU fan boost (1-100)
  sgb <value>             Set GPU fan boost (1-100)
```

# Device Tested

Tested on:
Dell  G15 5530 with USB 187c:0551

Should Work in all Dell G15 models and some G16 too
Feel Free to test and give suggestions!

# FAQ and TIPS

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
and Bind it to  a key preferrably F5

Q. Can I keybind the gmode toggle to a key like windows?

Ans: Ofc you can here is how i do it
```bash
#!/bin/bash

# Run the command to get the current mode
current_mode=$(sudo awcc qm | grep -oP 'Current mode: \K.*')
echo "Current mode: $current_mode"
# Check if the mode is "Gaming (G-Mode)" or not
if [[ "$current_mode" != "Gaming (G-Mode)" ]]; then
    # Mode is not Gaming (G-Mode), toggle to G-Mode
    sudo /usr/bin/awcc gt
    # Send notification for switching to Gaming Mode
    notify-send "Alienware Command Centre" "Turning on G-Mode"
else
    # Mode is Gaming (G-Mode), toggle to G-Mode
    sudo /usr/bin/awcc gt
    # Send notification for staying in Gaming Mode
    notify-send "Alienware Command Centre" "Turning off G-Mode"
fi
```
now add the awcc executable with full path a in visudo so it dont ask for passwords and then when ever u run the bash script you swich between modes with a notification.
# TODO
- [X] CLI
- [ ] GUI


# Credits

- https://github.com/tiagoporsch/humanfx
- @meduk0
- Arch Wiki

“Intelligence is the ability to avoid doing work, yet getting the work done.”

_~Linus Torvalds_
