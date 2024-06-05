#include "backend.h"

class WiFiClient client;
struct tm boot_time;
struct tm network_time;
volatile bool interrupt_callback_flag = false;

void switch_relay(uint16_t delay_ms)
{
  digitalWrite(INFORMER_PIN_OUTPUT, INFORMER_SWITCH_WORK);
  delay(delay_ms);
  digitalWrite(INFORMER_PIN_OUTPUT, INFORMER_SWITCH_REST);
}

void print_desktop_state(Print& out)
{
  if (digitalRead(INFORMER_PIN_INPUT) == HIGH)
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

  out.printf("Informer Up Time: %hd Days, %hd Hours, %d Minutes, %ld Seconds\r\n",
              days, hours, minutes, seconds);

  if (getLocalTime(&network_time))
  {
    time_t boot_time_s = mktime(&boot_time);
    time_t network_time_s = mktime(&network_time);

    uptime_s = network_time_s - boot_time_s;
    uptime_min = uptime_s / 60;
    uptime_hour = uptime_min / 60;
    uptime_day = uptime_hour / 24;

    seconds = uptime_s % 60;
    minutes = uptime_min % 60;
    hours = uptime_hour % 24;
    days = uptime_day;

    out.printf("Informer has been running for: %hd Days, %hd Hours, %d Minutes, %ld Seconds ",
                days, hours, minutes, seconds);
    out.printf("since %d-%02d-%02d %02d:%02d:%02d\r\n",
                boot_time.tm_year + 1900, boot_time.tm_mon + 1, boot_time.tm_mday,
                boot_time.tm_hour, boot_time.tm_min, boot_time.tm_sec);
  }
  else
  {
    out.println("Failed to obtain time");
  }
}

void print_network_time(Print& out)
{
  if (getLocalTime(&network_time))
  {
    out.printf("Network Time: %d-%02d-%02d %02d:%02d:%02d\r\n",
                network_time.tm_year + 1900, network_time.tm_mon + 1, network_time.tm_mday,
                network_time.tm_hour, network_time.tm_min, network_time.tm_sec);
  }
  else
  {
    out.println("Failed to obtain time");
  }
}

void print_firmware_version(Print& out)
{
  out.printf("Firmware Version: %d.%d.%d\r\n", INFORMER_FIRMWARE_VERSION_MAJOR, INFORMER_FIRMWARE_VERSION_MINOR, INFORMER_FIRMWARE_VERSION_PATCH);
}

void print_compile_time(Print& out)
{
  out.printf("Compiled on: %s %s\r\n", __DATE__, __TIME__);
}

void print_logo(Print& out)
{
  out.println("      __        __      ");
  out.println("     /  \\      /  \\     ");
  out.println("    |    \\____/    |    ");
  out.println("   |----------------|   ");
  out.println("  ||**     || **    ||  ");
  out.println("  ||_**___/  \\_**___||  ");
  out.println("  |                  |  ");
  out.println("  |                  |  ");
  out.println("  |                  |  ");
  out.println("  |__________________|  ");
  out.println("___     _               ");
  out.println(" | __ _|_ _  ____  _  __");
  out.println("_|_| | | (_) | |||(/_ | ");
}

void print_info(Print& out)
{
  out.println("USE WIFI: " + String(USE_WIFI ? "true" : "false"));
  out.println("USE ETH: " + String(USE_ETH ? "true" : "false"));
  out.println("USE LCD: " + String(USE_LCD ? "true" : "false"));
  out.println("USE MODEM: " + String(USE_MODEM ? "true" : "false"));

  out.println("USE BLYNK: " + String(USE_BLYNK ? "true" : "false"));
  out.println("USE WEB SERIAL: " + String(USE_WEB_SERIAL ? "true" : "false"));
  out.println("USE NTP: " + String(USE_NTP ? "true" : "false"));
  out.println("USE REMOTER: " + String(USE_REMOTER ? "true" : "false"));
}

void process_input(Print& out, const String& input)
{
  char *command = NULL;
  char *option = NULL;
  char *parameter = NULL;

  char *token = strtok(const_cast<char*>(input.c_str()), " ");

  if (token != NULL)
  {
    command = token;
    token = strtok(NULL, " ");
  }
  if (token != NULL && token[0] == '-')
  {
    option = token + 1;
    token = strtok(NULL, " ");
  }
  if (token != NULL)
  {
    parameter = token;
  }
  
  if (strcmp(command, "reboot") == 0)
  {
    ESP.restart();
  }
  else if (strcmp(command, "sysinfo") == 0)
  {
    if (option == NULL)
    {
      print_logo(out);
      print_firmware_version(out);
      print_compile_time(out);
    }
    else if (strcmp(option, "ip") == 0)
    {
      print_esp_ip(out);
    }
    else if (strcmp(option, "time") == 0)
    {
      print_network_time(out);
    }
    else if (strcmp(option, "uptime") == 0)
    {
      print_esp_uptime(out);
    }
    else if (strcmp(option, "all") == 0)
    {
      print_logo(out);
      print_firmware_version(out);
      print_compile_time(out);
      print_esp_ip(out);
      print_network_time(out);
      print_esp_uptime(out);
      print_info(out);
    }
    else
    {
      out.println("Invalid option");
    }
  }
  else if (strcmp(command, "remoter") == 0)
  {
    if (digitalRead(INFORMER_PIN_INPUT) == HIGH)
    {
      if (option != NULL)
      {
        if (strcmp(option, "run") == 0 || strcmp(option, "kill") == 0 || strcmp(option, "status") == 0)
        {
          if (parameter != NULL)
          {
            std::string command = std::string(parameter) + " " + std::string(option);
            client.println(command.c_str());
          }
          else
          {
            out.println("Parameter is required");
          }
        }
        else
        {
          out.println("Invalid option");
        }
      }
      else
      {
        out.println("Option is required");
      }
    }
    else
    {
      out.println("Desktop is offline");
    }
  }
  else if (strcmp(command, "power") == 0)
  {
    if (option != NULL)
    {
      if (strcmp(option, "on") == 0)
      {
        switch_relay(500);
      }
      else if (strcmp(option, "off") == 0)
      {
        switch_relay(3000);
      }
      else if (strcmp(option, "status") == 0)
      {
        print_desktop_state(out);
      }
      else
      {
        out.println("Invalid option");
      }
    }
    else
    {
      out.println("Option is required");
    }
  }
  else if (strcmp(command, "help") == 0)
  {
    out.println("usage: command -[option] [parameter]");
    out.println("help");
    out.println("reboot");
    out.println("power -[on,off,status]");
    out.println("sysinfo -[ip,time,uptime,all]");
    out.println("remoter -[run,kill,status] [app]");
  }
  else
  {
    out.println("Invalid command");
  }
}

void get_web_input(uint8_t *data, size_t len)
{
  String input(data, len);

  WebSerial.print("> ");
  WebSerial.println(input);

  process_input(WebSerial, input);
}

void interrupt_callback(void)
{
  interrupt_callback_flag = true;
}
