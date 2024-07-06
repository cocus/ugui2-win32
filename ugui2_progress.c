#include "ugui2_progress.h"

static const UG2_COLOR_RECT _ug2_progress_default_border_theme[3] = {C_PAL_PROGRESS};

static UG2_RESULT _UG2_ProgressHandleRedraw(UG2_PROGRESS *pb)
{
    UG2_RECT abs_rect = {0, 0, 0, 0};
    UG2_POS_T d = 0;
    UG2_POS_T w, wps, wpe;

    if (pb == NULL)
        return UG_RESULT_ARG;

    UG2_GetObjectScreenRect(UG2_BaseObject(pb), &abs_rect);

    /* Is the window visible? */
    if (pb->base_object.style & STYLE_VISIBLE)
    {
        if (pb->base_object.style & STYLE_3D)
        { /* 3D */
            UG2_Draw3DObjectFrame(
                abs_rect.xs,
                abs_rect.ys,
                abs_rect.xe,
                abs_rect.ye,
                _ug2_progress_default_border_theme);
            d += 3;
        }
        else
        { /* 2D */
            UG2_DrawFrame(
                abs_rect.xs,
                abs_rect.ys,
                abs_rect.xe,
                abs_rect.ye,
                pb->base_object.colors.foreground);
            d += 1;
        }

        w = ((abs_rect.xe - d) - (abs_rect.xs + d));
        wps = w * pb->progress / 100;
        wpe = w - wps;

        // Draw remaining frame first
        if (wpe > 0)
        {
            UG_S16 xs = abs_rect.xs + d + wps;
            UG_S16 xe = abs_rect.xe - d;

            if (pb->base_object.style & STYLE_BACKGROUND_MESH)
            {
                // FIXME: Need fix, if start at 0, it is shifted 1 pixel right.
                // Needed to match mesh pattern, otherwise it would "scroll right"
                if ((((abs_rect.xs + d) & 1) && (wps & 1)) ||
                    (!((abs_rect.xs + d) & 1) && !(wps & 1)))
                    xs++;

                UG2_DrawMesh(
                    xs,
                    abs_rect.ys + d,
                    xe,
                    abs_rect.ye - d,
                    2,
                    pb->base_object.colors.foreground);
            }
            else
            {
                UG2_FillFrame(
                    xs,
                    abs_rect.ys + d,
                    xe,
                    abs_rect.ye - d,
                    pb->base_object.colors.background);
            }
        }

        // Draw elapsed frame
        if (pb->progress > 0)
        {
            UG2_FillFrame(
                abs_rect.xs + d,
                abs_rect.ys + d,
                abs_rect.xs + d + wps,
                abs_rect.ye - d,
                pb->base_object.colors.foreground);
        }
    }
    else
    {
        /* invisible so fill it with the background color? */
        UG2_FillFrame(
            abs_rect.xs + d,
            abs_rect.ys + d,
            abs_rect.xe - d,
            abs_rect.ye - d,
            pb->base_object.colors.background);
    }

    return UG_RESULT_OK;
}

static UG2_RESULT _UG2_ProgressHandleMessage(UG2_MESSAGE *msg)
{
    if (!msg || !msg->obj)
        return UG_RESULT_ARG;

    UG2_PROGRESS *pb = UG2_CAST_OBJ_AS_PROGRESS(msg->obj);

    switch (msg->type)
    {
        /* repaint all */
    case MSG_REDRAW:
        pb->base_object.busy = 1;
        _UG2_ProgressHandleRedraw(pb);
        pb->base_object.busy = 0;
        return UG_RESULT_OK;

        /* set the progress */
    case MSG_PROGRESS_SET:
        pb->base_object.busy = 1;
        if ((UG_U8)msg->data > 100)
            return UG_RESULT_ARG;
        pb->progress = (UG_U8)msg->data;
        pb->base_object.busy = 0;
        /* and redraw it ! */
        return UG2_SendMessage(msg->obj, MSG_REDRAW, 0, 0, 0, NULL);

        /* get the progress */
    case MSG_PROGRESS_GET:
        if (!msg->data)
            return UG_RESULT_ARG;
        pb->base_object.busy = 1;
        *(UG_U8 *)msg->data = pb->progress;
        pb->base_object.busy = 0;
        return UG_RESULT_OK;

    default:
        return UG_RESULT_MSG_UNHANDLED;
    }
}

UG2_RESULT UG2_ProgressInitialize(
    UG2_PROGRESS *pb,
    UG2_OBJECT *parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message)
{
    UG2_RESULT res = UG2_GenericObjectInitialize(
        UG2_BaseObject(pb),
        parent,
        x,
        y,
        width,
        height,
        &_UG2_ProgressHandleMessage,
        OBJ_TYPE_WINDOW);

    if (res != UG_RESULT_OK)
        return res;

    /* Initialize window */
    pb->base_object.user_handler = handle_message;

    UG2_RectFromDims(&pb->base_object.rect, x, y, width, height);

    pb->progress = 0;
    pb->base_object.style =
        STYLE_ENABLED |
        STYLE_3D;

    return UG_RESULT_OK;
}

UG2_RESULT UG2_ProgressSetProgress(UG2_PROGRESS *pb, const UG_U8 progress)
{
    return UG2_SendMessage(UG2_BaseObject(pb), MSG_PROGRESS_SET, 0, 0, 0, (void *)progress);
}

UG2_RESULT UG2_ProgressGetProgress(UG2_PROGRESS *pb, UG_U8 *progress)
{
    return UG2_SendMessage(UG2_BaseObject(pb), MSG_PROGRESS_GET, 0, 0, 0, (void *)progress);
}
