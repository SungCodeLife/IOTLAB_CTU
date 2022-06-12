#include "user_cfg.h"

// variable declare
uint32 seconds = 0;

// prototype function
void MQTT_Reconnect();
void WIFI_SETUP();
void SerialRead();
void DataPublish(char topic[], String DataUser);
inline void DebugLog_Show(String content);

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);
    LED(OFF);
    Serial.begin(BAUDRATE2);
    WIFI_SETUP();
    /* setup Server */
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop()
{
    if (!client.connected())
      MQTT_Reconnect();
    client.loop();
    SerialRead();
    if(millis() - seconds >= 1000)
    {
      DataPublish("MQTT_esp8266_status", String(seconds/1000));
      seconds = millis();
    }
}

void SerialRead()
{
    String DataBuffer = "";
    while(Serial.available())
    {
        DataBuffer = Serial.readString();
        if(DataBuffer != "")
        {
            DataPublish("MQTT_DEBUG", DataBuffer);
            delay(100);
            DataPublish("MQTT_Data_Pub", DataBuffer);
        }
    }
}

/* -----------------------Program Function-----------------------*/
void WIFI_SETUP()
{
    LED(OFF);
    delay(10);
    DebugLog_Show("\nConnecting to WiFi: " + String(ssid_WiFi) + "...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_WiFi, pass_WiFi);
    while (WiFi.status() != WL_CONNECTED)
    {
        LED(!READLED);
        delay(300);
    }
    DebugLog_Show("\nWIFI CONNECTED");
}

/* ---------------------------MQTT function ---------------------------*/
void callback(char* topic, byte* payload, unsigned int length)
{
    String mess_payload = "";
    char msg_process[length+1];

    DebugLog_Show("Message [" + (String)topic + "]");
    for (int i = 0; i < length; i++)
        mess_payload += (char)payload[i];
    DebugLog_Show(mess_payload);
    UARTSEND(mess_payload);

} /* end callback */

void DataPublish(char topic[], String DataUser)
{
    uint8 value = 0;
    char msg[Buffer];
    char payload[DataUser.length()+1];
    for(int i=0; i<= DataUser.length(); ++i)
        payload[i] = DataUser[i];
    snprintf(msg, Buffer, payload);
    boolean boolPub = client.publish(topic, msg);
    if(!boolPub)
        DebugLog_Show("mistake to send data to server.");
}

void MQTT_Reconnect()
{
  while (!client.connected())
  {
    DebugLog_Show("Connecting to MQTT Server...");
    if(WiFi.status() != WL_CONNECTED)
        WIFI_SETUP();
    if(client.connect("ESP"))
    {
        LED(OFF);
        DebugLog_Show("CONNECTED MQTT");
        client.subscribe("MQTT_Data_Sub");
    }
    else{
        DebugLog_Show("failed, rc = " + String(client.state()) + " try again in 5 seconds");
        LED(ON);
        delay(5000);
    }
  } /* end while */
} /* end reconnect MQTT*/

/*---------------------------Debug function----------------------------------------*/
inline void DebugLog_Show(String content)
{
    #if DEBUG_LOG
        showln(content);
    #endif
}
