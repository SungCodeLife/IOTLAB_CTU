/* ---------------------------------------------------------------------------------------------- */
/*  File name:    eeprom_user.h                                                                   */
/*  Description:  functions                                                               */
/*  Version:      1.0.0                                                                           */
/*  Date:         24/02/2022                                                                      */
/* ---------------------------------------------------------------------------------------------- */
#ifndef __EEP_H_
#define __EEP_H_

/* ---------------------------------------------------------------------------------------------- */
/*                                         include library                                        */
/* ---------------------------------------------------------------------------------------------- */
#include "user_cfg.h"

extern boolean  ON;
extern boolean  OFF;
/* ---------------------------------------------------------------------------------------------- */
/*                                       prototype function                                       */
/* ---------------------------------------------------------------------------------------------- */
int8 EEPROM_Read_int8(uint8 address);
void EEPROM_Write_void(uint16 address, uint8 SaveValue);
uint8 EEPROM_ReadCfg_uint8();

int8 EEPROM_Read_int8(int address)
{
    int8 value = EEPROM.read(address);
    delay(10);
    Debug_ShowStr_EEP("EEPROM _ value read: " + (String)value + DOWLN);
    return value;
}

void EEPROM_Write_void(uint16 address, uint8 SaveValue)
{
    /* An EEPROM write takes 3.3 ms to complete - The EEPROM memory has a specified life of 100,000 write/erase cycles */
    uint8 conditionLoop = 5; 
    EEPROM.write(address, SaveValue);
    delay(10);
    while(EEPROM.read(address) != SaveValue)
    {
        if(--conditionLoop)
        break;
        delay(5);
        EEPROM.write(address, SaveValue);
    }
    Debug_ShowStr_EEP("EEPROM SAVE VALUE : "+ String(SaveValue) + DOWLN);
    delay(5);
}

/* read config from eeprom 0 - 5 at address 4095 */
uint8 EEPROM_ReadCfg_uint8()
{
    delay(10);
    uint8 conf = EEPROM_Read_int8(EEPROM_ADDRESS);
    if (conf >= 0 && conf < 6)
    {
        Debug_ShowStr_EEP("EEPROM _ Calibration config is: " + (String)conf + DOWLN);
    }
    else
    {
        Debug_ShowStr_EEP("EEPROM _ value config read from EEPROM is not suitable");
    }

    return conf;
}


#endif
