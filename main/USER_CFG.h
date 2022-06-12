/* ---------------------------------------------------------------------------------------------- */
/*  File name:    def_cfg.h                                                                       */
/*  Description:  define macro                                                                    */
/*  Version:      1.0.0                                                                           */
/*  Date:         24/02/2022                                                                      */
/* ---------------------------------------------------------------------------------------------- */
#ifndef __USER_CFG_H_
#define __USER_CFG_H_

/* ---------------------------------------------------------------------------------------------- */
/*                                         include library                                        */
/* ---------------------------------------------------------------------------------------------- */

#include <stdint.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SD.h>
#include <ArduinoJson.h>         /* JSON version 6.19.0 by Beniot Blanchon */

/* ---------------------------------------------------------------------------------------------- */
/*                                     define position sensor                                     */
/* ---------------------------------------------------------------------------------------------- */

#define TEMP              0
#define SAL               1
#define PH                2
#define NH4               3
#define DO                4
#define NO3               5

/* ---------------------------------------------------------------------------------------------- */
/*                             define position parametter calibration                             */
/* ---------------------------------------------------------------------------------------------- */

#define SAL_Intercept     0
#define SAL_Slope         1
#define PH_Intercept      2
#define PH_Slope          3
#define NH4_Eo            4
#define NH4_m             5
#define DO_Intercept      6
#define DO_Slope          7

/* ---------------------------------------------------------------------------------------------- */
/*                               define position time in RTC Ds3231                               */
/* ---------------------------------------------------------------------------------------------- */

#define second            0
#define minute            1
#define hour              2
#define dayofweek         3
#define date              4
#define month             5
#define year              6

/* ---------------------------------------------------------------------------------------------- */
/*                               define position day of week                                      */
/* ---------------------------------------------------------------------------------------------- */

#define SUNDAY             1
#define MONDAY             2
#define TUESDAY            3
#define WEDNESDAY          4
#define THURSDAY           5
#define FRIDAY             6
#define SATURDAY           7

/* ---------------------------------------------------------------------------------------------- */
/*                                      define parametter ADC                                     */
/* ---------------------------------------------------------------------------------------------- */

#define VOLTAGE_REF                 (5.0)
#define NumberTimes_ReadAnalog      (3)

/* ---------------------------------------------------------------------------------------------- */
/*                                   define some information use                                  */
/* ---------------------------------------------------------------------------------------------- */

#define PIN_SD_CARD       (24)                                         // PIN CS of SD Card SPI
#define DS3231_ADDRESS    (0x68)                                       // Address i2c of slave RTC DS3231
#define EEPROM_ADDRESS    4095                                         // calib config number save in eeprom
#define UARTJSON_ANALOG   255                                          // analog 
#define UARTJSON_CALIB    254                                          // calibration 
#define NEWDAY            253                                          // Dataloger at NewDay
#define NORMALDAY         252                                          // Dataloger at normal day
#define NEWBOARD          251                                          // 99 is new board ---------------------------------
#define OLDBOARD          250                                          // 11 is old board ---------------------------------
#define RESETBOARD        230
#define NONE              0

#define JSON_ANAOG_DETAIL_byClient  244                                          // json client
#define JSON_CALIB_byClient         240                                          // json client
#define JSON_ANALOG_byClient        248                                          // json client
#define JSON_VALUE_SENSOR_byClient  247                                          // json client

#define JSON_TASK_ByServer          246                                          // json server
#define JSON_NUMERIC_ByServer       245                                          // json server
#define JSON_CALIB_ByServer         249                                          // json server
#define JSON_CALIB_Get              243                                          // json server

#define JSON_TASK_CALIB             249
#define JSON_TASK_VALUE             247
#define JSON_TASK_ANALOG_SINGLE     248
#define JSON_TASK_ANALOG_DETAL      0
#define JSON_TASK_ARDUINORESER      235

#define DEBUG_RTC         false
#define DEBUG_SENSOR      true                                         // debug for SHOW value sensor
#define DEBUG_LOG         true                                         // debug for SHOW Serial
#define DEBUG_ALL         true
#define DEBUG_EEP         false
#define DEBUG_JSON        false
#define DEBUG_SD          true
#define DEBUG_APP         false
#define SHOW              if(DEBUG_LOG) Serial.print
#define SHOWLN            if(DEBUG_LOG) Serial.println
#define ENDLN             if(DEBUG_LOG) Serial.println();
#define DOWLN             "\r\n"

#define FOLDER_CONFIG     "Config"
#define FOLDER_DATALOG    "DataLog"
#define FOLDER_AN_CALI    "An_Cali"
#define DIRECT_ANALOG     ""
#define DIRECT_CONFIGS    ""
#define DIRECT_DATALOG    ""

/* ---------------------------------------------------------------------------------------------- */
/*                                           define UART                                          */
/* ---------------------------------------------------------------------------------------------- */

#define BUADRATE1         9600
#define BUADRATE2         115200
#define ESP8266V1         Serial1
#define ESPD1MINI         Serial2

/* ---------------------------------------------------------------------------------------------- */
/*                                define information Wifi + Server                                */
/* ---------------------------------------------------------------------------------------------- */

#define _ssid             "CTU_IOTLAB"
#define _pass             "02710271"              // An Bien - Kien Giang
#define _ipserver         "IoTlab.net.vn"         // station test   - TayYenA       - NamThaiA      - DanhNa        - BaBien
#define _StationCode      "iepF8KwIKv"            // iepF8KwIKv       9ZI1L084I7      9eqA2hcmpZ      K6hgva7zJd      dkWSDD8pQ4
#define _StationSecret    "G0JmNawogG"            // G0JmNawogG       2cPnm9RWtQ      3iJQtNz1zB      qjKPbhgcMs      zHtrLheTmE
#define _ServerPort       "3000"

/* ---------------------------------------------------------------------------------------------- */
/*                                        define a new type                                       */
/* ---------------------------------------------------------------------------------------------- */

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;

enum FlagGlobal {TIM1_E = 1, ESPD1_1E, ESPD1_2E, ESPD1_3E, DataLog, JsonValue, JsonCalib, JsonAnalog};

inline void Debug_ShowStr_RTC(String str)
{
    #if DEBUG_RTC
        SHOW(str);
    #endif
}

inline void Debug_ShowStr_Sensor(String str)
{
    #if DEBUG_SENSOR
        Serial.print(str);
    #endif
}

inline void Debug_ShowStr_Json(String str)
{
    #if DEBUG_JSON
        SHOW(str);
    #endif
}

inline void Debug_ShowStr_EEP(String str)
{
    #if DEBUG_EPP
        SHOW(str);
    #endif
}

inline void Debug_ShowStr_SD(String str)
{
    #if DEBUG_SD
        SHOW(str);
    #endif
}

inline void Debug_ShowStr_APP(String str)
{
    #if DEBUG_APP
        SHOW(str);
    #endif
}

#endif
