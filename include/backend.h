#ifndef __BACKEND_H
#define __BACKEND_H

#include "config.h"

#include <map>
#include <esp_ddns.h>
#include <WebSerialPro.h>

extern struct tm network_time;
extern volatile time_t boot_time;
extern volatile bool interrupt_callback_flag;

void ARDUINO_ISR_ATTR interrupt_callback(void);

void switch_relay(uint16_t delay_ms);
void print_desktop_state(Print& out);
void print_esp_ip(Print& out);
void print_esp_uptime(Print& out);
void print_network_time(Print& out);
void print_command(Print& out);
void print_firmware_version(Print& out);
void print_compile_time(Print& out);
void process_input(Print& out, const String& input);
void get_web_input(uint8_t *data, size_t len);

#endif
