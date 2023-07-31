#ifndef __MAIN_H
#define __MAIN_H

#include "backend.h"

#include <BlynkSimpleEsp32.h>

WidgetLED ledV6(V6);
WidgetTerminal terminalV10(V10);

AsyncWebServer server(WEB_SERIAL_PORT);

#endif
