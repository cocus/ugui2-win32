#pragma once
#include "ugui2.h"

/* -------------------------------------------------------------------------------- */
/* -- CHECKBOX                                                                   -- */
/* -------------------------------------------------------------------------------- */

/* Checkbox structure */
struct S_CHECKBOX
{
    UG2_TYPEDEF_OBJ_INHERITT;

    UG_U8 status;
};

typedef struct S_CHECKBOX               UG2_CHECKBOX;
#define UG2_CAST_OBJ_AS_CHECKBOX(obj)   ((UG2_CHECKBOX*)(obj))

#define UG2_CHECKBOX_STATUS_UNTICK 0
#define UG2_CHECKBOX_STATUS_UNTICK_PRESSED 1
#define UG2_CHECKBOX_STATUS_TICK 2
#define UG2_CHECKBOX_STATUS_TICK_PRESSED 3
#define UG2_CHECKBOX_STATUS_GRAYED 4
#define UG2_CHECKBOX_STATUS_GRAYED_PRESSED 5

UG2_RESULT UG2_CheckboxInitialize(UG2_CHECKBOX* cb,
    UG2_OBJECT* parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message);

