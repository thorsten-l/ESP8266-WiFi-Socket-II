#ifndef __DEVICE_CONFIG_HPP__
#define __DEVICE_CONFIG_HPP__

#ifdef BOARD_TYPE_OBI_V2
  // Late 2018 Obi Wifi Socket
  #define WIFI_LED      12
  #define POWER_LED     13
  #define POWER_BUTTON  5
  #define RELAY_PIN     4
  #define WIFI_LED_ON   1
  #define WIFI_LED_OFF  0
  #define POWER_LED_ON  1
  #define POWER_LED_OFF 0
#endif

#ifdef BOARD_TYPE_OBI_V1
  // First Obi Wifi Socket
  #define WIFI_LED          4
  #define POWER_BUTTON      14
  #define RELAY_TRIGGER_OFF 5
  #define RELAY_TRIGGER_ON  12
  #define WIFI_LED_ON       1
  #define WIFI_LED_OFF      0
#endif

#ifdef BOARD_TYPE_DEV1
  #define WIFI_LED      LED_BUILTIN // D4, GPIO2
  #define POWER_LED     D6          // GPIO12
  #define POWER_BUTTON  D7          // GPIO13
  #define RELAY_PIN     D5          // GPIO14
  #define WIFI_LED_ON   0
  #define WIFI_LED_OFF  1
  #define POWER_LED_ON  0
  #define POWER_LED_OFF 1
#endif

#ifdef BOARD_TYPE_BW_SHP6
  #define WIFI_LED      2 
  #define POWER_LED     0
  #define POWER_BUTTON  13
  #define RELAY_PIN     15
  #define WIFI_LED_ON   0
  #define WIFI_LED_OFF  1
  #define POWER_LED_ON  0
  #define POWER_LED_OFF 1

  #define HAVE_ENERGY_SENSOR
  #define HAVE_HLW8012
  #define SEL_PIN 12
  #define CF1_PIN 14
  #define CF_PIN 5
  #define UPDATE_TIME 2000
  #define CURRENT_MODE LOW
#endif

#endif
