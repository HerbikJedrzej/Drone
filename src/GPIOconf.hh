#ifndef GPIOCONF_H
#define GPIOCONF_H

enum OutputList{
    none = -1,
    LED_R = 0,
    LED_G1,
    LED_G2,
    LED_Y1,
    LED_Y2,
    LED_B,
    LED_Y,
    LED_R1,
    LED_EXCEPTION_THROWED,
    LED_LAST_LED,
    TRIG = 36,
    SensorSS = 37,
    MemoryWriteProtect = 40,
    RadioCSN = 45,
    RadioCE,
};

enum InputList{
};

enum InterruptInputList{
  RadioIRQ = 0,
  ECHO,
  endOfInterruptInputList
};

enum ADClist{
	batteryCell1,
	batteryCell2,
	batteryCell3,
	temperature,  
  end
};

#endif
