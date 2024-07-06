#pragma once

/* -------------------------------------------------------------------------------- */
/* -- CONFIG SECTION                                                             -- */
/* -------------------------------------------------------------------------------- */

#include <stdint.h>

/* Enable color mode */
// #define UGUI2_USE_COLOR_RGB888   // RGB = 0xFF,0xFF,0xFF
// #define UGUI2_USE_COLOR_RGB565   // RGB = 0bRRRRRGGGGGGBBBBB
#define UGUI2_USE_COLOR_BW   // BW = 0x00|0xFF


#ifdef UGUI2_USE_COLOR_BW
#define C_PAL_WINDOW                   /* Frame 0 */ \
                                       { C_BLACK,    \
                                         C_BLACK,    \
                                         C_BLACK,    \
                                         C_BLACK },  \
                                       /* Frame 1 */ \
                                       { C_WHITE,    \
                                         C_WHITE,    \
                                         C_BLACK,    \
                                         C_BLACK },  \
                                       /* Frame 2 */ \
                                       { C_WHITE,    \
                                         C_WHITE,    \
                                         C_BLACK,    \
                                         C_BLACK }

#define C_PAL_BUTTON_PRESSED           /* Frame 0 */ \
                                       { C_BLACK,    \
                                        C_BLACK,     \
                                        C_BLACK,     \
                                        C_BLACK },   \
                                       /* Frame 1 */ \
                                       { C_BLACK,    \
                                        C_BLACK,     \
                                        C_WHITE,     \
                                        C_WHITE },   \
                                       /* Frame 2 */ \
                                       { C_WHITE,    \
                                        C_WHITE,     \
                                        C_WHITE,     \
                                        C_WHITE }

#define C_PAL_BUTTON_RELEASED          /* Frame 0 */ \
                                       { C_BLACK,    \
                                         C_BLACK,    \
                                         C_BLACK,    \
                                         C_BLACK },  \
                                       /* Frame 1 */ \
                                       { C_WHITE,    \
                                        C_WHITE,     \
                                        C_BLACK,     \
                                        C_BLACK },   \
                                       /* Frame 2 */ \
                                       { C_WHITE,    \
                                        C_WHITE,     \
                                        C_WHITE,     \
                                        C_WHITE }

#define C_DESKTOP_COLOR                C_WHITE
#define C_FORE_COLOR                   C_BLACK
#define C_BACK_COLOR                   C_WHITE
#define C_TITLE_FORE_COLOR             C_WHITE
#define C_TITLE_BACK_COLOR             C_BLACK
#define C_INACTIVE_TITLE_FORE_COLOR    C_WHITE
#define C_INACTIVE_TITLE_BACK_COLOR    C_BLACK

#define C_PAL_PROGRESS                 C_PAL_BUTTON_RELEASED

#define C_PAL_CHECKBOX_PRESSED         C_PAL_BUTTON_PRESSED
#define C_PAL_CHECKBOX_RELEASED        C_PAL_BUTTON_RELEASED

#else
#define C_PAL_WINDOW                   /* Frame 0 */   \
                                       { C_WHITE_39,   \
                                         C_WHITE_39,   \
                                         C_WHITE_39,   \
                                         C_WHITE_39 }, \
                                       /* Frame 1 */   \
                                       { C_WHITE_89,   \
                                         C_WHITE_89,   \
                                         C_WHITE_41,   \
                                         C_WHITE_41 }, \
                                       /* Frame 2 */   \
                                       { C_WHITE,      \
                                         C_WHITE,      \
                                         C_WHITE_63,   \
                                         C_WHITE_63 }

#define C_PAL_BUTTON_PRESSED           /* Frame 0 */ \
                                       { C_WHITE_39, \
                                        C_WHITE_39,  \
                                        C_WHITE_39,  \
                                        C_WHITE_39 },\
                                       /* Frame 1 */ \
                                       { C_WHITE_63, \
                                        C_WHITE_63,  \
                                        C_WHITE_63,  \
                                        C_WHITE_63 },\
                                       /* Frame 2 */ \
                                       { C_WHITE_94, \
                                        C_WHITE_94,  \
                                        C_WHITE_94,  \
                                        C_WHITE_94 }

#define C_DESKTOP_COLOR                C_TEAL
#define C_FORE_COLOR                   C_BLACK
#define C_BACK_COLOR                   0xD0D0D0
#define C_TITLE_FORE_COLOR             C_WHITE
#define C_TITLE_BACK_COLOR             0x1064D0
//#define C_TITLE_BACK_COLOR             0x000080
#define C_INACTIVE_TITLE_FORE_COLOR    C_WHITE
#define C_INACTIVE_TITLE_BACK_COLOR    C_GRAY

#define C_PAL_BUTTON_RELEASED          C_PAL_WINDOW

#define C_PAL_PROGRESS                 C_PAL_BUTTON_RELEASED

#define C_PAL_CHECKBOX_PRESSED         C_PAL_BUTTON_PRESSED
#define C_PAL_CHECKBOX_RELEASED        C_PAL_BUTTON_RELEASED

#endif



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
