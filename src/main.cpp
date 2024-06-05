#include "main.h"

#if USE_BLYNK
  BLYNK_WRITE(V3)
  {
    if (param.asInt() == 1)
    {
      digitalWrite(INFORMER_PIN_OUTPUT, INFORMER_SWITCH_WORK);
    }
    else
    {
      digitalWrite(INFORMER_PIN_OUTPUT, INFORMER_SWITCH_REST);
    }
  }

  BLYNK_WRITE(V10)
  {  
    process_input(terminalV10, param.asStr());
    terminalV10.flush();
  }
#endif

void setup()
{
  pinMode(INFORMER_PIN_INPUT, INPUT_PULLDOWN);
  pinMode(INFORMER_PIN_OUTPUT, OUTPUT);
  pinMode(LCD_PIN_BLK, OUTPUT);
  pinMode(MODEM_PIN_SWITCH, OUTPUT);

  digitalWrite(INFORMER_PIN_OUTPUT, INFORMER_SWITCH_REST);

  #if USE_LCD
    digitalWrite(LCD_PIN_BLK, LCD_STATE_WORK);
  #else
    digitalWrite(LCD_PIN_BLK, LCD_STATE_REST);
  #endif

  #if USE_MODEM
    digitalWrite(MODEM_PIN_SWITCH, MODEM_STATE_REST);
    delay(500);
    digitalWrite(MODEM_PIN_SWITCH, MODEM_STATE_WORK);

    MODEM_SERIAL.begin(MODEM_BAUDRATE, SERIAL_8N1, MODEM_PIN_TX, MODEM_PIN_RX);
    Serial.println("Please enter the AT command in the Serial Monitor to interact");
  #else
    digitalWrite(MODEM_PIN_SWITCH, MODEM_STATE_REST);
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

    if (digitalRead(INFORMER_PIN_INPUT) == HIGH)
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
    #if USE_WEB_SERIAL_AUTH
      WebSerial.setAuthentication(WEB_SERIAL_AUTH_USER, WEB_SERIAL_AUTH_PASS);
    #endif
    WebSerial.setID("Informer");
    WebSerial.setTitle("Informer");
    WebSerial.begin(&server);
    WebSerial.onMessage(get_web_input);
    server.begin();

    print_desktop_state(WebSerial);
  #endif

  #if USE_NTP
    configTime(NTP_TIMEZONE * 3600, NTP_DST * 3600, NTP_SERVER);

    if (getLocalTime(&boot_time))
    {
      Serial.print("Informer Boot Time: ");
      Serial.printf("%d-%02d-%02d %02d:%02d:%02d\r\n",
                    boot_time.tm_year + 1900, boot_time.tm_mon + 1, boot_time.tm_mday,
                    boot_time.tm_hour, boot_time.tm_min, boot_time.tm_sec);
    }
    else
    {
      Serial.println("Failed to obtain time");
    }
  #endif

  attachInterrupt(digitalPinToInterrupt(INFORMER_PIN_INPUT), interrupt_callback, CHANGE);
}

void loop()
{
  #if USE_BLYNK
    Blynk.run();
  #endif

  #if USE_WEB_SERIAL
    WebSerial.loop();
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

  #if USE_REMOTER
    if (digitalRead(INFORMER_PIN_INPUT) == HIGH)
    {
      if (!client.connected()) 
      {
        Serial.println("Connecting to server...");

        if (client.connect(REMOTER_SERVER_IP, REMOTER_SERVER_PORT)) 
        {
          Serial.println("Connected to server");

          #if USE_BLYNK
            terminalV10.println("Connected to server");
            terminalV10.flush();
          #endif

          #if USE_WEB_SERIAL
            WebSerial.println("Connected to server");
          #endif
        }
        else
        {
          Serial.println("Failed to connect to server");
        }
      }

      if (client.available()) 
      {
        String data = client.readStringUntil('\n');

        #if USE_BLYNK
          terminalV10.print("-> ");
          terminalV10.println(data);
          terminalV10.flush();
        #endif

        #if USE_WEB_SERIAL
          WebSerial.print("-> ");
          WebSerial.println(data);
        #endif
      }
    }
  #endif

  if (interrupt_callback_flag)
  {
    delay(10);

    #if USE_BLYNK
      if (digitalRead(INFORMER_PIN_INPUT) == HIGH)
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
      print_desktop_state(WebSerial);
    #endif

    interrupt_callback_flag = false;
  }
}
