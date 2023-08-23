#include "backend.h"

volatile bool interrupt_callback_flag = false;

void switch_relay(uint16_t delay_ms)
{
  digitalWrite(RELAY_PIN_OUTPUT, RELAY_STATE_WORK);
  delay(delay_ms);
  digitalWrite(RELAY_PIN_OUTPUT, RELAY_STATE_REST);
}

void print_desktop_state(Print& out)
{
  if (digitalRead(GPIO_PIN_INPUT) == HIGH)
  {
    out.println("Desktop is online");
  }
  else
  {
    out.println("Desktop is offline");
  }
}

void print_esp_ip(Print& out)
{
  out.print("Local  IPv4: ");
  out.println(WiFi.localIP());
  out.print("Global IPv4: ");
  out.println(get_global_ipv4_address());
  out.print("Local  IPv6: ");
  out.println(WiFi.localIPv6());
  out.print("Global IPv6: ");
  out.println(get_global_ipv6_address());
}

void print_esp_uptime(Print& out)
{
  uint64_t uptime_us = esp_timer_get_time();
  uint64_t uptime_ms = uptime_us / 1000;
  uint32_t uptime_s = uptime_ms / 1000;
  uint32_t uptime_min = uptime_s / 60;
  uint16_t uptime_hour = uptime_min / 60;
  uint16_t uptime_day = uptime_hour / 24;

  uint32_t seconds = uptime_s % 60;
  uint32_t minutes = uptime_min % 60;
  uint16_t hours = uptime_hour % 24;
  uint16_t days = uptime_day;

  out.printf("System Up Time: %hd Days, %hd Hours, %d Minutes, %ld Seconds\r\n",
              days, hours, minutes, seconds);
}

void print_command(Print& out)
{
  const char* commands[] = {
    "check",
    "command",
    "ip",
    "reboot",
    "turnon",
    "turnoff",
    "uptime"
  };

  for (uint8_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
  {
    out.println(commands[i]);
  }
}

void print_firmware_version(Print& out)
{
  out.printf("Firmware Version: %d.%d.%d\r\n", FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR, FIRMWARE_VERSION_PATCH);
}

void print_compile_time(Print& out)
{
  out.printf("Compiled on: %s %s\r\n", __DATE__, __TIME__);
}

void process_input(Print& out, const String& input)
{
  std::map<String, std::function<void(Print&)>> commandMap = 
  {
    {"check", print_desktop_state},
    {"command", print_command},
    {"ip", print_esp_ip},
    {"reboot", [] (Print&) { ESP.restart(); }},
    {"turnon", [] (Print&) { switch_relay(500); }},
    {"turnoff", [] (Print&) { switch_relay(3000); }},
    {"uptime", print_esp_uptime}
  };

  if (commandMap.count(input))
  {
    commandMap[input](out);
  }
  else
  {
    out.println("Wrong input");
  }
}

void get_web_input(uint8_t *data, size_t len)
{
  String input(data, len);

  WebSerialPro.print("> ");
  WebSerialPro.println(input);

  process_input(WebSerialPro, input);
}

void interrupt_callback(void)
{
  interrupt_callback_flag = true;
}
