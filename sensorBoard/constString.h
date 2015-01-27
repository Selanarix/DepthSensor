#ifndef CONST_STRING_H
#define CONST_STRING_H

#include <Arduino.h>

namespace ConstStr
{
    typedef struct
    {
        char* buffer;
        uint32_t size;
        uint32_t level;
    } ConstString;

    boolean initConstString(ConstString* thi, char* buf, uint32_t s);
    ConstString* addconstString(ConstString* thi, const char* str);
    ConstString* addString(ConstString* thi, const String b);
    ConstString* addFString(ConstString* thi, const __FlashStringHelper* str);
    ConstString* addChar(ConstString* thi, char chr);
    ConstString* addLong(ConstString* thi, long n, uint8_t base);
    ConstString* addDouble(ConstString* thi, double number, uint8_t digits);

    ConstString* trim(ConstString* thi);
    ConstString* toLower(ConstString* thi);
    ConstString* toUpper(ConstString* thi);
    const ConstString* substring(const ConstString* thi, ConstString* other, uint32_t start, uint32_t len);
    void clear(ConstString* thi);
    
    boolean isFull(const ConstString* thi );
    uint32_t getLength(const ConstString* thi);
    uint32_t getSize(const ConstString* thi);
    const char* getString(const ConstString* thi);
}
#endif
