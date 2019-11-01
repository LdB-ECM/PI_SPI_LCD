#ifndef _BMPFONT_H_
#define _BMPFONT_H_

#ifdef __cplusplus								// If we are including to a C++
extern "C" {									// Put extern C directive wrapper around
#endif
    
#include <stdbool.h>		// C standard unit needed for bool and true/false
#include <stdint.h>			// C standard unit needed for uint8_t, uint32_t, etc
  
typedef struct {
    struct {
        unsigned char_width : 8;                // Font width in pixels
        unsigned char_height: 8;                // Font height in pixels
        unsigned first_char : 8;                // First character
        unsigned last_char : 8;                 // Last character
        unsigned char_pitch: 8;                 // Bytes from character to character
    };
    char* font_name;                            // Font name null term string
    uint8_t* font_bitmap;                       // Pointer to the bitmap
} font_t; 
    

const font_t* GetFont (unsigned number);
    
#ifdef __cplusplus								// If we are including to a C++ file
}												// Close the extern C directive wrapper
#endif

#endif