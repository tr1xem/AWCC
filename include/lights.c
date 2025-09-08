#include <libusb-1.0/libusb.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lights.h"

// Logging
static void log_fatal(const char *msg) {
  fprintf(stderr, "fatal: %s\n", msg);
  exit(1);
}

static void log_error(const char *msg) { fprintf(stderr, "error: %s\n", msg); }

// Device
static libusb_context *context = NULL;
static libusb_device_handle *handle = NULL;
static bool acquired = false;

void device_open(void) {
  if (context)
    log_fatal("libusb already initialized");
  if (handle)
    log_fatal("device already opened");

  libusb_init(&context);
  libusb_set_option(context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);

  libusb_device **devices;
  ssize_t count = libusb_get_device_list(context, &devices);
  if (count < 0)
    log_fatal("get device list");

  libusb_device *device = NULL;
  for (ssize_t i = 0; i < count; i++) {
    struct libusb_device_descriptor descriptor = {0};
    if (libusb_get_device_descriptor(devices[i], &descriptor) != 0)
      log_fatal("get device descriptor");
    if (descriptor.idVendor == 0x187c &&
        (descriptor.idProduct == 0x0551 || descriptor.idProduct == 0x0550)) {
      device = devices[i];
      break;
    }
  }
  libusb_free_device_list(devices, 1);
  if (device == NULL)
    log_fatal("find device");

  if (libusb_open(device, &handle) != 0)
    log_fatal("open device");
}

void device_acquire(void) {
  if (!handle)
    log_fatal("device not opened");
  if (acquired)
    return;
  if (libusb_kernel_driver_active(handle, 0))
    if (libusb_detach_kernel_driver(handle, 0) != 0)
      log_fatal("detach kernel driver");
  if (libusb_claim_interface(handle, 0) != 0)
    log_fatal("claim interface");
  acquired = true;
}

void device_release(void) {
  if (!handle)
    log_fatal("device not opened");
  if (!acquired)
    return;
  libusb_release_interface(handle, 0);
  libusb_attach_kernel_driver(handle, 0);
  acquired = false;
}

void device_close(void) {
  if (!handle)
    log_fatal("device not opened");
  if (acquired)
    device_release();
  libusb_close(handle);
  handle = NULL;
  libusb_exit(context);
  context = NULL;
}

void device_send(const uint8_t data[], uint16_t length) {
  if (!acquired)
    log_fatal("device not acquired");
  unsigned char buffer[33];
  memset(buffer, 0, sizeof(buffer));
  memcpy(buffer, data, length);
  if (libusb_control_transfer(handle, 0x21, 9, 0x202, 0, buffer, 33, 0) != 33)
    log_error("could't write full packet");
}

void device_receive(uint8_t data[], uint16_t length) {
  if (!acquired)
    log_fatal("device not acquired");
  unsigned char buffer[33];
  if (libusb_control_transfer(handle, 0xA1, 1, 0x101, 0, buffer, 33, 0) != 33)
    log_error("could't read full packet");
  memcpy(data, buffer, length > 33 ? 33 : length);
}

// Interface
void send_request_firmware_version(void) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          REQUEST,
          REQUEST_FIRMWARE_VERSION,
      },
      3);
}

void send_request_status(void) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          REQUEST,
          REQUEST_STATUS,
      },
      3);
}

void send_request_elc_config(void) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          REQUEST,
          REQUEST_ELC_CONFIG,
      },
      3);
}

void send_request_animation_count(void) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          REQUEST,
          REQUEST_ANIMATION_COUNT,
      },
      3);
}

void send_animation_config_start(uint16_t animation_id) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          ANIMATION,
          (ANIMATION_CONFIG_START >> 8) & 0xFF,
          (ANIMATION_CONFIG_START) & 0xFF,
          (animation_id >> 8) & 0xFF,
          (animation_id) & 0xFF,
      },
      6);
}

void send_animation_config_play(uint16_t animation_id) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          ANIMATION,
          (ANIMATION_CONFIG_PLAY >> 8) & 0xFF,
          (ANIMATION_CONFIG_PLAY) & 0xFF,
          (animation_id >> 8) & 0xFF,
          (animation_id) & 0xFF,
      },
      6);
}

void send_animation_config_save(uint16_t animation_id) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          ANIMATION,
          (ANIMATION_CONFIG_SAVE >> 8) & 0xFF,
          (ANIMATION_CONFIG_SAVE) & 0xFF,
          (animation_id >> 8) & 0xFF,
          (animation_id) & 0xFF,
      },
      6);
}

void send_animation_remove(uint16_t animation_id) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          ANIMATION,
          (ANIMATION_REMOVE >> 8) & 0xFF,
          (ANIMATION_REMOVE) & 0xFF,
          (animation_id >> 8) & 0xFF,
          (animation_id) & 0xFF,
      },
      6);
}

void send_animation_play(uint16_t animation_id) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          ANIMATION,
          (ANIMATION_PLAY >> 8) & 0xFF,
          (ANIMATION_PLAY) & 0xFF,
          (animation_id >> 8) & 0xFF,
          (animation_id) & 0xFF,
      },
      6);
}

void send_animation_set_default(uint16_t animation_id) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          ANIMATION,
          (ANIMATION_SET_DEFAULT >> 8) & 0xFF,
          (ANIMATION_SET_DEFAULT) & 0xFF,
          (animation_id >> 8) & 0xFF,
          (animation_id) & 0xFF,
      },
      6);
}

void send_animation_set_startup(uint16_t animation_id) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          ANIMATION,
          (ANIMATION_SET_STARTUP >> 8) & 0xFF,
          (ANIMATION_SET_STARTUP) & 0xFF,
          (animation_id >> 8) & 0xFF,
          (animation_id) & 0xFF,
      },
      6);
}

void send_zone_select(uint8_t loop, uint16_t zone_count, ...) {
  va_list args;
  va_start(args, zone_count);
  uint8_t packet[5 + zone_count];
  packet[0] = PREAMBLE;
  packet[1] = ZONE_SELECT;
  packet[2] = loop;
  packet[3] = (zone_count >> 8) & 0xFF;
  packet[4] = (zone_count) & 0xFF;
  for (uint16_t i = 0; i < zone_count; i++)
    packet[5 + i] = (uint8_t)va_arg(args, int);
  device_send(packet, sizeof(packet));
  va_end(args);
}

void send_add_action(uint16_t action, uint16_t duration, uint16_t tempo,
                     uint32_t color) {
  device_send(
      (uint8_t[]){
          PREAMBLE,
          ADD_ACTION,
          action,
          (duration >> 8) & 0xFF,
          (duration) & 0xFF,
          (tempo >> 8) & 0xFF,
          (tempo) & 0xFF,
          (color >> 16) & 0xFF,
          (color >> 8 & 0xFF),
          (color) & 0xFF,
      },
      10);
}

void send_set_dim(uint8_t dim, uint16_t zone_count, ...) {
  va_list args;
  va_start(args, zone_count);
  uint8_t packet[5 + zone_count];
  packet[0] = PREAMBLE;
  packet[1] = SET_DIM;
  packet[2] = dim;
  packet[3] = (zone_count >> 8) & 0xFF;
  packet[4] = (zone_count) & 0xFF;
  for (uint16_t i = 0; i < zone_count; i++)
    packet[5 + i] = (uint8_t)va_arg(args, int);
  device_send(packet, sizeof(packet));
  va_end(args);
}
