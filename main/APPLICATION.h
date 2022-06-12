#include <avr/wdt.h>                /* standard library */
#include "RTC_DS3231.h"             /* lib user */
#include "JSON.h"                   /* lib user */
#include "EEP.h"                    /* lib user */
#include "SENSOR.h"                 /* lib user */
#include "SD_CARD.h"                /* lib user */

uint8  FlagState[10];
uint16 TimeLine = 0;
uint16 RTC_Time[8];

// prototype function
void WDT_ON();
void WDT_OFF();
void TIMER1_Init();
void Calibration_Init_void();
void DataLoger2SDCard(uint8);
void Json_Parse_void(char DataParse[], uint8 FlagReset);
void TIM1_POLL_void(uint8, uint8, uint8, uint8, uint8);

// application function
void WDT_ON()
{
    cli();
    wdt_reset();                                      // reset wdt to set prescaler
    /* enable WDT ON 8seconds */
    WDTCSR |= (1 << WDCE) | (1 << WDE);               // enable watchdog system reset
    WDTCSR  = (1 << WDE) | (1 << WDP3) | (1 << WDP0); // presceler 1024k 8s vcc=5.0
    sei();
}

void WDT_OFF()
{
    cli();
    wdt_reset();
    MCUSR &= ~(1 << WDRF);
    WDTCSR = (1 << WDCE) | (1 << WDE);
    /* Turn off WDT */
    WDTCSR = 0x00;
    sei();
}

void TIMER1_Init()
{
    cli();                        // disable all interrupts
    // mode normal is set
    TCCR1A = 0x00;                // WGM11 = 0; WGM10 = 0;
    TCCR1B = 0x00;                // WGM12 = 0; WGM13 = 0;
    /* Timer/Counter Register. The actual timer value is stored here. */
    TCNT1 = 3035;                 // preload timer 65536-16MHz/256/1Hz <=> (65535 - 1s.16Mhz/256)
    TCCR1B |= (1 << CS12);        // 256 prescaler
    TIMSK1 |= (1 << TOIE1);       // enable timer overflow interrupt
    sei();                        // enable all interrupts
}


void DataLoger2SDCard(uint8 WhData)
{
    String  TimeSave   = RTC_DS3231_ReadTime_String(RTC_Time);
    String  DirFolder  = "DATALOG/YEAR" + (String)RTC_Time[year] + "/MONTH_" + (String)RTC_Time[month];
    String  SaveFile   = DirFolder + "/DATE_" + (String)RTC_Time[date] + ".txt";
    String  Title      = "Temperature,Salinity,PH,NH4,DO\r\n";
    String  comma      = ",";
    String  DataSave   = Value_Sensor[TEMP] + comma + Value_Sensor[SAL] + comma + Value_Sensor[PH] + comma + Value_Sensor[NH4] + comma + Value_Sensor[DO] + comma + TimeSave + DOWLN;
    #if DEBUG_APP
    SHOWLN("DATALOGER SAVE AT: " + SaveFile);
    #endif

    String DataSave2SD = "";
    if (WhData == NEWDAY)
        DataSave2SD = Title;
    if (WhData == NORMALDAY)
        DataSave2SD = DataSave;
    if (!SD.exists(DirFolder))
        SDCARD_CreateFolder_bool(DirFolder);
    SDCARD_WriteFile_String(SaveFile, DataSave2SD);
}

void Json_Parse_void(char DataParse[], uint8 FlagReset)
{
    String output;
    StaticJsonDocument <256> doc;
    deserializeJson(doc, DataParse);

    uint8 JsonTask = doc["task"];

    if(JsonTask == JSON_NUMERIC_ByServer)
    {
        uint8_t numeric = doc["numeric"];
        SHOWLN("Numeric configuration is: " + (String)numeric);
        String DataRead = SDCARD_ReadFileCfg_String(numeric);
        Debug_ShowStr_APP("Data >>> " + DataRead + DOWLN);
        ESPD1MINI.print(DataRead);       
    }
    else if(JsonTask == JSON_CALIB_ByServer)
    {
        uint8 NumCfg               = doc["config"];
        Calibration[SAL_Intercept] = doc["SAL"][0];
        Calibration[SAL_Slope]     = doc["SAL"][1];
        Calibration[PH_Intercept]  = doc["PH"][0];
        Calibration[PH_Slope]      = doc["PH"][1];
        Calibration[NH4_Eo]        = doc["NH4"][0];
        Calibration[NH4_m]         = doc["NH4"][1];
        Calibration[DO_Intercept]  = doc["OXI"][0];
        Calibration[DO_Slope]      = doc["OXI"][1];

        if(FlagReset == RESETBOARD)
        {
            TIMSK1 &= ~(1 << TOIE1);
            EEPROM_Write_void(EEPROM_ADDRESS, NumCfg);
            serializeJson(doc, output);
            SDCARD_SaveConfigFile_uint8(NumCfg, output);
            SHOWLN(F("Reset to apply new Calibration (8s)...."));
            delay(10000);
        }
    }
    else if(JsonTask == JSON_TASK_ARDUINORESER)
    {
        SHOWLN("Arduino will reset after 10s");
        TIMSK1 &= ~(1 << TOIE1);
        delay(10000);
    }
    #if DEBUG_APP
        serializeJson(doc, output);
        Serial.println("output: " + output);
    #endif
}

void Calibration_Init_void()
{
    char   CData[256];
    uint8  conf = EEPROM_Read_int8(EEPROM_ADDRESS);
    String Data = SDCARD_ReadFileCfg_String(conf);
    ESPD1MINI.print(Data);
    
    for(int i = 0; i<= 255; i++)
        CData[i] =  Data[i];
    
    Json_Parse_void(CData, NONE);
    if (conf <= 5)
        SHOWLN("(Calibration now) SAL_Intercept: " + (String)Calibration[SAL_Intercept] + " SAL_Slope:" + (String)Calibration[SAL_Slope] + " PH_Intercept:" + (String)Calibration[PH_Intercept] + " PH_Slope:" + (String)Calibration[PH_Slope] + " Eo:" + (String)Calibration[NH4_Eo] + " m:" + (String)Calibration[NH4_m] + " DO_Intercept:" + (String)Calibration[DO_Intercept] + " DO_Slope:" + (String)Calibration[DO_Slope]);
    else
        SHOWLN("(Use calib default) SAL_Intercept: " + (String)Calibration[SAL_Intercept] + " SAL_Slope:" + (String)Calibration[SAL_Slope] + " PH_Intercept:" + (String)Calibration[PH_Intercept] + " PH_Slope:" + (String)Calibration[PH_Slope] + " Eo:" + (String)Calibration[NH4_Eo] + " m:" + (String)Calibration[NH4_m] + " DO_Intercept:" + (String)Calibration[DO_Intercept] + " DO_Slope:" + (String)Calibration[DO_Slope]);
}

void TIM1_POLL_void(uint8 WarmUpTEMP, uint8 WarmUpSAL, uint8 WarmUpPH, uint8 WarmUpNH4, uint8 WarmUpDO)
{

    uint16 TaskTEMP = 0;                                                            // if(30s) 0   -  39s      0 + time = end
    uint16 TaskSAL  = WarmUpTEMP + 10;                                              // if(30s) 40  -  79s
    uint16 TaskPH   = WarmUpSAL  + WarmUpTEMP + 20;                                 // if(30s) 80  -  119s
    uint16 TaskNH4  = WarmUpPH   + WarmUpSAL  + WarmUpTEMP + 30;                    // if(30s) 120 -  159s
    uint16 TaskDO   = WarmUpNH4  + WarmUpPH   + WarmUpSAL  + WarmUpTEMP + 40;       // if(60s) 160 -  229s
    uint16 TimeWait = (TaskDO + WarmUpDO + 10 + 6);
    if(TimeLine == 1)
        Serial.println("TimeStart(" + (String)"TEMP:" + (String)(TaskTEMP+10) + " SAL:"+(String)TaskSAL+ " PH:"+(String)TaskPH +" NH4:"+(String)TaskNH4 +" DO:"+ TaskDO +" END:" +(String)(TimeWait) + ")");
    
    Sensor_ReadValue_void(TEMP, TaskTEMP, WarmUpTEMP, TimeLine);
    Sensor_ReadValue_void(SAL, TaskSAL, WarmUpSAL, TimeLine);
    Sensor_ReadValue_void(PH, TaskPH, WarmUpPH, TimeLine);
    Sensor_ReadValue_void(NH4, TaskNH4, WarmUpNH4, TimeLine);
    Sensor_ReadValue_void(DO, TaskDO, WarmUpDO, TimeLine);

    if((TimeLine - TimeWait) == 120)
        TimeLine = 0;

    else if((TimeLine - TimeWait) == 5)
        FlagState[DataLog] = true;
    else if((TimeLine - TimeWait) == 10)
        FlagState[JsonAnalog] = true;    
    else if((TimeLine - TimeWait) == 15)
        FlagState[JsonValue] = true;
}

inline void CharArr_Clear(char Data[], uint16 NumArrClr)
{
    while(NumArrClr--)
        Data[NumArrClr] = 0;
}
