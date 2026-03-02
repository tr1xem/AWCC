# HACKING

Listing finding as I gather them

**NOTE**: if you send any illegal value through it the device should return `0xfffffff`

Base Construct : `\_SB.{DEVICE}.WMAX 0 {BYTE1} {{BYTE2}, {BYTE3}, 0x00, 0x00}`

Here device should be either `AMWW` for intel based and `AWM3` for AMD based

Legal Byte Values:

| NAME            | BYTE1 | BYTE2 | BYTE3 | Remarks                                                    |
| --------------- | ----- | ----- | ----- | ---------------------------------------------------------- |
| Get Mem Freq    | 0x1a  | 0x02  | 0x02  | Regarded somewhere as model getter but wrong               |
| Get power mode  | 0x14  | 0x0b  | 0x00  |                                                            |
| Set power mode  | 0x15  | 0x01  | X     | Byte 3 is the parameter from the modes table cant be blank |
| Toggle G mode   | 0x25  | 0x01  | 0x00  | needed sometime to actually mark gmode as set              |
| Get G mode      | 0x25  | 0x02  | 0x00  | Special func,gmode can be get with normal mode getter too  |
| Set fan1 boost  | 0x15  | 0x02  | 0x32  |                                                            |
| Get fan1 boost  | 0x14  | 0x0c  | 0x32  |                                                            |
| Get fan1 rpm    | 0x14  | 0x05  | 0x32  |                                                            |
| Get cpu temp    | 0x14  | 0x04  | 0x01  |                                                            |
| Set fan2 boost  | 0x15  | 0x02  | 0x33  |                                                            |
| Get fan2 boost  | 0x14  | 0x0c  | 0x33  |                                                            |
| Get fan2 rpm    | 0x14  | 0x05  | 0x33  |                                                            |
| Get gpu temp    | 0x14  | 0x04  | 0x06  |                                                            |
| Get gpu max rpm | 0x14  | 0x09  | 0x32  |                                                            |
| Get gpu min rpm | 0x14  | 0x08  | 0x32  |                                                            |
| Get cpu max rpm | 0x14  | 0x09  | 0x33  |                                                            |
| Get cpu min rpm | 0x14  | 0x08  | 0x33  |                                                            |

Mode Hex Code:
| MODE | VALUE | NOTE |
| ----------------- | ----- | ------------- |
| USTT_Balanced | 0xa0 | |
| USTT_Performance | 0xa1 | |
| USTT_Cool | 0xa2 | Works on 5511 |
| USTT_Quiet | 0xa3 | |
| USTT_FullSpeed | 0xa4 | |
| USTT_BatterySaver | 0xa5 | |
| G Mode | 0xab | |
| Manual | 0x0 | |
