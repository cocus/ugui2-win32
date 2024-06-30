#pragma once

/* -------------------------------------------------------------------------------- */
/* -- CONFIG SECTION                                                             -- */
/* -------------------------------------------------------------------------------- */

#include <stdint.h>

/* Enable color mode */
#define UGUI2_USE_COLOR_RGB888   // RGB = 0xFF,0xFF,0xFF
// #define UGUI2_USE_COLOR_RGB565   // RGB = 0bRRRRRGGGGGGBBBBB
//#define UGUI2_USE_COLOR_BW   // BW = 0x00|0xFF

/* Depress text on touch, giving additional visual feedback, but requires full button redraw  */
#define BUTTON_TXT_DEPRESS

/* Enable Touch Support */
// #define UGUI2_USE_TOUCH

/* Enable Console Support */
// #define UGUI2_USE_CONSOLE

/* If you only use standard ASCII, disabling this will save some resources */
// #define UGUI2_USE_UTF8

#define UGUI2_ENABLE_GUI_PROGRESS

/* Enable needed fonts */
#define UGUI2_USE_FONT_4X6
#define UGUI2_USE_FONT_4X16
#define UGUI2_USE_FONT_5X8
#define UGUI2_USE_FONT_5X12
#define UGUI2_USE_FONT_6X8
#define UGUI2_USE_FONT_6X10
#define UGUI2_USE_FONT_7X12
#define UGUI2_USE_FONT_8X8
#define UGUI2_USE_FONT_8X12
////#define UGUI2_USE_FONT_8X12_CYRILLIC
#define UGUI2_USE_FONT_10X16
#define UGUI2_USE_FONT_12X16
#define UGUI2_USE_FONT_12X20
#define UGUI2_USE_FONT_16X26
#define UGUI2_USE_FONT_22X36
#define UGUI2_USE_FONT_24X40
#define UGUI2_USE_FONT_32X53

#define UGUI2_USE_FONT_arial_6X6
//#define UGUI2_USE_FONT_arial_6X6_CYRILLIC
#define UGUI2_USE_FONT_arial_9X10
//#define UGUI2_USE_FONT_arial_9X10_CYRILLIC
#define UGUI2_USE_FONT_arial_10X13
//#define UGUI2_USE_FONT_arial_10X13_CYRILLIC
#define UGUI2_USE_FONT_arial_12X15
//#define UGUI2_USE_FONT_arial_12X15_CYRILLIC
#define UGUI2_USE_FONT_arial_16X18
//#define UGUI2_USE_FONT_arial_16X18_CYRILLIC
#define UGUI2_USE_FONT_arial_20X23
//#define UGUI2_USE_FONT_arial_20X23_CYRILLIC
#define UGUI2_USE_FONT_arial_25X28
//#define UGUI2_USE_FONT_arial_25X28_CYRILLIC
#define UGUI2_USE_FONT_arial_29X35
//#define UGUI2_USE_FONT_arial_29X35_CYRILLIC
#define UGUI2_USE_FONT_arial_35X40
//#define UGUI2_USE_FONT_arial_35X40_CYRILLIC
#define UGUI2_USE_FONT_arial_39X45
//#define UGUI2_USE_FONT_arial_39X45_CYRILLIC
#define UGUI2_USE_FONT_arial_45X52
//#define UGUI2_USE_FONT_arial_45X52_CYRILLIC
#define UGUI2_USE_FONT_arial_49X57
//#define UGUI2_USE_FONT_arial_49X57_CYRILLIC

/* Feature enablers */
// #define UGUI2_USE_PRERENDER_EVENT
// #define UGUI2_USE_POSTRENDER_EVENT
// #define UGUI2_USE_MULTITASKING

/* Specify platform-dependent types here */
#define UG2_POS_T	 UG_S16
typedef uint8_t      UG_U8;
typedef int8_t       UG_S8;
typedef uint16_t     UG_U16;
typedef int16_t      UG_S16;
typedef uint32_t     UG_U32;
typedef int32_t      UG_S32;
typedef const UG_U8  UG2_FONT;
