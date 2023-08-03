#ifndef __CONFIG_H
#define __CONFIG_H

#define BLYNK_TEMPLATE_ID       ""
#define BLYNK_TEMPLATE_NAME     ""
#define BLYNK_AUTH_TOKEN        ""

#define FIRMWARE_VERSION_MAJOR  1
#define FIRMWARE_VERSION_MINOR  2
#define FIRMWARE_VERSION_PATCH  1

#define WIFI_SSID               ""
#define WIFI_PASS               ""

#define AUTH_USER               ""
#define AUTH_PASS               ""

#define USE_WIFI                true
#define USE_ETH                 true

#define USE_BLYNK               true
#define USE_WEB_SERIAL          true

#define WEB_SERIAL_PORT         80

#define GPIO_PIN_INPUT          4
#define GPIO_PIN_OUTPUT         32

#define ETH_SPI_HOST            HSPI_HOST
#define ETH_SPI_CLOCK_MHZ       25

#define ETH_PIN_INT             27
#define ETH_PIN_MISO            12
#define ETH_PIN_MOSI            13
#define ETH_PIN_SCLK            14
#define ETH_PIN_CS              15
#define ETH_PIN_RST             26

#define SCREEN_PIN_BLK          2

#define RELAYS_STATE_WORK       HIGH
#define RELAYS_STATE_REST       LOW

#define SCREEN_STATE_WORK       HIGH
#define SCREEN_STATE_REST       LOW

#endif
