#pragma once
#include "ugui2.h"

/* -------------------------------------------------------------------------------- */
/* -- BUTTON                                                                     -- */
/* -------------------------------------------------------------------------------- */

/* Button structure */
struct S_BUTTON
{
    UG2_TYPEDEF_OBJ_INHERITT;
    UG_U8 pressed;
};

typedef struct S_BUTTON UG2_BUTTON;
#define UG2_CAST_OBJ_AS_BUTTON(obj) ((UG2_BUTTON *)(obj))

UG2_RESULT UG2_ButtonInitialize(
    UG2_BUTTON *btn,
    UG2_OBJECT *parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message);
