#ifndef GENERAL_TYPES_H_
#define GENERAL_TYPES_H_

typedef enum
{
    ENABLE,
    DISABLE
} FunctionStatus;

typedef enum
{
  UNIT_mV,
  UNIT_degC,
  UNIT_cm,
  UNIT_mm,
  UNIT_m
} Unit;

const __FlashStringHelper* unitNames[5]; //Amount of Units

void fillUnitNameArray()
{
    unitNames[UNIT_mV] = F("mV");
    unitNames[UNIT_degC] = F("degC");
    unitNames[UNIT_cm] = F("cm");
    unitNames[UNIT_mm] = F("mm");
    unitNames[UNIT_m] = F("m");
}

const __FlashStringHelper* getUnitString(Unit a)
{
    if(a > UNIT_m)
        return F("");
    return unitNames[a];
}

#endif
