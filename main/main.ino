#include "APPLICATION.h"

// variable declare
uint32 RTC1second = 0;
char  Data1[254];
char  Data2[254];
char  Data3[254];


// main function
void setup()
{
    Serial.begin(BUADRATE2);
    ESPD1MINI.begin(BUADRATE2);
    Board_Init_void(OLDBOARD);
    Board_SetPinOut_void(OFF);
    RTC_Init();
    // RTC_DS3231_SetTime_void(14, 9, 30, TUESDAY, 17, 5, 22);            //14h9p30s - tuesday - 17/05/2022
    SDCARD_init_void();
    Calibration_Init_void();
    CharArr_Clear(Data1, 254);
    
    WDT_ON();
    TIMER1_Init();
}

void loop()
{
    wdt_reset();
    if(millis() - RTC1second >= 999)
    {
        RTC_DS3231_ReadTime_String(RTC_Time);
       if(RTC_Time[hour] == 0 && RTC_Time[minute] == 0 && RTC_Time[second] == 0)
       {
           delay(999);
           DataLoger2SDCard(NEWDAY);
       }
        RTC1second = millis();
    }

    if(FlagState[TIM1_E])
    {
        TIM1_POLL_void(10, 10, 10, 10, 10);
        FlagState[TIM1_E] = false;
    }

    if(FlagState[ESPD1_1E])
    {
        Json_Parse_void(Data1, RESETBOARD);
        CharArr_Clear(Data1, 254);
        FlagState[ESPD1_1E] = false;
    }
    else if(FlagState[ESPD1_2E])
    {
        Json_Parse_void(Data2, RESETBOARD);
        CharArr_Clear(Data2, 254);
        FlagState[ESPD1_2E] = false;
    }
    else if(FlagState[ESPD1_3E])
    {
        Json_Parse_void(Data3, RESETBOARD);
        CharArr_Clear(Data3, 254);
        FlagState[ESPD1_3E] = false;
    }

    if(FlagState[DataLog])
    {
        DataLoger2SDCard(NORMALDAY);
        FlagState[DataLog] = false;
    }

    if(FlagState[JsonValue])
    {
        String JsonValueString = Json_CreateValue_String(Value_Sensor);
        ESPD1MINI.println(JsonValueString);
        FlagState[JsonValue] = false;
    }

    if(FlagState[JsonAnalog])
    {
        String JsonAnalogString = Json_CreateAnalog_String(JSON_ANALOG_byClient, Analog_Read);
        ESPD1MINI.print(JsonAnalogString);
        FlagState[JsonAnalog] = false;
    }
}

// interrupt function
ISR(TIMER1_OVF_vect)
{
    TCNT1 = 3035;
    FlagState[TIM1_E] = true;
    ++TimeLine;
}

void serialEvent2()
{
    String DataRev  = "";
    while (ESPD1MINI.available())
        DataRev = ESPD1MINI.readString();
    if(!FlagState[ESPD1_1E])
    {
        for(uint8 ElementCount = 0; ElementCount <= DataRev.length()+1; ElementCount++)
            Data1[ElementCount] = (uint8)DataRev[ElementCount];
        FlagState[ESPD1_1E] = true;
    }
    else if(!FlagState[ESPD1_2E])
    {
        for(uint8 ElementCount = 0; ElementCount <= DataRev.length()+1; ElementCount++)
            Data2[ElementCount] = (uint8)DataRev[ElementCount];
        FlagState[ESPD1_2E] = true;
    }
    else if(!FlagState[ESPD1_3E])
    {
        for(uint8 ElementCount = 0; ElementCount <= DataRev.length()+1; ElementCount++)
            Data3[ElementCount] = (uint8)DataRev[ElementCount];
        FlagState[ESPD1_3E] = true;
    }

    SHOW(F("NEW Data receive from ESP8266D1 mini: "));  SHOW((String)DataRev + DOWLN);
}
