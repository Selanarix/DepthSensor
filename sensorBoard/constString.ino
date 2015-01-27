#include "constString.h"

namespace ConstStr
{
    //-------------------------- Private Types -------------------------------------
    //-------------------- Private Function Prototypes -----------------------------
    inline static boolean isFreeSpaceInBuffer(const ConstString* thi);
    //------------------------- Private Data ---------------------------------------
    //------------------------ Read only -------------------------------------------
    //------------------------ Public Functions ------------------------------------

    boolean initConstString(ConstString* thi, char* buf, uint32_t s)
    {
        if(thi == NULL || buf == NULL || s < 1)
            return false;
        thi->buffer = buf;
        thi->level = 0;
        thi->size = s;
        clear(thi);
        return true;
    }
    
    ConstString* addconstString(ConstString* thi, const char* str)
    {
        if(thi == NULL)
            return NULL;
        if(str == NULL)
            return thi;

        uint32_t index = 0;
        while(str[index] != 0 && isFreeSpaceInBuffer(thi))
        {
            thi->buffer[thi->level] = str[index];
            index++;
            thi->level++;
        }
        return thi;
    }

    ConstString* addString(ConstString* thi, const String b)
    {
        if(thi == NULL)
            return NULL;
        if(b == NULL)
            return thi;
        b.toCharArray(&thi->buffer[thi->level], thi->size - 1 - thi->level);
        return thi;
    }

    ConstString* addFString(ConstString* thi, const __FlashStringHelper* str)
    {
        if(thi == NULL)
            return NULL;
        if(str == NULL)
            return thi;
            
        const prog_char* p = (const prog_char *)str;
        char chr = 0;
        do
        {
            chr = pgm_read_byte_near (p++);
            thi->buffer[thi->level] = chr;
            thi->level++;
        }  
        while(chr != 0 && isFreeSpaceInBuffer(thi));
        thi->level--;
        return thi;
    }

    ConstString* addChar(ConstString* thi, char chr)
    {
        if(thi == NULL)
            return NULL;
        if(!isFreeSpaceInBuffer(thi))
            return thi;
        thi->buffer[thi->level] = chr;
        thi->level ++;
        return thi;
    }

    ConstString* addLong(ConstString* thi, long n, uint8_t base) 
    {
        if(thi == NULL)
            return NULL;

        char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
        char* str = &buf[sizeof(buf) - 1];

        // Handle negative numbers
        if (n < 0)
        {
         addChar(thi,'-');;
         n = -n;
        }

        *str = '\0';

        // prevent crash if called with base == 1
        if (base < 2) 
           base = 10;

        do 
        {
           unsigned long m = n;
           n /= base;
           char c = m - base * n;
           *--str = c < 10 ? c + '0' : c + 'A' - 10;
        } while(n);

        while(*str != 0 && isFreeSpaceInBuffer(thi))
        {
            thi->buffer[thi->level] = *str;
            thi->level++;
            str++;
        }
        
        return thi;
    }

    ConstString* addDouble(ConstString* thi, double number, uint8_t digits)
    { 

        if(thi == NULL)
            return NULL;
        if (isnan(number)) 
           return addString(thi, "nan");
        if (isinf(number)) 
           return addString(thi, "nan");
        if (number > 4294967040.0) 
           return addString (thi,"ovf");  // constant determined empirically
        if (number < -4294967040.0) 
           return addString (thi,"ovf");  // constant determined empirically

        // Handle negative numbers
        if (number < 0.0)
        {
         addChar(thi,'-');
         number = -number;
        }

        // Round correctly so that print(1.999, 2) prints as "2.00"
        double rounding = 0.5;
        for (uint8_t i=0; i<digits; ++i)
        rounding /= 10.0;

        number += rounding;

        // Extract the integer part of the number and print it
        unsigned long int_part = (unsigned long)number;
        double remainder = number - (double)int_part;
        addLong(thi,int_part,10);

        // Print the decimal point, but only if there are digits beyond
        if (digits > 0) 
          addChar(thi,'.');

        // Extract digits from the remainder one at a time
        while (digits-- > 0)
        {
            remainder *= 10.0;
            int toPrint = int(remainder);
            addLong(thi,toPrint,10);
            remainder -= toPrint; 
        } 

        return thi;
    }

    ConstString* trim(ConstString* thi)
    {
        if(thi == NULL)
            return NULL;
        char tmpBuf[thi->level + 1]; //Copy legal chars here
        for(uint32_t index = 0; index < thi->level + 1; index ++)
            tmpBuf[index] = 0;
            
        uint32_t tmpBufIndex = 0;
        for(uint32_t index = 0; index < thi->level; index ++)
            if(thi->buffer[index] != '\r' && thi->buffer[index] != '\n' && thi->buffer[index] != '\t')
                tmpBuf[tmpBufIndex++] = thi->buffer[index];

        thi->level = tmpBufIndex;
        uint32_t overwriteIndex = 0;        
        for(; overwriteIndex < tmpBufIndex; overwriteIndex ++)
            thi->buffer[overwriteIndex] = tmpBuf[overwriteIndex];
        for(; overwriteIndex < thi->size; overwriteIndex ++)
            thi->buffer[overwriteIndex] = 0; //Wipe rest of buffer 
        return thi;
    }
    ConstString* toLower(ConstString* thi)
    {
        if(thi == NULL)
            return NULL;
        for(uint32_t index = 0; index < thi->level; index ++)
            if(thi->buffer[index] <= 'Z' && thi->buffer[index] >= 'A')
                thi->buffer[index] = 'a' + (thi->buffer[index] - 'A');
        return thi;       
    }

    ConstString* toUpper(ConstString* thi)
    {
        if(thi == NULL)
            return NULL;
        for(uint32_t index = 0; index < thi->level; index ++)
            if(thi->buffer[index] <= 'z' && thi->buffer[index] >= 'a')
                thi->buffer[index] = 'A' + (thi->buffer[index] - 'a');
        return thi;  
    }
    
    const ConstString* substring(const ConstString* thi, ConstString* other, uint32_t start, uint32_t len)
    {
        if(thi == NULL)
            return NULL;
        if(other == NULL)
            return thi; 
        clear(other);
        
        for(uint32_t index = 0; start + index < thi->level && index < len; index ++)
            other->buffer[index] = thi->buffer[start+index];
        return thi;
    }

    void clear(ConstString* thi)
    {
        if(thi == NULL)
            return;
        thi->level = 0;
        for(uint32_t index = 0; index < thi->size; index++)
            thi->buffer[index] = 0;
    }
    
    boolean isFull(const ConstString* thi)
    {
        return !isFreeSpaceInBuffer(thi);
    }

    uint32_t getLength(const ConstString* thi)
    {
        if(thi == NULL)
            return 0;
        return thi->level;
    }

    uint32_t getSize(const ConstString* thi)    
    {
        if(thi == NULL)
            return 0;
        return thi->size;
    }

    const char* getString(const ConstString* thi)
    {
        if(thi == NULL)
            return "";
        return thi->buffer;
    }

    //------------------------------ Private Functions ----------------------------

    inline static boolean isFreeSpaceInBuffer(const ConstString* thi)
    {
        return thi != NULL && thi->level < thi->size - 1; //Skip one char for zero termination
    }
}

