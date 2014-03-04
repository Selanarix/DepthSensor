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

const char* unitNames[] = {
  "mV"
  "degC"
  "cm"
  "mm"
  "m"
};

static const char* getUnitString(Unit a)
{
    if(a > UNIT_m)
        return "";
    return unitNames[a];
}

#endif
