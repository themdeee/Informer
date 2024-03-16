#ifndef __CONFIG_H
#define __CONFIG_H

// Informer Config
#define INFORMER_FIRMWARE_VERSION_MAJOR   2
#define INFORMER_FIRMWARE_VERSION_MINOR   2
#define INFORMER_FIRMWARE_VERSION_PATCH   0

#define INFORMER_PIN_INPUT                47
#define INFORMER_PIN_OUTPUT               21

#define INFORMER_SWITCH_WORK              HIGH
#define INFORMER_SWITCH_REST              LOW

// WiFi Config
#define USE_WIFI      true

#if USE_WIFI
  #define WIFI_SSID   ""
  #define WIFI_PASS   ""
#endif

// ETH Config
#define USE_ETH               false

#if USE_ETH
  #define ETH_SPI_HOST        SPI3_HOST
  #define ETH_SPI_CLOCK_MHZ   25

  #define ETH_PIN_INT         9
  #define ETH_PIN_MISO        11
  #define ETH_PIN_MOSI        12
  #define ETH_PIN_SCLK        13
  #define ETH_PIN_CS          14
  #define ETH_PIN_RST         10
#endif

// LCD Config
#define USE_LCD           false

#define LCD_PIN_BLK       48

#define LCD_STATE_WORK    HIGH
#define LCD_STATE_REST    LOW

#if USE_LCD
  #define LCD_PIN_MOSI      1
  #define LCD_PIN_DC        2
  #define LCD_PIN_CLK       42
  #define LCD_PIN_CS        41
  #define LCD_PIN_RES       40
  #define LCD_PIN_SCL       39
  #define LCD_PIN_SDA       38
  #define LCD_PIN_INT       45
#endif

// Modem Config
#define USE_MODEM           false

#define MODEM_PIN_SWITCH    8

#define MODEM_STATE_WORK    HIGH
#define MODEM_STATE_REST    LOW

#if USE_MODEM
  #define MODEM_SERIAL      Serial1
  #define MODEM_BAUDRATE    115200
  
  #define MODEM_PIN_TX      6
  #define MODEM_PIN_RX      5
  #define MODEM_PIN_FLT     7
  #define MODEM_PIN_RST     15
#endif

// Blynk Config
#define USE_BLYNK               true

#if USE_BLYNK
  #define BLYNK_TEMPLATE_ID     ""
  #define BLYNK_TEMPLATE_NAME   ""
  #define BLYNK_AUTH_TOKEN      ""
#endif

// Web Serial Config
#define USE_WEB_SERIAL            true

#if USE_WEB_SERIAL
  #define WEB_SERIAL_PORT         80

  #define USE_WEB_SERIAL_AUTH     true
#endif

#if USE_WEB_SERIAL_AUTH
  #define WEB_SERIAL_AUTH_USER    ""
  #define WEB_SERIAL_AUTH_PASS    ""
#endif

// NTP Config
#define USE_NTP           true

#if USE_NTP
  #define NTP_SERVER      "pool.ntp.org"
  #define NTP_TIMEZONE    8
  #define NTP_DST         0
#endif

// Remoter Config
#define USE_REMOTER             true

#if USE_REMOTER
  #define REMOTER_SERVER_IP     "192.168.1.1"
  #define REMOTER_SERVER_PORT   12345
#endif

#endif
