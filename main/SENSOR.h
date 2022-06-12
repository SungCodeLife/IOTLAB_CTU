
/* ---------------------------------------------------------------------------------------------- */
/*  File name:    sensor.h                                                                        */
/*  Description:  Declare functions                                                               */
/*  Version:      1.0.0                                                                           */
/*  Date:         24/02/2022                                                                      */
/* ---------------------------------------------------------------------------------------------- */
#ifndef __SENSOR_H_
#define __SENSOR_H_

/* ---------------------------------------------------------------------------------------------- */
/*  Inclusion                                                                                     */
/* ---------------------------------------------------------------------------------------------- */

#include "user_cfg.h"

extern float    Value_Sensor[6];
extern float    Calibration[8];
extern uint16   Analog_Read[15];

/*******************************************************************************
* @FunctionName:    Board_Init_void                                            
* @Description:     identify oldboard hay newboard to configuration pin        
* @Param:           uint8 board, uint8 *Relay_Sensor, uint16 *Analog_Sensor    
* @Return:          Void                                                       
*******************************************************************************/
void Board_Init_void(uint8 board);

/*******************************************************************************
* @FunctionName:    Board_SetPinOut_void                                           
* @Description:     turn off or turn on relay pin init     
* @Param:           boolean StatusPin, uint8 *Relay_Sensor    
* @Return:          Void                                                       
*******************************************************************************/
void Board_SetPinOut_void(boolean StatusPin);

/*******************************************************************************
* @FunctionName:    Board_PowerONSensor_void                                           
* @Description:     turn on relay of sensor + show name sensor     
* @Param:           uint8 Pin, uint8 Name    
* @Return:          Void                                                       
*******************************************************************************/
void Board_PowerONSensor_void(uint8 Pin, uint8 Name);

/*******************************************************************************
* @FunctionName:    Board_PowerOFFSensor_void                                           
* @Description:     turn off relay of sensor + show name sensor     
* @Param:           uint8 Pin, uint8 Name    
* @Return:          Void                                                       
*******************************************************************************/
void Board_PowerOFFSensor_void(uint8 Pin, uint8 Name);

/*******************************************************************************
* @FunctionName:    Sensor_Identify_String                                           
* @Description:     identify sensor and return name String sensor    
* @Param:           uint8 Name    
* @Return:          String                                                       
*******************************************************************************/
String Sensor_Identify_String(uint8 Name);

/*******************************************************************************
* @FunctionName:    Sensor_ReadAnalog_float                                           
* @Description:     read and show analogread     
* @Param:           uint8 Pin_Sensor, uint16 *Analog_Averager    
* @Return:          float                                                       
*******************************************************************************/
float Sensor_ReadAnalog_float(uint8 Pin_Sensor, uint16 *Analog_Averager);

/*******************************************************************************
* @FunctionName:    Sensor_IDValue_float                                           
* @Description:     identify sensor and read value sensor     
* @Param:           uint8 Name    
* @Return:          float                                                       
*******************************************************************************/
float Sensor_IDValue_float(uint8 Name);

/*******************************************************************************
* @FunctionName:    Sensor_TemperatureGet_float                                           
* @Description:     read Temperature sensor     
* @Param:           uint16 AnalogValue_Temp    
* @Return:          float                                                       
*******************************************************************************/
float Sensor_TemperatureGet_float(uint16 AnalogValue_Temp);

/*******************************************************************************
* @FunctionName:    Sensor_SalinityGet_float                                           
* @Description:     read sensor Salinity with parametter calibration     
* @Param:           uint16 AnalogValue_Sal, float *Calib    
* @Return:          float                                                       
*******************************************************************************/
float Sensor_SalinityGet_float(uint16 AnalogValue_Sal);

/*******************************************************************************
* @FunctionName:    Sensor_PhGet_float                                           
* @Description:     read sensor PH with parametter calibration     
* @Param:           uint16 AnalogValue_Ph, float *Calib    
* @Return:          float                                                       
*******************************************************************************/
float Sensor_PhGet_float(uint16 AnalogValue_Ph);

/*******************************************************************************
* @FunctionName:    Sensor_Nh4Get_float                                           
* @Description:     read sensor NH4 with parametter calibration     
* @Param:           uint16 AnalogValue_Nh4, float *Calib   
* @Return:          float                                                       
*******************************************************************************/
float Sensor_Nh4Get_float(uint16 AnalogValue_Nh4);

/*******************************************************************************
* @FunctionName:    Sensor_DoGet_float                                           
* @Description:     read sensor DO with parametter calibration     
* @Param:           uint16 AnalogValue_DO, float *Calib   
* @Return:          float                                                       
*******************************************************************************/
float Sensor_DoGet_float(uint16 AnalogValue_DO);

/*******************************************************************************
* @FunctionName:    Sensor_ReadValue_void                                           
* @Description:     read sensor with time warmup and time of start     
* @Param:           uint8 IDSensor, uint16 start_time, uint16 warmup_time, uint8 *Relay_Sensor, uint16 *Analog_Sensor, float *Analog_Get, float *Value_Sensor, float *Calib, uint16 const TimeLine   
* @Return:          void                                                       
*******************************************************************************/
void Sensor_ReadValue_void(uint8 IDSensor, uint16 start_time, uint16 warmup_time, const uint16 TimeLine);

/*******************************************************************************
* @FunctionName:    TASK_TIM1_void                                           
* @Description:     read all of sensor with time warmup     
* @Param:           uint8 WarmUpTEMP, uint8 WarmUpSAL, uint8 WarmUpPH, uint8 WarmUpNH4, uint8 WarmUpDO   
* @Return:          void                                                       
*******************************************************************************/
void TASK_TIM1_void(uint8 WarmUpTEMP, uint8 WarmUpSAL, uint8 WarmUpPH, uint8 WarmUpNH4, uint8 WarmUpDO);


#endif
