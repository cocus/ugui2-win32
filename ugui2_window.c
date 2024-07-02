#include "ugui2.h"
#include "ugui2_window.h"


/* -------------------------------------------------------------------------------- */
/* -- WINDOW FUNCTIONS                                                           -- */
/* -------------------------------------------------------------------------------- */

static const UG2_COLOR_RECT _ug2_window_default_border_theme[3] = { C_PAL_WINDOW };

static UG2_RESULT _UG2_WindowDrawTitle(UG2_WINDOW* wnd)
{
    UG2_TEXT txt;
    UG2_POS_T xs, ys, xe, ye;

    if (wnd == NULL) return UG_RESULT_ARG;

    UG2_PosFromRect(
        &UG2_CAST_WINDOW_AS_OBJ(wnd)->rect,
        xs, ys, xe, ye);

    /* 3D style? */
    if (UG2_CAST_WINDOW_AS_OBJ(wnd)->style & STYLE_3D)
    {
        xs += 3;
        ys += 3;
        xe -= 3;
        ye -= 3;
    }

    /* Is the window active or inactive? */
    if (UG2_CAST_WINDOW_AS_OBJ(wnd)->style & STYLE_FOCUSED)
    {
        txt.colors = wnd->title.colors_active;
    }
    else
    {
        txt.colors = wnd->title.colors_inactive;
    }

    /* Draw title */
    UG2_FillFrame(xs, ys, xe, ys + wnd->title.height - 1, txt.colors.background);

    /* Draw title text */
    txt.str = wnd->title.str;
    txt.font = wnd->title.font;
    txt.area.xs = xs + 3;
    txt.area.ys = ys;
    txt.area.xe = xe;
    txt.area.ye = ys + wnd->title.height - 1;
    txt.align = wnd->title.align;
    txt.h_space = wnd->title.h_space;
    txt.v_space = wnd->title.v_space;
    UG2_PutText(&txt);

    /* Draw line */
    UG_DrawLine(
        xs,
        ys + wnd->title.height,
        xe,
        ys + wnd->title.height,
        _ug2_window_default_border_theme[2].right);
    return UG_RESULT_OK;
}


static UG2_RESULT _UG2_WindowHandleRedraw(UG2_WINDOW* wnd)
{
    size_t i, objcnt;
    UG2_POS_T xs, ys, xe, ye;

    if (wnd == NULL) return UG_RESULT_ARG;

    UG2_PosFromRect(
        &UG2_CAST_WINDOW_AS_OBJ(wnd)->rect,
        xs, ys, xe, ye);

    /* Is the window visible? */
    if (wnd->base_object.style & STYLE_VISIBLE)
    {
        /* 3D style? */
        if ((wnd->base_object.style & STYLE_3D))
        {
            UG2_Draw3DObjectFrame(xs, ys, xe, ye, _ug2_window_default_border_theme);
            xs += 3;
            ys += 3;
            xe -= 3;
            ye -= 3;
        }

        /* Show title bar? */
        if (wnd->base_object.style & STYLE_TITLEBAR)
        {
            _UG2_WindowDrawTitle(wnd);
            ys += wnd->title.height + 1;
        }

        /* Draw window area? */
        UG2_FillFrame(xs, ys, xe, ye, wnd->base_object.colors.background);

        /* Force each object to be redrawn! */
        objcnt = wnd->base_object.child_cnt;
        for (i = 0; i < objcnt; i++)
        {
            UG2_SendMessage(&wnd->base_object.child[i], MSG_REDRAW, 0, 0, 0, NULL);
        }
    }
    else
    {
        UG2_FillFrame(wnd->base_object.rect.xs, wnd->base_object.rect.xs, wnd->base_object.rect.xe, wnd->base_object.rect.ye, UG2_GuiGetDesktopColor());
        // TODO: probably we should re-paint all objects as well
    }

    return UG_RESULT_OK;
}

static UG2_RESULT _UG2_WindowHandleMessage(UG2_MESSAGE* msg)
{
    if (!msg || !msg->obj) return UG_RESULT_ARG;

    UG2_WINDOW* wnd = UG2_CAST_OBJ_AS_WINDOW(msg->obj);
    UG2_RESULT res;

    if (msg->obj->user_handler)
    {
        wnd->base_object.busy = 1;
        res = msg->obj->user_handler(msg);
        wnd->base_object.busy = 0;

        if (res != UG_RESULT_MSG_UNHANDLED)
            return res;
    }

    switch (msg->type)
    {
        /* repaint all */
    case MSG_REDRAW:
        wnd->base_object.busy = 1;
        _UG2_WindowHandleRedraw(wnd);
        wnd->base_object.busy = 0;
        return UG_RESULT_OK;

        /* set text (titlebar) */
    case MSG_TEXT_SET:
        wnd->base_object.busy = 1;
        wnd->title.str = (const char*)msg->data;
        wnd->base_object.busy = 0;
        /* and redraw it ! */
        return UG2_SendMessage(UG2_CAST_WINDOW_AS_OBJ(wnd), MSG_REDRAW, 0, 0, 0, NULL);

        /* get text (titlebar) */
    case MSG_TEXT_GET:
        wnd->base_object.busy = 1;
        (const char*)msg->data = wnd->title.str;
        wnd->base_object.busy = 0;
        return UG_RESULT_OK;

        /* set text font (titlebar) */
    case MSG_FONT_SET:
        wnd->base_object.busy = 1;
        wnd->title.font = (UG2_FONT*)msg->data;
        wnd->base_object.busy = 0;
        /* and redraw it ! */
        return UG2_SendMessage(UG2_CAST_WINDOW_AS_OBJ(wnd), MSG_REDRAW, 0, 0, 0, NULL);


        /* get text font (titlebar) */
    case MSG_FONT_GET:
        wnd->base_object.busy = 1;
        (UG2_FONT*)msg->data = wnd->title.font;
        wnd->base_object.busy = 0;
        return UG_RESULT_OK;

        /* set the titlebar colors */
    case MSG_TITLEBAR_COLOR_SET:
        if (msg->id == UG2_COLORS_ACTIVE)
            wnd->title.colors_active = *(UG2_COLOR_FORE_BACK*)msg->data;
        else if (msg->id == UG2_COLORS_INACTIVE)
            wnd->title.colors_inactive = *(UG2_COLOR_FORE_BACK*)msg->data;
        else
            return UG_RESULT_ARG;
        /* and redraw it ! */
        return UG2_SendMessage(UG2_CAST_WINDOW_AS_OBJ(wnd), MSG_REDRAW, 0, 0, 0, NULL);

        /* get the titlebar colors */
    case MSG_TITLEBAR_COLOR_GET:
        if (msg->id == UG2_COLORS_ACTIVE)
            (UG2_COLOR_FORE_BACK*)msg->data = &wnd->title.colors_active;
        else if (msg->id == UG2_COLORS_INACTIVE)
            (UG2_COLOR_FORE_BACK*)msg->data = &wnd->title.colors_inactive;
        else
            return UG_RESULT_ARG;
        return UG_RESULT_OK;

        /* make this object visible & repaint */
    case MSG_SHOW:
        wnd->base_object.busy = 1;
        wnd->base_object.style |= STYLE_VISIBLE;
        wnd->base_object.busy = 0;
        /* and redraw it ! */
        return UG2_SendMessage(UG2_CAST_WINDOW_AS_OBJ(wnd), MSG_REDRAW, 0, 0, 0, NULL);

        /* make this object invisible & repaint */
    case MSG_HIDE:
        wnd->base_object.busy = 1;
        wnd->base_object.style &= ~STYLE_VISIBLE;
        wnd->base_object.busy = 0;
        return UG2_SendMessage(UG2_CAST_WINDOW_AS_OBJ(wnd), MSG_REDRAW, 0, 0, 0, NULL);

    default:
        return UG_RESULT_MSG_UNHANDLED;
    }
}

UG2_RESULT UG2_WindowInitialize(UG2_WINDOW* wnd,
    UG2_OBJECT* parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message)
{
    UG2_RESULT res = UG2_GenericObjectInitialize(
        UG2_CAST_WINDOW_AS_OBJ(wnd),
        parent,
        x,
        y,
        width,
        height,
        &_UG2_WindowHandleMessage,
        OBJ_TYPE_WINDOW);

    if (res != UG_RESULT_OK) return res;

    /* Initialize window */
    wnd->base_object.user_handler = handle_message;

    UG2_RectFromDims(&wnd->base_object.rect, x, y, width, height);

    wnd->base_object.style =
        STYLE_ENABLED |
        STYLE_TITLEBAR |
        STYLE_3D |
        (UG2_GuiGetActiveWindow() == NULL ? STYLE_FOCUSED : STYLE_UNFOCUSED);

    /* Initialize window title-bar */
    wnd->title.str = NULL;
    wnd->title.h_space = 2;
    wnd->title.v_space = 2;
    wnd->title.align = ALIGN_CENTER_LEFT;
    wnd->title.colors_active.foreground = C_TITLE_FORE_COLOR;
    wnd->title.colors_active.background = C_TITLE_BACK_COLOR;
    wnd->title.colors_inactive.foreground = C_INACTIVE_TITLE_FORE_COLOR;
    wnd->title.colors_inactive.background = C_INACTIVE_TITLE_BACK_COLOR;
    wnd->title.height = 15;

    /* if no windows are created, make this one the active window */
    if (parent == NULL) UG2_GuiSetActiveWindow(UG2_CAST_WINDOW_AS_OBJ(wnd));

    return UG_RESULT_OK;
}

UG2_RESULT UG2_WindowShow(UG2_WINDOW* wnd)
{
    if (wnd == NULL) return UG_RESULT_ARG;
    return UG2_SendMessage(UG2_CAST_WINDOW_AS_OBJ(wnd), MSG_SHOW, 0, 0, 0, NULL);
}

UG2_RESULT UG2_WindowSetTitleText(UG2_WINDOW* wnd, const char* str)
{
    if (wnd == NULL) return UG_RESULT_ARG;
    return UG2_SendMessage(UG2_CAST_WINDOW_AS_OBJ(wnd), MSG_TEXT_SET, 0, 0, 0, (void*)str);
}

UG2_RESULT UG2_WindowSetTitleFont(UG2_WINDOW* wnd, UG2_FONT* font)
{
    if (wnd == NULL) return UG_RESULT_ARG;
    return UG2_SendMessage(UG2_CAST_WINDOW_AS_OBJ(wnd), MSG_FONT_SET, 0, 0, 0, (void*)font);
}

// TODO: colos for titlebar, etc