All packets should be 33 bit sized

- Sending Animation Start:

```
0000   03 21 00 01 ff ff 00 00 00 00 00 00 00 00 00 00
0010   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0020   00
```

Decoded as `03` is the preamble, `21` is set animation flag, `00 01` are high and low value for config start flag,
`ff ff` are high and low for animation id(can be any digit apparntly? we use 01 for setting and 00 for temporray effects)

- Sending ZoneSelect:

```
0000   03 23 01 00 01 02 00 00 00 00 00 00 00 00 00 00
0010   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0020   00
```

`23` is zone select flag,`01` is look flag, `00 01` is the zone count high & low ,then all values ahead of them are zone codes.

- Send Action:

```
0000   03 24 00 07 d0 00 fa f1 f5 ff 00 00 00 00 00 00
0010   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0020   00
```

`24` is add action flag,`00` is the action flag (00 is static colour more can be found in lightfx.h),`07 d0` are high
& low for duration time,`00 fa` is the high & low for tempo, `f1 f5 ff` are the colours

More info at https://github.com/tr1xem/alienfx-linux/tree/main/AlienFX-SDK
