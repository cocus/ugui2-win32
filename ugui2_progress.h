#pragma once
#include "ugui2.h"

/* -------------------------------------------------------------------------------- */
/* -- PROGRESS BAR                                                               -- */
/* -------------------------------------------------------------------------------- */

/* Window structure */
struct S_PROGRESS
{
    UG2_TYPEDEF_OBJ_INHERITT;

    UG_U8 progress;
};

typedef struct S_PROGRESS UG2_PROGRESS;
#define UG2_CAST_OBJ_AS_PROGRESS(obj) ((UG2_PROGRESS *)(obj))

UG2_RESULT UG2_ProgressInitialize(
    UG2_PROGRESS *pb,
    UG2_OBJECT *parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message);

UG2_RESULT UG2_ProgressSetProgress(UG2_PROGRESS *pb, const UG_U8 progress);
UG2_RESULT UG2_ProgressGetProgress(UG2_PROGRESS *pb, UG_U8 *progress);
