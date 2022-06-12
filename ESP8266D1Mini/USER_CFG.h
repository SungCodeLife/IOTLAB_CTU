/*
 * pubsubclient version 2.8.0 by Nic O'leary 
 * when can not connect to server led on board will turn on
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*buffer data pubish*/
#define Buffer      240
#define ON          LOW
#define OFF         HIGH
#define LED(x)      digitalWrite(BUILTIN_LED, x)
#define READLED     digitalRead(BUILTIN_LED)
#define UARTSEND    Serial.print

#define endln       Serial.println();
#define show        Serial.print
#define showln      Serial.println

#define BAUDRATE1   9600
#define BAUDRATE2   115200

#define DEBUG_LOG   false

#define ssid_WiFi   "CTU_IOTLAB"
#define pass_WiFi   "02710271"
#define mqtt_server "192.168.137.1"

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;

WiFiClient espClient;
PubSubClient client(espClient);

 
