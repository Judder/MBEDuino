//Multi-dimensional array test

void setup() {
  // put your setup code here, to run once:

//Values
char* mbeTitleValues[] = {"Title","legend","units","max","min","command","SamplingInterval"};
char* mbeSpeedValues[] = {"Speed","Speed","rpm","64000","0","C37C","10000"};
char* mbeBaroValues[] = {"Baro","Baro","mBar","1063","0","0050"};
char* mbeLambda1Values[] = {"Lambda1","Lambda1","V","2.50","0","0049"};
char* mbeLambda2Values[] = {"Lambda2","Lambda2","V","2.50","0","004A"};
char* mbeBatteryValues[] = {"Battery","Battery","V","20.00","0","00A7"};
char* mbeInj1Values[] = {"Inj 123","Inj 123","mS","131.072","0","C37E","10"};
char* mbeInj2Values[] = {"Inj 456","Inj 456","mS","131.072","0","C376","10"};
char* mbeIgnitionValues[] = {"Ignition","Ignition","Deg","0.0"," 60.0","87"};
char* mbeThrottle1Values[] = {"Throttle1","Throttle1","%","100.0","9.48","9C"};
char* mbeThrottle2Values[] = {"Throttle2","Throttle2","%","100.0","9.48","94"};
char* mbeWaterTempValues[] = {"WaterTemp","WaterTemp","C","130.0","-30","C0"};
char* mbeAirTempValues[] = {"AirTemp","AirTemp","C","130.0","-30","BF"};
char* mbeAdaptive1Values[] = {"Adaptive 1","Adaptive 1","%","100.0","-100.0","EF"};
char* mbeAdaptive2Values[] = {"Adaptive 2","Adaptive 2","%","100.0","-100.0","F1"};
char* mbeFulePumpValues[] = {"Fuel Pump","Fuel Pump","Bit","ON","OFF","0158"};
char* mbeRadFan1Values[] = {"Rad Fan1","Rad Fan1","Bit","ON","OFF","0858"};
char* mbeRadFan2Values[] = {"Rad Fan2","Rad Fan2","Bit","ON","OFF","0258"};

//Errors
char* mbeBatteryVoltageError[] = {"Battery Voltage","Battery Voltage","Bit","BAD","OK","02D3"};
char* mbeWaterTempError[] = {"Water Temp","Water Temp","Bit","BAD","OK","03D3"};
char* mbeThrottle2Error[] = {"Throttle 2","Throttle 2","Bit","BAD","OK","04D3"};
char* mbeThrottle1Error[] = {"Throttle 1","Throttle 1","Bit","BAD","OK","06D3"};
char* mbeAirTempError[] = {"Air Temp","Air Temp","Bit","BAD","OK","08D3"};
char* mbeBaroPressureError[] = {"Baro Pressure","Baro Pressure","Bit","BAD","OK","01D3"};
char* mbeLambda1Error[] = {"Lambda 1","Lambda 1","Bit","BAD","OK","05D3"};
char* mbeLambda2Error[] = {"Lambda 2","Lambda 2","Bit","BAD","OK","07D3"};
char* mbeCrankWiringError[] = {"Crank Wiring","Crank Wiring","Bit","Swapped","OK","05FE"};
char* mbeAFR1Error[] = {"AFR 123","AFR 123","Bit","BAD","OK","03FE"};
char* mbeAFR2Error[] = {"AFR 456","AFR 456","Bit","BAD","OK","04FE"};
char* mbeCrankSensorError[] = {"Crank Sensor","Crank Sensor","Bit","BAD","OK","02FE"};
char* mbeBatteryVoltageLogError[] = {"BatteryVoltageLog","BatteryVoltageLog","Bit","Log Fault","OK","02FF"};
char* mbeWaterTempLogError[] = {"WaterTemp Log","WaterTemp Log","Bit","Log Fault","OK","03FF"};
char* mbeThrottle2LogError[] = {"Throttle2 Log","Throttle2 Log","Bit","Log Fault","OK","04FF"};
char* mbeThrottle1LogError[] = {"Throttle1 Log","Throttle1 Log","Bit","Log Fault","OK","06FF"};
char* mbeAirTempLogError[] = {"Air TempLog","Air TempLog","Bit","Log Fault","OK","08FF"};
char* mbeBaroPressureLogError[] = {"BaroPressureLog","BaroPressureLog","Bit","Log Fault","OK","01FF"};
char* mbeLambda1LogError[] = {"Lambda1 Log","Lambda1 Log","Bit","Log Fault","OK","05FF"};
char* mbeLambda2LogError[] = {"Lambda2 Log","Lambda2 Log","Bit","Log Fault","OK","07FF"};

//Control
char* mbeLambda1Control[] = {"Lambda 123","Lambda 123","Bit","NO Control","OK","01CA"};
char* mbeLambda2Control[] = {"Lambda 456","Lambda 456","Bit","NO Control","OK","02CA"};
char* mbeFuelCutControl[] = {"Fuel Cut","Fuel Cut","Bit","NO Control","OK","03CA"};
char* mbeWaterTempControl[] = {"Water Temp.","Water Temp.","Bit","NO Control","OK","04CA"};
char* mbedThrottleControl[] = {"dThrottle","dThrottle","Bit","NO Control","OK","05CA"};
char* mbedSpeedControl[] = {"dSpeed","dSpeed","Bit","NO Control","OK","06CA"};
char* mbeWarmUpTimerControl[] = {"Warm Up Timer","Warm Up Timer","Bit","NO Control","OK","07CA"};
char* mbeLambdaControl[] = {"Lambda Control","Lambda Control","Bit","Disabled","OK","08CA"};

//S6 Only(?)
char* mbeOilPressureValues[] = {"OilPress","OilPress","psi","160","0","009A"};
char* mbeTime1Values[] = {"Time","Time","Secs","45813","0","FF02"};
char* mbeTime2Values[] = {"Time","Time","Secs","5726.6","0","FF03"};
char* mbeOverSpeedValues[] = {"OverSpeed","OverSpeed","RPM","4000","0","FF04"};
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
