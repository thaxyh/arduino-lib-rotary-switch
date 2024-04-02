#ifndef __ROTARY_SWITCH_H
#define __ROTARY_SWITCH_H

class RotarySwitch
{
private:
    void setupPins();

    void (*_onCWSwitch)(void);
    void (*_onCCWSwitch)(void);
    void (*_onButtonPress)(void);
    void (*_onButtonRelease)(void);
    unsigned short _pinCLK;
    unsigned short _pinDT;
    unsigned short _pinSW;
    int valueCLK = -1;
    int valueDT = -1;
    int valueSW = -1;
    int previousValueCLK = -1;
    int previousValueDT = -1;
    int previousValueSW = -1;
    unsigned long lastMillisCLK = 0;
    unsigned long lastMillisDT = 0;
    unsigned long lastMillisSW = 0;
    int8_t rotationDirectionCounter = 0;
    bool buttonChanged = false;

public:
    RotarySwitch(unsigned short pinCLK, unsigned short pinDT, unsigned short pinSW);
    void begin();
    void attachOnCWSwitch(void (*func)(void));
    void attachOnCCWSwitch(void (*func)(void));
    void attachOnButtonPress(void (*func)(void));
    void attachOnButtonRelease(void (*func)(void));
    void onCLK();
    void onDT();
    void onSW();
    void update();
};

#endif
