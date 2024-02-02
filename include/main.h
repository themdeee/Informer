#ifndef __MAIN_H
#define __MAIN_H

#include "backend.h"

#if USE_BLYNK
  #include <BlynkSimpleEsp32.h>

  WidgetLED ledV6(V6);
  WidgetTerminal terminalV10(V10);
#endif

#if USE_WEB_SERIAL
  AsyncWebServer server(WEB_SERIAL_PORT);
#endif

#endif
