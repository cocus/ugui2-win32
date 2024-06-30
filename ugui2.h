#pragma once

//#include <stdlib.h>
#include "ugui2_config.h"
#include "ugui2_colors.h"

/* -------------------------------------------------------------------------------- */
/* -- TYPEDEFS                                                                   -- */
/* -------------------------------------------------------------------------------- */
typedef struct S_OBJECT                               UG2_OBJECT;
typedef UG_S8                                         UG2_RESULT;

#if defined(UGUI2_USE_COLOR_RGB888)
typedef UG_U32                       UG2_COLOR;
#elif defined(UGUI2_USE_COLOR_RGB565)
typedef UG_U16                       UG2_COLOR;
#elif defined(UGUI2_USE_COLOR_BW)
typedef UG_U8                        UG2_COLOR;
#endif
#if !defined(UGUI2_USE_COLOR_RGB888) && !defined(UGUI2_USE_COLOR_RGB565) && !defined(UGUI2_USE_COLOR_BW)
#error "You must define a color space!"
#endif
#if defined(UGUI2_USE_COLOR_RGB888) && defined(UGUI2_USE_COLOR_RGB565) || \
      defined(UGUI2_USE_COLOR_RGB888) && defined(UGUI2_USE_COLOR_BW) || \
      defined(UGUI2_USE_COLOR_RGB565) && defined(UGUI2_USE_COLOR_BW) || \
      defined(UGUI2_USE_COLOR_RGB888) && defined(UGUI2_USE_COLOR_RGB565) && defined(UGUI2_USE_COLOR_BW)
#error "You must define only one color space!"
#endif

/* -------------------------------------------------------------------------------- */
/* -- DEFINES                                                                    -- */
/* -------------------------------------------------------------------------------- */
/* Helpers */
#define UG2_GetFontWidth(f)                            *(f+1)
#define UG2_GetFontHeight(f)                           *(f+2)
#define UG2_swap(x, y)                                    { x = y^x; y = x ^ y; x = y ^ x; }

/* Sizing helpers */
#define UG2_SizeToPos(xs, ys, w, h)                        xs, ys, xs+w, ys+h


/* Alignments */
#define ALIGN_H_LEFT                                  (1<<0)
#define ALIGN_H_CENTER                                (1<<1)
#define ALIGN_H_RIGHT                                 (1<<2)
#define ALIGN_V_TOP                                   (1<<3)
#define ALIGN_V_CENTER                                (1<<4)
#define ALIGN_V_BOTTOM                                (1<<5)
#define ALIGN_BOTTOM_RIGHT                            (ALIGN_V_BOTTOM|ALIGN_H_RIGHT)
#define ALIGN_BOTTOM_CENTER                           (ALIGN_V_BOTTOM|ALIGN_H_CENTER)
#define ALIGN_BOTTOM_LEFT                             (ALIGN_V_BOTTOM|ALIGN_H_LEFT)
#define ALIGN_CENTER_RIGHT                            (ALIGN_V_CENTER|ALIGN_H_RIGHT)
#define ALIGN_CENTER                                  (ALIGN_V_CENTER|ALIGN_H_CENTER)
#define ALIGN_CENTER_LEFT                             (ALIGN_V_CENTER|ALIGN_H_LEFT)
#define ALIGN_TOP_RIGHT                               (ALIGN_V_TOP|ALIGN_H_RIGHT)
#define ALIGN_TOP_CENTER                              (ALIGN_V_TOP|ALIGN_H_CENTER)
#define ALIGN_TOP_LEFT                                (ALIGN_V_TOP|ALIGN_H_LEFT)

/* -------------------------------------------------------------------------------- */
/* -- FUNCTION RESULTS                                                           -- */
/* -------------------------------------------------------------------------------- */
#define UG_RESULT_MSG_UNHANDLED                       -3
#define UG_RESULT_ARG                                 -2
#define UG_RESULT_FAIL                                -1
#define UG_RESULT_OK                                  0

/* -------------------------------------------------------------------------------- */
/* -- FONTS                                                                      -- */
/* -------------------------------------------------------------------------------- */

/* Font structures */
typedef enum
{
    FONT_TYPE_1BPP,
    FONT_TYPE_8BPP,
    FONT_TYPE_LIC_1BPP   /* Fonts generated by the LCD Image Converter */
} FONT_TYPE;

typedef struct
{
    FONT_TYPE    font_type;
    UG_U8        is_old_font;                      // This exists to maintain compatibility with old fonts, as they use code page 850 instead of Unicode
    UG_U8        char_width;
    UG_U8        char_height;
    UG_U16       bytes_per_char;
    UG_U16       number_of_chars;
    UG_U16       number_of_offsets;
    const UG_U8* widths;
    const UG_U8* offsets;
    const UG_U8* data;
    UG2_FONT* font;
} UG2_FONT_DATA;

#ifdef UGUI2_USE_UTF8
typedef UG_U16                                        UG2_CHAR;
#else
typedef char                                          UG2_CHAR;
#endif

/* for fonts generated by LCD Image Converter */
typedef const struct
{
    UG2_CHAR      code;
    UG_U8        width;
    UG_U16       size;
    const UG_U8* data;
} UG_FONT_LIC_INFO;

typedef const struct
{
    FONT_TYPE         font_type;        // FONT_TYPE_LIC_1BPP
    UG_U8             char_width;       // max width of char
    UG_U8             char_height;      // Char height
    UG_U16            num_of_chars;     // Number of chars
    UG2_CHAR           first_char;       // Min char code
    UG2_CHAR           last_char;        // Max char code
    UG_FONT_LIC_INFO* info;
} UG_FONT_LIC;

/* -------------------------------------------------------------------------------- */
/* -- UNIVERSAL STRUCTURES                                                       -- */
/* -------------------------------------------------------------------------------- */
/* Area structure */
typedef struct
{
    UG2_POS_T xs;
    UG2_POS_T ys;
    UG2_POS_T xe;
    UG2_POS_T ye;
} UG2_RECT;

/* Text structure */
typedef struct
{
    const char* str;
    UG2_FONT* font;
    UG2_RECT area;
    UG2_COLOR color_foreground;
    UG2_COLOR color_background;
    UG_U8 align;
    UG2_POS_T h_space;
    UG2_POS_T v_space;
} UG2_TEXT;

/* -------------------------------------------------------------------------------- */
/* -- MESSAGES                                                                   -- */
/* -------------------------------------------------------------------------------- */
/* Message structure */


typedef enum
{
    MSG_UPDATE = 0,
    MSG_INIT,
    MSG_RESIZE,
    MSG_DESTROY,
    MSG_FOCUS_SET,
    MSG_FOCUS_LOST,
    MSG_ENABLE,
    MSG_DISABLE,
    MSG_TEXT_SET,
    MSG_TEXT_GET,
    MSG_SHOW,
    MSG_HIDE,
    MSG_FONT_SET,
    MSG_FONT_GET
} UG2_MESSAGE_TYPE;

typedef struct
{
    UG2_OBJECT* obj;
    UG_U16 type; /* see UG2_MESSAGE_TYPE for base message types */
    UG_U8 id;
    UG_U8 sub_id;
    UG_U8 event;
    void* data;
} UG2_MESSAGE;


/* -------------------------------------------------------------------------------- */
/* -- OBJECTS                                                                    -- */
/* -------------------------------------------------------------------------------- */
/* Object structure */
typedef enum
{
    OBJ_TYPE_WINDOW = 0,
    OBJ_TYPE_LABEL,
    OBJ_TYPE_TEXT,
    OBJ_TYPE_BUTTON,
    OBJ_TYPE_CHECKBOX,
    OBJ_TYPE_PROGRESS
} UG2_OBJECT_TYPES;

typedef enum
{
    STYLE_HIDDEN  = (0 << 0),
    STYLE_VISIBLE = (1 << 0),

    STYLE_DISABLED = (0 << 1),
    STYLE_ENABLED  = (1 << 1),

    STYLE_UNFOCUSED = (0 << 2),
    STYLE_FOCUSED   = (1 << 2),

    STYLE_NO_TITLEBAR = (0 << 3),
    STYLE_TITLEBAR    = (1 << 3),

    STYLE_FLAT = (0 << 4),
    STYLE_3D   = (1 << 4),

} UG2_STYLE_TYPES;

typedef UG2_RESULT (*UG2_HandleMessage)(UG2_MESSAGE*);

struct S_OBJECT
{
    UG_U32 style; /* see UG2_STYLE_TYPES for base style types */

    UG2_RECT area_abs;                            /* absolute area of the object                */
    UG2_RECT area_rel;                            /* relative area of the object                */

    UG_U8 type;                               /* object type                                */
    UG_U8 id;                                 /* object ID                                  */

    UG_U8 busy;
    UG2_OBJECT* parent;
    UG2_OBJECT* child; /* list of children, if any */
    UG2_OBJECT* focused_child; /* focused children, if any */

    UG2_HandleMessage handle_message;   /* pointer to object-specific update function */
};




/* -------------------------------------------------------------------------------- */
/* -- �GUI DRIVER                                                                -- */
/* -------------------------------------------------------------------------------- */
typedef struct
{
    void* driver;
    UG_U8 state;
} UG2_DRIVER;

#define DRIVER_REGISTERED                             (1<<0)
#define DRIVER_ENABLED                                (1<<1)

/* Supported drivers */
#define NUMBER_OF_DRIVERS                             4
#define DRIVER_DRAW_LINE                              0
#define DRIVER_FILL_FRAME                             1
#define DRIVER_FILL_AREA                              2
#define DRIVER_DRAW_BMP                               3

/* -------------------------------------------------------------------------------- */
/* -- �GUI CORE STRUCTURE                                                        -- */
/* -------------------------------------------------------------------------------- */

struct _UG2_DEVICE;
typedef struct _UG2_DEVICE UG2_DEVICE;

typedef void (*PixelSetFunc)(UG_S16, UG_S16, UG2_COLOR);
typedef void (*FlushFunc)(void);
typedef void (*PushPixelsFunc)(UG_U16, UG2_COLOR);
typedef PushPixelsFunc(*DriverFillAreaFunct)(UG_S16, UG_S16, UG_S16, UG_S16);

struct _UG2_DEVICE {
    UG_S16 x_dim;
    UG_S16 y_dim;
    PixelSetFunc pset;
    FlushFunc flush;
};

typedef struct
{
    UG2_DEVICE* device;

    UG2_OBJECT* next_window;
    UG2_OBJECT* active_window;
    UG2_OBJECT* last_window;

    UG2_FONT* font;
    UG2_FONT_DATA currentFont;
    UG_U8 transparent_font;
    UG_S8 char_h_space;
    UG_S8 char_v_space;
    UG2_COLOR fore_color;
    UG2_COLOR back_color;
    UG2_COLOR desktop_color;
    UG_U8 state;
    UG2_DRIVER driver[NUMBER_OF_DRIVERS];

    void* message_pump; /* impl specific */
} UG2_GUI;
