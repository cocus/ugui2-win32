#include "ugui2_button.h"

static const UG2_COLOR_RECT _ug2_checkbox_pressed_border_theme[3] = { C_PAL_BUTTON_PRESSED };

static const UG2_COLOR_RECT _ug2_checkbox_released_border_theme[3] = { C_PAL_BUTTON_RELEASED };

static UG2_RESULT _UG2_ButtonHandleRedraw(UG2_BUTTON* btn)
{
    UG2_POS_T xs, ys, xe, ye;
    UG2_RECT abs_rect = { 0, 0, 0, 0 };
    UG2_TEXT txt;
    UG2_POS_T d = 0, o = 0;

    if (btn == NULL) return UG_RESULT_ARG;

    UG2_GetObjectScreenRect(UG2_BaseObject(btn), &abs_rect);

    UG2_PosFromRect(
        &abs_rect,
        xs, ys, xe, ye);

    /* Is the window visible? */
    if (btn->base_object.style & STYLE_VISIBLE)
    {
        /* 3D or 2D style? */
        d = (btn->base_object.style & STYLE_3D) ? 3 : 1;

        txt.colors= btn->base_object.colors;

        UG2_FillFrame(abs_rect.xs + d, abs_rect.ys + d, abs_rect.xe - d, abs_rect.ye - d, txt.colors.background);

        /* Draw button text */
        txt.area.xs = abs_rect.xs + d + o;
        txt.area.ys = abs_rect.ys + d + o;
        txt.area.xe = abs_rect.xe - d + o;
        txt.area.ye = abs_rect.ye - d + o;
        txt.align = btn->base_object.text_align;
        txt.font = btn->base_object.font;
        txt.h_space = 2;
        txt.v_space = 2;
        txt.str = btn->base_object.text;
        UG2_PutText(&txt);

        /* Draw button frame */
        if (btn->base_object.style & STYLE_3D)
        {  /* 3D */
            UG2_Draw3DObjectFrame(abs_rect.xs, abs_rect.ys, abs_rect.xe, abs_rect.ye, (btn->pressed) ? _ug2_checkbox_pressed_border_theme : _ug2_checkbox_released_border_theme);
            if (btn->base_object.style & STYLE_FOCUSED)
            {
                UG2_DrawDottedFrame(abs_rect.xs + 2, abs_rect.ys + 2, abs_rect.xe - 4, abs_rect.ye - 4, 2, C_BLACK);
            }
        }
        else
        {  /* 2D */
            UG2_DrawFrame(abs_rect.xs, abs_rect.ys, abs_rect.xe, abs_rect.ye, (btn->pressed) ? btn->base_object.colors.background : btn->base_object.colors.foreground);
        }
    }
    else
    {
        UG2_FillFrame(abs_rect.xs + d, abs_rect.ys + d, abs_rect.xe - d, abs_rect.ye - d, btn->base_object.colors.background);
    }

    return UG_RESULT_OK;
}

static UG2_RESULT _UG2_ProgressHandleMessage(UG2_MESSAGE* msg)
{
    if (!msg || !msg->obj) return UG_RESULT_ARG;

    UG2_BUTTON* btn = UG2_CAST_OBJ_AS_BUTTON(msg->obj);

    switch (msg->type)
    {
        /* repaint all */
    case MSG_REDRAW:
        btn->base_object.busy = 1;
        _UG2_ButtonHandleRedraw(btn);
        btn->base_object.busy = 0;
        return UG_RESULT_OK;

        /* press */
    case MSG_TOUCH_DOWN:
        btn->base_object.busy = 1;
        if (btn->pressed == 0)
        {
            btn->pressed = 1;
            btn->base_object.busy = 0;
            return UG2_SendMessage(msg->obj, MSG_REDRAW, 0, 0, 0, NULL);
        }
        btn->base_object.busy = 0;
        return UG_RESULT_OK;

        /* release press */
    case MSG_TOUCH_UP:
        btn->base_object.busy = 1;
        if (btn->pressed == 1)
        {
            btn->pressed = 0;
            btn->base_object.busy = 0;
            return UG2_SendMessage(msg->obj, MSG_REDRAW, 0, 0, 0, NULL);
        }
        btn->base_object.busy = 0;
        return UG_RESULT_OK;

    default:
        return UG_RESULT_MSG_UNHANDLED;
    }
}

UG2_RESULT UG2_ButtonInitialize(UG2_BUTTON* btn,
    UG2_OBJECT* parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message)
{
    UG2_RESULT res = UG2_GenericObjectInitialize(
        UG2_BaseObject(btn),
        parent,
        x,
        y,
        width,
        height,
        &_UG2_ProgressHandleMessage,
        OBJ_TYPE_WINDOW);

    if (res != UG_RESULT_OK) return res;

    /* Initialize window */
    btn->base_object.user_handler = handle_message;

    UG2_RectFromDims(&btn->base_object.rect, x, y, width, height);

    btn->base_object.style =
        STYLE_ENABLED |
        STYLE_3D |
        STYLE_CAN_FOCUS;

    return UG_RESULT_OK;
}
