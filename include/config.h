#ifndef __CONFIG_H
#define __CONFIG_H

#define BLYNK_TEMPLATE_ID       ""
#define BLYNK_TEMPLATE_NAME     ""
#define BLYNK_AUTH_TOKEN        ""

#define FIRMWARE_VERSION_MAJOR  2
#define FIRMWARE_VERSION_MINOR  1
#define FIRMWARE_VERSION_PATCH  1

#define WIFI_SSID               ""
#define WIFI_PASS               ""

#define AUTH_USER               ""
#define AUTH_PASS               ""

#define USE_WIFI                true
#define USE_ETH                 true
#define USE_BLYNK               true
#define USE_WEB_SERIAL          true
#define USE_NTP                 true
#define USE_REMOTER             true
#define USE_MODEM               false
#define USE_LCD                 false

#define WEB_SERIAL_PORT         80

#define GPIO_PIN_INPUT          21

// Relay config
#define RELAY_PIN_OUTPUT        47

#define RELAY_STATE_WORK        HIGH
#define RELAY_STATE_REST        LOW

// ETH config
#define ETH_SPI_HOST            SPI3_HOST
#define ETH_SPI_CLOCK_MHZ       25

#define ETH_PIN_INT             9
#define ETH_PIN_MISO            11
#define ETH_PIN_MOSI            12
#define ETH_PIN_SCLK            13
#define ETH_PIN_CS              14
#define ETH_PIN_RST             10

// LCD config
#define LCD_PIN_CLK             1
#define LCD_PIN_MOSI            2
#define LCD_PIN_RES             42
#define LCD_PIN_DC              41
#define LCD_PIN_BLK             40
#define LCD_PIN_MISO            39
#define LCD_PIN_CS1             38
#define LCD_PIN_CS2             45
#define LCD_PIN_PEN             48

#define LCD_STATE_WORK          HIGH
#define LCD_STATE_REST          LOW

// Modem config
#define MODEM_SERIAL            Serial1
#define MODEM_BAUDRATE          115200

#define MODEM_PIN_TX            16
#define MODEM_PIN_RX            15
#define MODEM_PIN_FLT           17
#define MODEM_PIN_RST           34
#define MODEM_PIN_SW            8

#define MODEM_STATE_WORK        HIGH
#define MODEM_STATE_REST        LOW

// NTP config
#define NTP_SERVER              "pool.ntp.org"
#define NTP_TIMEZONE            8
#define NTP_DST                 0

// Remoter config
#define REMOTER_SERVER_IP       "192.168.1.1"
#define REMOTER_SERVER_PORT     12345

#endif
