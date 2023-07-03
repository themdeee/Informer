#ifndef __BACKEND_H
#define __BACKEND_H

#include <map>

#include "config.h"
#include "ddns_esp32.h"
#include "ESPAsyncWebServer.h"
#include "WebSerialPro.h"

extern volatile bool interrupt_callback_flag;

void ARDUINO_ISR_ATTR interrupt_callback(void);

void switch_relay(uint16_t delay_ms);
void print_desktop_state(Print& out);
void print_esp_ip(Print& out);
void print_esp_uptime(Print& out);
void print_command(Print& out);
void process_input(Print& out, const String& input);
void get_web_input(uint8_t *data, size_t len);

#endif
