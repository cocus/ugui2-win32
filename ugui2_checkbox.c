#include "ugui2_checkbox.h"

static const UG2_COLOR_RECT _ug2_checkbox_pressed_border_theme[3] = { C_PAL_CHECKBOX_PRESSED };

static const UG2_COLOR_RECT _ug2_checkbox_released_border_theme[3] = { C_PAL_CHECKBOX_RELEASED };

static UG2_RESULT _UG2_CheckboxHandleRedraw(UG2_CHECKBOX* cb)
{
    UG2_POS_T xs, ys, xe, ye;
    UG2_RECT abs_rect = { 0, 0, 0, 0 };
    UG2_TEXT txt;
    UG2_POS_T d = 0, o = 0;
    UG2_POS_T d2;
    UG2_COLOR c;
    UG_U8 pressed = 0;
    UG_U8 tick = 0;

    if (cb == NULL) return UG_RESULT_ARG;

    UG2_GetObjectScreenRect(UG2_BaseObject(cb), &abs_rect);

    UG2_PosFromRect(
        &abs_rect,
        xs, ys, xe, ye);

    /* Is the window visible? */
    if (cb->base_object.style & STYLE_VISIBLE)
    {
        /* 3D or 2D style? */
        d = (cb->base_object.style & STYLE_3D) ? 3 : 1;

        if ((cb->status == UG2_CHECKBOX_STATUS_GRAYED_PRESSED) ||
            (cb->status == UG2_CHECKBOX_STATUS_TICK_PRESSED) ||
            (cb->status == UG2_CHECKBOX_STATUS_UNTICK_PRESSED))
        {
            pressed = 1;
        }

        if ((cb->status == UG2_CHECKBOX_STATUS_TICK_PRESSED) ||
            (cb->status == UG2_CHECKBOX_STATUS_TICK))
        {
            tick = 1;
        }
        else if ((cb->status == UG2_CHECKBOX_STATUS_GRAYED_PRESSED) ||
            (cb->status == UG2_CHECKBOX_STATUS_GRAYED))
        {
            tick = 2;
        }

        d2 = (UG2_GetFontWidth(cb->base_object.font) < UG2_GetFontHeight(cb->base_object.font)) ? UG2_GetFontHeight(cb->base_object.font) : UG2_GetFontWidth(cb->base_object.font);

        txt.colors = cb->base_object.colors;

        UG2_FillFrame(abs_rect.xs, abs_rect.ys, abs_rect.xe, abs_rect.ye, txt.colors.background);

        /* Draw button text */
        txt.area.xs = abs_rect.xs + d2 + 2 * d + 1;
        txt.area.ys = abs_rect.ys + d + o;
        txt.area.xe = abs_rect.xe - d + o;
        txt.area.ye = abs_rect.ye - d + o;
        txt.align = cb->base_object.text_align;
        txt.font = cb->base_object.font;
        txt.h_space = cb->base_object.text_h_space;
        txt.v_space = cb->base_object.text_v_space;
        txt.str = cb->base_object.text;
        UG2_PutText(&txt);

        /* Draw Checkbox X */
        c = tick == 1 ? cb->base_object.colors.foreground : cb->base_object.colors.background;
        UG_DrawLine(abs_rect.xs + d + 1, abs_rect.ys + d, abs_rect.xs + d2 + d - 1, abs_rect.ys + d2 + d - 2, c);
        UG_DrawLine(abs_rect.xs + d, abs_rect.ys + d, abs_rect.xs + d2 + d - 1, abs_rect.ys + d2 + d - 1, c);
        UG_DrawLine(abs_rect.xs + d, abs_rect.ys + d + 1, abs_rect.xs + d2 + d - 2, abs_rect.ys + d2 + d - 1, c);

        UG_DrawLine(abs_rect.xs + d2 + d - 1, abs_rect.ys + d + 1, abs_rect.xs + d + 1, abs_rect.ys + d2 + d - 1, c);
        UG_DrawLine(abs_rect.xs + d2 + d - 1, abs_rect.ys + d, abs_rect.xs + d, abs_rect.ys + d2 + d - 1, c);
        UG_DrawLine(abs_rect.xs + d2 + d - 2, abs_rect.ys + d, abs_rect.xs + d, abs_rect.ys + d2 + d - 2, c);

        /* Draw Checkbox frame */
        if (cb->base_object.style & STYLE_3D)
        {  /* 3D */
            UG2_Draw3DObjectFrame(abs_rect.xs, abs_rect.ys, abs_rect.xs + d2 + 2 * d - 1, abs_rect.ys + d2 + 2 * d - 1, (tick == 1) ? _ug2_checkbox_pressed_border_theme : _ug2_checkbox_released_border_theme);
            if (cb->base_object.style & STYLE_FOCUSED)
            {
                UG2_DrawDottedFrame(abs_rect.xs + 2, abs_rect.ys + 2, abs_rect.xe - 4, abs_rect.ye - 4, 2, C_BLACK);
            }
        }
        else
        {  /* 2D */
            UG2_DrawFrame(abs_rect.xs, abs_rect.ys, abs_rect.xs + d2 + 2 * d - 1, abs_rect.ys + d2 + 2 * d - 1, (tick == 1) ? cb->base_object.colors.background : cb->base_object.colors.foreground);
        }
    }
    else
    {
        UG2_FillFrame(abs_rect.xs + d, abs_rect.ys + d, abs_rect.xe - d, abs_rect.ye - d, cb->base_object.colors.background);
    }

    return UG_RESULT_OK;
}

static UG2_RESULT _UG2_CheckboxHandleMessage(UG2_MESSAGE* msg)
{
    if (!msg || !msg->obj) return UG_RESULT_ARG;

    UG2_CHECKBOX* cb = UG2_CAST_OBJ_AS_CHECKBOX(msg->obj);

    switch (msg->type)
    {
        /* repaint all */
    case MSG_REDRAW:
        cb->base_object.busy = 1;
        _UG2_CheckboxHandleRedraw(cb);
        cb->base_object.busy = 0;
        return UG_RESULT_OK;

    case MSG_KEY_DOWN:
        if (msg->id != ' ') return UG_RESULT_MSG_UNHANDLED;
        /* TODO: something different */
        __fallthrough;
        /* fallthrough */

        /* press */
    case MSG_TOUCH_DOWN:
        cb->base_object.busy = 1;
        switch (cb->status)
        {
        case UG2_CHECKBOX_STATUS_GRAYED:
            cb->status = UG2_CHECKBOX_STATUS_GRAYED_PRESSED;
            break;
        case UG2_CHECKBOX_STATUS_TICK:
            cb->status = UG2_CHECKBOX_STATUS_TICK_PRESSED;
            break;
        case UG2_CHECKBOX_STATUS_UNTICK:
            cb->status = UG2_CHECKBOX_STATUS_UNTICK_PRESSED;
            break;

        default:
            cb->base_object.busy = 0;
            return UG_RESULT_OK;
        }
        cb->base_object.busy = 0;
        return UG2_SendMessage(msg->obj, MSG_REDRAW, 0, 0, 0, NULL);

    case MSG_KEY_UP:
        if (msg->id != ' ') return UG_RESULT_MSG_UNHANDLED;
        /* TODO: something different */
        __fallthrough;
        /* fallthrough */

        /* release press */
    case MSG_TOUCH_UP:
        cb->base_object.busy = 1;
        switch (cb->status)
        {
        case UG2_CHECKBOX_STATUS_GRAYED_PRESSED:
            cb->status = UG2_CHECKBOX_STATUS_GRAYED;
            break;
        case UG2_CHECKBOX_STATUS_TICK_PRESSED:
            cb->status = UG2_CHECKBOX_STATUS_UNTICK; /* change state */
            break;
        case UG2_CHECKBOX_STATUS_UNTICK_PRESSED:
            cb->status = UG2_CHECKBOX_STATUS_TICK; /* change state */
            break;

        default:
            cb->base_object.busy = 0;
            return UG_RESULT_OK;
        }
        cb->base_object.busy = 0;
        return UG2_SendMessage(msg->obj, MSG_REDRAW, 0, 0, 0, NULL);

    default:
        return UG_RESULT_MSG_UNHANDLED;
    }
}

UG2_RESULT UG2_CheckboxInitialize(UG2_CHECKBOX* cb,
    UG2_OBJECT* parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message)
{
    UG2_RESULT res = UG2_GenericObjectInitialize(
        UG2_BaseObject(cb),
        parent,
        x,
        y,
        width,
        height,
        &_UG2_CheckboxHandleMessage,
        OBJ_TYPE_WINDOW);

    if (res != UG_RESULT_OK) return res;

    /* Initialize window */
    cb->base_object.user_handler = handle_message;

    UG2_RectFromDims(&cb->base_object.rect, x, y, width, height);

    cb->status = UG2_CHECKBOX_STATUS_UNTICK;

    cb->base_object.style =
        STYLE_ENABLED |
        STYLE_3D |
        STYLE_CAN_FOCUS;

    return UG_RESULT_OK;
}
