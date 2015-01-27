#ifndef IMAGES_H
#define IMAGES_H

#include <avr/pgmspace.h>

#define SRAM_IMAGE_BUFFER_SIZE 30

typedef const struct
{
    uint8_t* data;
    uint32_t size;
    uint32_t width;
    uint32_t height;  
} Image;

const PROGMEM prog_uchar warningBytes[]  = 
{
  0xff,0xff
  ,0xff,0xff
  ,0xfc,0xff
  ,0xfc,0xff
  ,0xf8,0x7f
  ,0xf3,0x3f
  ,0xf3,0x3f
  ,0xe3,0x1f
  ,0xc3,0x0f
  ,0xc0,0x0f
  ,0x83,0x07
  ,0x80,0x07
  ,0xff,0xff
  ,0xff,0xff
};

const PROGMEM prog_uchar errorBytes[]  = 
{
  0xff,0xff
  ,0x87,0xff
  ,0x8c,0xff
  ,0xe1,0xff
  ,0xf3,0xff
  ,0xe1,0xff
  ,0xcc,0xff
  ,0xff,0xff
  ,0xff,0xff
};

static uint8_t buffer[SRAM_IMAGE_BUFFER_SIZE];


void* memcpy_PF (void* dest, uint_farptr_t src, size_t n); 		

static Image loadWarning()
{
    const uint32_t size = 14*2; 
    uint32_t i = 0;
    for(i = 0; i < size && i < SRAM_IMAGE_BUFFER_SIZE; i++)
        buffer[i] = pgm_read_byte_near (warningBytes + i);
    for(; i < SRAM_IMAGE_BUFFER_SIZE; i++)
        buffer[i] = 0;
     Image warning = { 
      buffer,
      size,
      14,
      14
    };
    return warning;
};

static Image loadError()
{
   const uint32_t size = 9*2; 
   uint32_t i = 0;
   for(i = 0; i < size && i < SRAM_IMAGE_BUFFER_SIZE; i++)
        buffer[i] = pgm_read_byte_near (errorBytes + i);
   for(; i < SRAM_IMAGE_BUFFER_SIZE; i++)
        buffer[i] = 0;
   Image error = { 
    buffer,
    size,
    10,
    9
  };
  return error;
};


#endif
