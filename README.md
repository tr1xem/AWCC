# AWCC for Dell G series

Alienware Command Center for Dell G series with keybords USB 187c:0550 and USB 187c:0551

# Building  And  Installation

```bash
git clone https://github.com/tr1xem/AWCC
cd  AWCC
make
make install
```
# Usage
```bash
Alienware Command Center for  Dell G Series
Usage: awcc [command] [arguments]...

Light Controls:
        brightness <value>      Set brightness
        rainbow <duration>      Set Rainbows Spectrum
        wave <color>    Set Wave Effect
        bkf <color>     Set Back and fourth effect
        static <color>  Static color
        breathe <color> It lives and breathes!
        spectrum <duration>     Cycles through all colors

Fan Controls(Run  As root):
        g       G-Mode
        q       Quite Mode
        p       Performance Mode
        gt      G-Mode Toggle (useful for setting as keybinds)
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
notify-send -a "AWCC" "AWCC" "Set brightness to ${ARG}"
```
and Bind it to  a key preferrably F5

# TODO
- [X] CLI
- [ ] GUI


# Credits

- https://github.com/tiagoporsch/humanfx
- @meduk0
- Arch Wiki

