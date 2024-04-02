#include <Arduino.h>
#include "RotarySwitch.h"

RotarySwitch::RotarySwitch(unsigned short pinCLK, unsigned short pinDT, unsigned short pinSW)
{
    _onCWSwitch = nullptr;
    _onCCWSwitch = nullptr;
    _onButtonPress = nullptr;
    _onButtonRelease = nullptr;

    _pinCLK = pinCLK;
    _pinDT = pinDT;
    _pinSW = pinSW;
}

void RotarySwitch::begin()
{
    setupPins();

    valueCLK = digitalRead(_pinCLK);
    valueDT = digitalRead(_pinDT);
}

void RotarySwitch::attachOnCWSwitch(void (*func)(void))
{
    _onCWSwitch = func;
}

void RotarySwitch::attachOnCCWSwitch(void (*func)(void))
{
    _onCCWSwitch = func;
}

void RotarySwitch::attachOnButtonPress(void (*func)(void))
{
    _onButtonPress = func;
}

void RotarySwitch::attachOnButtonRelease(void (*func)(void))
{
    _onButtonRelease = func;
}

void RotarySwitch::setupPins()
{
    pinMode(_pinCLK, INPUT);
    pinMode(_pinDT, INPUT);
    pinMode(_pinSW, INPUT);

    // FIXME Can't use non-static member functions as ISRs.
    // attachInterrupt(digitalPinToInterrupt(_pinCLK), onCLK, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(_pinDT), onDT, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(_pinSW), onSW, CHANGE);
}

ICACHE_RAM_ATTR void RotarySwitch::onCLK()
{
    unsigned long now = millis();

    if (now - lastMillisCLK > 5)
    {
        valueCLK = digitalRead(_pinCLK);

        if (previousValueCLK != valueCLK)
        {
            previousValueCLK = valueCLK;
            lastMillisCLK = now;
        }
    }
}

ICACHE_RAM_ATTR void RotarySwitch::onDT()
{
    unsigned long now = millis();

    if (now - lastMillisDT > 5)
    {
        valueDT = digitalRead(_pinDT);

        if (previousValueDT != valueDT)
        {
            previousValueDT = valueDT;
            lastMillisDT = now;
        }
    }
}

ICACHE_RAM_ATTR void RotarySwitch::onSW()
{
    unsigned long now = millis();

    if (now - lastMillisSW > 5)
    {
        valueSW = digitalRead(_pinSW);

        if (previousValueSW != valueSW)
        {
            buttonChanged = true;

            previousValueSW = valueSW;
            lastMillisSW = now;
        }
    }
}

void RotarySwitch::update()
{
    uint8_t rotationState = valueCLK << 1 | valueDT;

    if (
        (rotationDirectionCounter == 0 && rotationState == 0b10) || (rotationDirectionCounter == -1 && rotationState == 0b00) || (rotationDirectionCounter == -2 && rotationState == 0b01) || (rotationDirectionCounter == -3 && rotationState == 0b11))
    {
        rotationDirectionCounter--;
    }
    else if (
        (rotationDirectionCounter == 0 && rotationState == 0b01) || (rotationDirectionCounter == 1 && rotationState == 0b00) || (rotationDirectionCounter == 2 && rotationState == 0b10) || (rotationDirectionCounter == 3 && rotationState == 0b11))
    {
        rotationDirectionCounter++;
    }

    if (rotationDirectionCounter == 4)
    {
        if (_onCWSwitch != nullptr)
        {
            _onCWSwitch();
        }
    }
    else if (rotationDirectionCounter == -4)
    {
        if (_onCCWSwitch != nullptr)
        {
            _onCCWSwitch();
        }
    }

    if (rotationState == 0b11)
    {
        rotationDirectionCounter = 0;
    }

    if (buttonChanged)
    {
        buttonChanged = false;

        if (valueSW == LOW)
        {
            if (_onButtonPress != nullptr)
            {
                _onButtonPress();
            }
        }
        else
        {
            if (_onButtonRelease != nullptr)
            {
                _onButtonRelease();
            }
        }
    }
}
