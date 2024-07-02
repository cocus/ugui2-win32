#pragma once
#include "ugui2.h"

/* -------------------------------------------------------------------------------- */
/* -- WINDOW                                                                     -- */
/* -------------------------------------------------------------------------------- */

/* Title structure */
typedef struct
{
    const char* str;
    UG2_FONT* font;
    UG2_POS_T h_space;
    UG2_POS_T v_space;
    UG_U8 align;
    UG2_COLOR_FORE_BACK colors_active;
    UG2_COLOR_FORE_BACK colors_inactive;
    UG2_POS_T height;
} UG_TITLE;

/* Window structure */
struct S_WINDOW
{
    UG2_TYPEDEF_OBJ_INHERITT

    UG_U16 state;

    UG_TITLE title;
};

typedef struct S_WINDOW                               UG2_WINDOW;
#define UG2_CAST_WINDOW_AS_OBJ(wnd) ((UG2_OBJECT*)(wnd))
#define UG2_CAST_OBJ_AS_WINDOW(obj) ((UG2_WINDOW*)(obj))

UG2_RESULT UG2_WindowInitialize(UG2_WINDOW* wnd,
    UG2_OBJECT* parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message);
UG2_RESULT UG2_WindowShow(UG2_WINDOW* wnd);
UG2_RESULT UG2_WindowSetTitleText(UG2_WINDOW* wnd, const char* str);
UG2_RESULT UG2_WindowSetTitleFont(UG2_WINDOW* wnd, UG2_FONT* font);
