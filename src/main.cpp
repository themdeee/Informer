#include "main.h"

BLYNK_WRITE(V3)
{
  if (param.asInt() == 1)
  {
    digitalWrite(GPIO_PIN_OUTPUT, RELAYS_STATE_WORK);
  }
  else
  {
    digitalWrite(GPIO_PIN_OUTPUT, RELAYS_STATE_REST);
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
  pinMode(GPIO_PIN_OUTPUT, OUTPUT);
  pinMode(SCREEN_PIN_BLK, OUTPUT);

  digitalWrite(GPIO_PIN_OUTPUT, RELAYS_STATE_REST);
  digitalWrite(SCREEN_PIN_BLK, SCREEN_STATE_REST);

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
    terminalV10.println("ESP32 is online");
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
