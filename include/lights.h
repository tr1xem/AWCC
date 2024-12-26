#pragma once

#include <stdint.h>

// Device
void device_open(void);
void device_acquire(void);
void device_release(void);
void device_close(void);
void device_send(const uint8_t data[], uint16_t length);
void device_receive(uint8_t data[], uint16_t length);

// Interface
#define PREAMBLE 0x03
#define ZONE_LEFT 0x00
#define ZONE_MIDDLE_LEFT 0x01
#define ZONE_MIDDLE_RIGHT 0x02
#define ZONE_RIGHT 0x03
#define ZONE_ALL ZONE_LEFT, ZONE_MIDDLE_LEFT, ZONE_MIDDLE_RIGHT, ZONE_RIGHT

#define REQUEST 0x20
#define REQUEST_FIRMWARE_VERSION 0x00
#define REQUEST_STATUS 0x01
#define REQUEST_ELC_CONFIG 0x02
#define REQUEST_ANIMATION_COUNT 0x03
void send_request_firmware_version(void);
void send_request_status(void);
void send_request_elc_config(void);
void send_request_animation_count(void);

#define ANIMATION 0x21
#define ANIMATION_CONFIG_START 0x0001
#define ANIMATION_CONFIG_SAVE 0x0002
#define ANIMATION_CONFIG_PLAY 0x0003
#define ANIMATION_REMOVE 0x0004
#define ANIMATION_PLAY 0x0005
#define ANIMATION_SET_DEFAULT 0x0006
#define ANIMATION_SET_STARTUP 0x0007
void send_animation_config_start(uint16_t animation_id);
void send_animation_config_play(uint16_t animation_id);
void send_animation_config_save(uint16_t animation_id);
void send_animation_remove(uint16_t animation_id);
void send_animation_play(uint16_t animation_id);
void send_animation_set_default(uint16_t animation_id);
void send_animation_set_startup(uint16_t animation_id);

#define ZONE_SELECT 0x23
void send_zone_select(uint8_t loop, uint16_t zone_count, ...);

#define ADD_ACTION 0x24
#define ACTION_COLOR 0x00
#define ACTION_PULSE 0x01
#define ACTION_MORPH 0x02
void send_add_action(uint16_t action, uint16_t duration, uint16_t tempo, uint32_t color);

#define SET_DIM 0x26
#define BRIGHTNESS_OFF 0x64
#define BRIGHTNESS_DIM 0x32
#define BRIGHTNESS_FULL 0x00
void send_set_dim(uint8_t dim, uint16_t zone_count, ...);