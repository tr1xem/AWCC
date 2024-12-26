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
        gt      G-Mode Toggle (use full for setting as keybinds)
```

# Device Tested

Tested on:
Dell  G15 5530 with USB 187c:0551

Should Work in all Dell G15 models and some G16 too
Feel Free to test and give suggestions!
# TODO
- [X] CLI
- [ ] GUI


# Credits

- https://github.com/tiagoporsch/humanfx
- @meduk0
- Arch Wiki

