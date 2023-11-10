#include "main.h"

BLYNK_WRITE(V3)
{
  if (param.asInt() == 1)
  {
    digitalWrite(RELAY_PIN_OUTPUT, RELAY_STATE_WORK);
  }
  else
  {
    digitalWrite(RELAY_PIN_OUTPUT, RELAY_STATE_REST);
  }
}

BLYNK_WRITE(V10)
{  
  process_input(terminalV10, param.asStr());
  terminalV10.flush();
}

void setup()
{
  pinMode(GPIO_PIN_INPUT, INPUT_PULLDOWN);
  pinMode(RELAY_PIN_OUTPUT, OUTPUT);
  pinMode(LCD_PIN_BLK, OUTPUT);
  pinMode(MODEM_PIN_SW, OUTPUT);

  digitalWrite(RELAY_PIN_OUTPUT, RELAY_STATE_REST);

  #if USE_LCD
    digitalWrite(LCD_PIN_BLK, LCD_STATE_WORK);
  #else
    digitalWrite(LCD_PIN_BLK, LCD_STATE_REST);
  #endif

  #if USE_MODEM
    digitalWrite(MODEM_PIN_SW, MODEM_STATE_WORK);

    MODEM_SERIAL.begin(MODEM_BAUDRATE, SERIAL_8N1, MODEM_PIN_TX, MODEM_PIN_RX);
    Serial.println("Please enter the AT command in the Serial Monitor to interact");
  #else
    digitalWrite(MODEM_PIN_SW, MODEM_STATE_REST);
  #endif

  Serial.begin(115200);

  #if USE_WIFI
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    
    Serial.println("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }

    WiFi.enableIpV6();

    Serial.println("Connected to the WiFi network");
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
  #endif

  #if USE_ETH
    ESP32_W6100_onEvent();

    ETH.begin(ETH_PIN_MISO, ETH_PIN_MOSI, ETH_PIN_SCLK, ETH_PIN_CS, ETH_PIN_INT, ETH_SPI_CLOCK_MHZ, ETH_SPI_HOST);
  #endif

  #if USE_BLYNK
    Blynk.config(BLYNK_AUTH_TOKEN);

    terminalV10.clear();
    terminalV10.println("Informer is online");
    terminalV10.flush();

    if (digitalRead(GPIO_PIN_INPUT) == HIGH)
    {
      ledV6.on();
      terminalV10.println("Desktop is online");
      terminalV10.flush();
    }
    else
    {
      ledV6.off();
      terminalV10.println("Desktop is offline");
      terminalV10.flush();
    }
  #endif

  #if USE_WEB_SERIAL
    #if (defined(AUTH_USER) && defined(AUTH_PASS))
      WebSerialPro.setAuthentication(AUTH_USER, AUTH_PASS);
    #endif
    WebSerialPro.setID("Informer");
    WebSerialPro.begin(&server);
    WebSerialPro.msgCallback(get_web_input);
    server.begin();

    print_desktop_state(WebSerialPro);
  #endif

  #if USE_NTP
    configTime(NTP_TIMEZONE * 3600, NTP_DST * 3600, NTP_SERVER);

    if (getLocalTime(&network_time))
    {
      boot_time = mktime(&network_time);

      Serial.print("Informer Boot Time: ");
      Serial.printf("%d-%02d-%02d %02d:%02d:%02d\r\n",
                    network_time.tm_year + 1900, network_time.tm_mon + 1, network_time.tm_mday,
                    network_time.tm_hour, network_time.tm_min, network_time.tm_sec);
    }
    else
    {
      Serial.println("Failed to obtain time");
    }
  #endif

  attachInterrupt(digitalPinToInterrupt(GPIO_PIN_INPUT), interrupt_callback, CHANGE);
}

void loop()
{
  #if USE_BLYNK
    Blynk.run();
  #endif

  #if USE_WEB_SERIAL
    WebSerialPro.loop();
  #endif

  #if USE_MODEM
    while (MODEM_SERIAL.available())
    {
      Serial.write(MODEM_SERIAL.read());
    }
    if (Serial.available())
    {
      MODEM_SERIAL.write(Serial.read());
    }
  #endif

  if (interrupt_callback_flag)
  {
    delay(10);

    #if USE_BLYNK
      if (digitalRead(GPIO_PIN_INPUT) == HIGH)
      {
        ledV6.on();
        terminalV10.println("Desktop is online");
        terminalV10.flush();
      }
      else
      {
        ledV6.off();
        terminalV10.println("Desktop is offline");
        terminalV10.flush();
      }
    #endif
    
    #if USE_WEB_SERIAL
      print_desktop_state(WebSerialPro);
    #endif

    interrupt_callback_flag = false;
  }
}
