#ifndef __MAIN_H
#define __MAIN_H

#include <BlynkSimpleEsp32.h>

#include "backend.h"

WidgetLED ledV6(V6);
WidgetTerminal terminalV10(V10);

AsyncWebServer server(WEB_SERIAL_PORT);

#endif
