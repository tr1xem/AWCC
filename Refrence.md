```python


self.acpi_cmd = "echo \"\\_SB.AMWW.WMAX 0 {} {{{}, {}, {}, 0x00}}\" | tee /proc/acpi/call; cat /proc/acpi/call"

self.acpi_call_dict = {
    "get_laptop_model" : ["0x1a", "0x02", "0x02"],
    "get_power_mode" : ["0x14", "0x0b", "0x00"],
    "set_power_mode" : ["0x15", "0x01"],    #To be used with a parameter
    "toggle_G_mode" : ["0x25", "0x01"],
    "get_G_mode" : ["0x25", "0x02"],
    "set_fan1_boost" : ["0x15", "0x02", "0x32"],            #To be used with a parameter
    "get_fan1_boost" : ["0x14", "0x0c", "0x32"],
    "get_fan1_rpm" : ["0x14", "0x05", "0x32"],
    "get_cpu_temp" : ["0x14", "0x04", "0x01"],
    "set_fan2_boost" : ["0x15", "0x02", "0x33"],            #To be used with a parameter
    "get_fan2_boost" : ["0x14", "0x0c", "0x33"],
    "get_fan2_rpm" : ["0x14", "0x05", "0x33"],
    "get_gpu_temp" : ["0x14", "0x04", "0x06"]
}

self.power_modes_dict = {
    "USTT_Balanced" : "0xa0",
    "USTT_Performance" : "0xa1",
    # "USTT_Cool" : "0xa2",   #Does not work
    "USTT_Quiet" : "0xa3",
    "USTT_FullSpeed": "0xa4",
    "USTT_BatterySaver" : "0xa5",
    "G Mode" : "0xab",
    "Manual" : "0x0",
}
if (laptop_model == "0x12c0"):
            print("Detected dell g15 5525. Laptop model: 0x{}".format(laptop_model))


if (laptop_model == "0xc80"):
            print("Detected dell g15 5515. Laptop model: 0x{}".format(laptop_model))

if (laptop_model == "0xc80"):
            print("Detected dell g15 5511. Laptop model: 0x{}".format(laptop_model))

if (laptop_model == "0x12c0"):
            print("Detected dell g15 5520. Laptop model: 0x{}".format(laptop_model))
```
