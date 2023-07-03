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
  pinMode(GPIO_PIN_INPUT, INPUT);
  pinMode(GPIO_PIN_OUTPUT, OUTPUT);
  pinMode(SCREEN_PIN_BLK, OUTPUT);

  digitalWrite(GPIO_PIN_OUTPUT, RELAYS_STATE_REST);
  digitalWrite(SCREEN_PIN_BLK, SCREEN_STATE_REST);

  Serial.begin(115200);

  #if USE_BLYNK
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  #endif

  #if (USE_WEB_SERIAL && !USE_BLYNK)
    WiFi.begin(WIFI_SSID, WIFI_PASS);
  #endif

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

  #if USE_BLYNK
    terminalV10.clear();
    terminalV10.println("ESP32 is online");
    terminalV10.flush();
  #endif

  #if USE_WEB_SERIAL
    #if (defined(AUTH_USER) && defined(AUTH_PASS))
      WebSerialPro.setAuthentication(AUTH_USER, AUTH_PASS);
    #endif
    WebSerialPro.setID("Informer");
    WebSerialPro.begin(&server);
    WebSerialPro.msgCallback(get_web_input);
    server.begin();
  #endif
}

void loop()
{
  #if USE_BLYNK
    Blynk.run();

    if (digitalRead(GPIO_PIN_INPUT) == HIGH)
    {
      ledV6.on();
    }
    else
    {
      ledV6.off();
    }
  #endif

  #if USE_WEB_SERIAL
    WebSerialPro.loop();
  #endif
}
