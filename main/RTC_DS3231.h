/* ---------------------------------------------------------------------------------------------- */
/*  File name:    rtc_ds3231.h                                                                    */
/*  Description:  Declare functions                                                               */
/*  Version:      1.0.0                                                                           */
/*  Date:         24/02/2022                                                                      */
/* ---------------------------------------------------------------------------------------------- */
#ifndef __RTC_DS3231_H_
#define __RTC_DS3231_H_

/* ---------------------------------------------------------------------------------------------- */
/*                                         include Library                                        */
/* ---------------------------------------------------------------------------------------------- */
#include "user_cfg.h"

/* ---------------------------------------------------------------------------------------------- */
/*                                       prototype function                                       */
/* ---------------------------------------------------------------------------------------------- */
String RTC_DS3231_DayOfWeedF_String(uint8 num);
String RTC_DS3231_ReadTime_String(uint16 *RTC);
void RTC_DS3231_SetTime_void(uint8 hourW, uint8 minuteW, uint8 secondW, uint8 dayofweekW, uint8 dateW, uint8 monthW, uint8 yearW);
uint8 D2B(uint8 num);
uint8 B2D(uint8 num);
inline void RTC_Init();

/*******************************************************************************
* @FunctionName:    RTC_DS3231_DayOfWeedF_String                                            
* @Description:     identify monday to sunday    
* @Param:           uint8 num
* @Return:          String                                                       
*******************************************************************************/
String RTC_DS3231_DayOfWeedF_String(uint8 num)
{
  String DayOfWeek = "";
  switch (num)
  {
    case 1:  return "sunday";
    case 2:  return "monday";
    case 3:  return "tueday";
    case 4:  return "wednesday";
    case 5:  return "thurday";
    case 6:  return "friday";
    case 7:  return "saturday";
    default: return "undefine";
  }
  return DayOfWeek;
}

/*******************************************************************************
* @FunctionName:    RTC_DS3231_ReadTime_String                                            
* @Description:     read time from RTC DS3231   
* @Param:           uint16 *ds3231
* @Return:          String                                                       
*******************************************************************************/
String RTC_DS3231_ReadTime_String(uint16 *RTC)
{
    String Dayofweekchange = "";
    String TimeSave= "";

    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDRESS, 7);

    RTC[second]    = B2D(Wire.read() & 0x7F);
    RTC[minute]    = B2D(Wire.read());
    RTC[hour]      = B2D(Wire.read() & 0x3F);
    RTC[dayofweek] = B2D(Wire.read());
    RTC[date]      = B2D(Wire.read());
    RTC[month]     = B2D(Wire.read());
    RTC[year]      = B2D(Wire.read()) + 2000;

    Dayofweekchange = RTC_DS3231_DayOfWeedF_String(RTC[dayofweek]);
    TimeSave =  RTC[hour]    + (String)"h:";
    TimeSave += RTC[minute]  + (String)"m:";
    TimeSave += RTC[second]  + (String)"s - ";
    TimeSave += Dayofweekchange + (String)" - ";
    TimeSave += RTC[date]    + (String)"/";
    TimeSave += RTC[month]   + (String)"/";
    TimeSave += RTC[year];
    Debug_ShowStr_RTC("RTC: " + TimeSave + DOWLN);
    return TimeSave;
}

/*******************************************************************************
* @FunctionName:    RTC_DS3231_SetTime_void                                            
* @Description:     set time for RTC DS3231   
* @Param:           uint8 hourW, uint8 minuteW, uint8 secondW, uint8 dayofweekW, uint8 dateW, uint8 monthW, uint8 yearW
* @Return:          void                                                       
*******************************************************************************/
void RTC_DS3231_SetTime_void(uint8 hourW, uint8 minuteW, uint8 secondW, uint8 dayofweekW, uint8 dateW, uint8 monthW, uint8 yearW)
{
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write(0x00);
  Wire.write( D2B(secondW)    );
  Wire.write( D2B(minuteW)    );
  Wire.write( D2B(hourW)      );
  Wire.write( D2B(dayofweekW) );
  Wire.write( D2B(dateW)      );
  Wire.write( D2B(monthW)     );
  Wire.write( D2B(yearW)      );
  Wire.endTransmission();
}

/*******************************************************************************
* @FunctionName:    D2B                                            
* @Description:     convert Decimal to BCD  
* @Param:           uint8 num
* @Return:          uint8                                                       
*******************************************************************************/
uint8 D2B(uint8 num) 
{
  return ( (num / 10 * 16) + (num % 10) );
}

/*******************************************************************************
* @FunctionName:    B2D                                            
* @Description:     convert BCD to Decimal
* @Param:           uint8 num
* @Return:          uint8                                                       
*******************************************************************************/
uint8 B2D(uint8 num) 
{
  return ( (num / 16 * 10) + (num % 16) );
}

/*******************************************************************************
* @FunctionName:    RTC_Init                                            
* @Description:     init I2C
* @Param:           none
* @Return:          none                                                       
*******************************************************************************/
inline void RTC_Init()
{
    Wire.begin();
}


#endif
