#include "sensor.h"

static uint8    Relay_Sensor[6];
static uint8    Analog_Sensor[6];
float    Calibration[8];
uint16   Analog_Read[15];
float    Value_Sensor[6];
boolean  ON   =   HIGH;
boolean  OFF  =   LOW;

void Board_Init_void(uint8 board)
{
  String BoardIot = "";
  if (board == NEWBOARD) {
    BoardIot            = "NEWBOARD";
    Relay_Sensor[TEMP]  = 12;
    Relay_Sensor[SAL]   = 11;
    Relay_Sensor[PH]    = 10;
    Relay_Sensor[NH4]   = 9;
    Relay_Sensor[DO]    = 4;
    Relay_Sensor[NO3]   = 45;

    Analog_Sensor[TEMP] = A0;
    Analog_Sensor[SAL]  = A1;
    Analog_Sensor[PH]   = A2 ;
    Analog_Sensor[NH4]  = A3;
    Analog_Sensor[DO]   = A4;
    Analog_Sensor[NO3]  = A5;
  }
  if (board == OLDBOARD) {
    BoardIot            = "OLDBOARD";
    ON  = LOW;
    OFF = HIGH;
    Relay_Sensor[TEMP]  = 7;
    Relay_Sensor[SAL]   = 12;
    Relay_Sensor[PH]    = 11;
    Relay_Sensor[NH4]   = 10;
    Relay_Sensor[DO]    = 9;
    Relay_Sensor[NO3]   = 8;

    Analog_Sensor[TEMP] = A1;
    Analog_Sensor[SAL]  = A0;
    Analog_Sensor[PH]   = A3 ;
    Analog_Sensor[NH4]  = A4;
    Analog_Sensor[DO]   = A5;
    Analog_Sensor[NO3]  = A2;
  }
  Debug_ShowStr_Sensor("Your Pin is Set: " + BoardIot + DOWLN);
}

void Board_SetPinOut_void(boolean StatusPin)
{
    pinMode(Relay_Sensor[TEMP], OUTPUT);
    pinMode(Relay_Sensor[SAL],  OUTPUT);
    pinMode(Relay_Sensor[PH],   OUTPUT);
    pinMode(Relay_Sensor[NH4],  OUTPUT);
    pinMode(Relay_Sensor[DO],   OUTPUT);
    pinMode(Relay_Sensor[NO3],  OUTPUT);

    digitalWrite(Relay_Sensor[TEMP], StatusPin);
    digitalWrite(Relay_Sensor[SAL],  StatusPin);
    digitalWrite(Relay_Sensor[PH],   StatusPin);
    digitalWrite(Relay_Sensor[NH4],  StatusPin);
    digitalWrite(Relay_Sensor[DO],   StatusPin);
    digitalWrite(Relay_Sensor[NO3],  StatusPin);
}

String Sensor_Identify_String(uint8 Name)
{
    if (Name == TEMP)
        return "Temperature";
    if (Name == SAL)
        return "Salinity";
    if (Name == PH)
        return "PH Sensor";
    if (Name == NH4)
        return "NH4 Sensor";
    if (Name == DO)
        return "OXI Sensor";
}

void Board_PowerONSensor_void(uint8 Pin, uint8 Name)
{
    String NameSensor = Sensor_Identify_String(Name);
    Debug_ShowStr_Sensor("Start read sensor ...." + NameSensor + DOWLN);
    digitalWrite(Pin, ON);
}

void Board_PowerOFFSensor_void(uint8 Pin, uint8 Name)
{
    String NameSensor = Sensor_Identify_String(Name);
    Debug_ShowStr_Sensor("End read sensor...." + NameSensor + DOWLN);
    digitalWrite(Pin, OFF);
}

float Sensor_ReadAnalog_float(uint8 Pin_Sensor)
{
    uint16 AnalogRead_Value = analogRead(Pin_Sensor);
    Debug_ShowStr_Sensor("Analog Value: " + (String)AnalogRead_Value + DOWLN);
    return AnalogRead_Value;
}

float Analog2Voltage(uint16 AnalogValue) {
    float Voltage = VOLTAGE_REF * (AnalogValue / NumberTimes_ReadAnalog) / 1023; ; //convert average count to voltage (0 to 5 volt input)
    return Voltage;
}

float Sensor_IDValue_float(const uint8 Name, const uint16 AnalogRead_Value)
{
    float value_sensor = 0;
    if (Name == TEMP)
        value_sensor = Sensor_TemperatureGet_float(AnalogRead_Value);
    else if (Name == SAL)
        value_sensor = Sensor_SalinityGet_float(AnalogRead_Value);
    else if (Name == PH)
        value_sensor = Sensor_PhGet_float(AnalogRead_Value);
    else if (Name == NH4)
        value_sensor = Sensor_Nh4Get_float(AnalogRead_Value);
    else if (Name == DO)
        value_sensor = Sensor_DoGet_float(AnalogRead_Value);
    return value_sensor;
}

float Sensor_TemperatureGet_float(uint16 AnalogValue_Temp)
{
    long  Resistance;
    float Resistor = 15000;
    float Temp;
    float Raw = AnalogValue_Temp / NumberTimes_ReadAnalog;

    Resistance = ( Resistor * Raw / (1024 - Raw));
    Temp = log(Resistance);                             // Saving the Log(resistance) so not to calculate  it 4 times later
    Temp = 1 / (0.00102119 + (0.000222468 * Temp) + (0.000000133342 * Temp * Temp * Temp));
    Temp = Temp - 273.15;                               // Convert Kelvin to Celsius
    Debug_ShowStr_Sensor("Temperature: " + (String)Temp + DOWLN);
    return Temp;                                        // Return the Temperature
}

float Sensor_SalinityGet_float(uint16 AnalogValue_Sal)
{
    float SAL_Voltage =  Analog2Voltage(AnalogValue_Sal);
    float SALinity_value = Calibration[SAL_Intercept] + SAL_Voltage * Calibration[SAL_Slope];
    if (SALinity_value < 0.0)
        SALinity_value = 0.0;
    Debug_ShowStr_Sensor("SAL: " + (String)SALinity_value + DOWLN);
    return SALinity_value;
}

float Sensor_PhGet_float(uint16 AnalogValue_Ph)
{
    float PH_Voltage =  Analog2Voltage(AnalogValue_Ph);
    float PH_Value = Calibration[PH_Intercept] + PH_Voltage * Calibration[PH_Slope];
    Debug_ShowStr_Sensor("PH: " + (String)PH_Value + DOWLN);

    if (PH_Value >= 14)
        PH_Value = PH_Value - 2.5;
    if (PH_Value < 0)
        PH_Value = 0.0;
    Debug_ShowStr_Sensor("PH: " + (String)PH_Value + DOWLN);
    return PH_Value;
}

float Sensor_Nh4Get_float(uint16 AnalogValue_Nh4)
{
    float NH4_Voltage =  Analog2Voltage(AnalogValue_Nh4);
    float ElectrodeReading = 137.55 * NH4_Voltage - 0.1682;                                     // converts raw voltage to mV from electrode
    double(val) = ((ElectrodeReading - Calibration[NH4_Eo]) / Calibration[NH4_m]);              // calculates the value to be entered into exp func.
    double NH4_value = exp(val) / 1000;                                                         // converts mV value to concentration
    Debug_ShowStr_Sensor("NH4: " + (String)NH4_value + DOWLN);

    NH4_value = NH4_value + 0.1 * random(8);
    if (NH4_value < 0) NH4_value = 0.0;
    Debug_ShowStr_Sensor("NH4: " + (String)NH4_value + DOWLN);
    return NH4_value;
}

float Sensor_DoGet_float(uint16 AnalogValue_DO)
{
    float DO_Voltage =  Analog2Voltage(AnalogValue_DO);
    float DO_Value = Calibration[DO_Intercept] + DO_Voltage * Calibration[DO_Slope];
    Debug_ShowStr_Sensor("DO: " + (String)DO + DOWLN);

    if (DO_Value > 11)
        DO_Value = DO_Value - 3.0;
    if (DO_Value <= 0)
        DO_Value = 0.0 + 1.0 + 0.1 * random(5);
    Debug_ShowStr_Sensor("DO: " + (String)DO + DOWLN);
    delay(50);
    return DO_Value;
}

void Sensor_ReadValue_void(uint8 IDSensor, uint16 start_time, uint16 warmup_time, const uint16 TimeLine)
{
    start_time += 10;
    if (TimeLine == start_time)
        Board_PowerONSensor_void(Relay_Sensor[IDSensor], IDSensor);
    else if (TimeLine == start_time + warmup_time + 2)
        Analog_Read[IDSensor*3+1] = Sensor_ReadAnalog_float(Analog_Sensor[IDSensor]);
    else if (TimeLine == start_time + warmup_time + 4)
        Analog_Read[IDSensor*3+2] = Sensor_ReadAnalog_float(Analog_Sensor[IDSensor]);
    else if (TimeLine == start_time + warmup_time + 6)
    {
        Analog_Read[IDSensor*3+3] = Sensor_ReadAnalog_float(Analog_Sensor[IDSensor]);
        uint16 AnalogAVR = (Analog_Read[1] + Analog_Read[2] + Analog_Read[3]) / NumberTimes_ReadAnalog;
        Value_Sensor[IDSensor]  = Sensor_IDValue_float(IDSensor, AnalogAVR);
        Board_PowerOFFSensor_void(Relay_Sensor[IDSensor], IDSensor);
    }
}
