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
        &UG2_BaseObject(wnd)->rect,
        xs, ys, xe, ye);

    /* 3D style? */
    if (UG2_BaseObject(wnd)->style & STYLE_3D)
    {
        xs += 3;
        ys += 3;
        xe -= 3;
        ye -= 3;
    }

    /* Is the window active or inactive? */
    if (UG2_BaseObject(wnd)->style & STYLE_FOCUSED)
    {
        txt.colors = wnd->title.colors_active;
    }
    else
    {
        txt.colors = wnd->title.colors_inactive;
    }

    if (UG2_BaseObject(wnd)->style & STYLE_BACKGROUND_MESH)
    {
        /* Draw mesh title background */
        UG2_DrawMesh(
            xs,
            ys,
            xe,
            ys + wnd->title.height - 1,
            4,
            txt.colors.background);
    }
    else if (UG2_BaseObject(wnd)->style & STYLE_BACKGROUND_BLEND)
    {
        /* TBD */
    }
    else
    {
        /* Draw solid title background */
        UG2_FillFrame(
            xs,
            ys,
            xe,
            ys + wnd->title.height - 1,
            txt.colors.background);
    }


    /* Draw title text */
    txt.str = wnd->base_object.text;
    txt.font = wnd->base_object.font;
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
    UG2_POS_T xs, ys, xe, ye;

    if (wnd == NULL) return UG_RESULT_ARG;

    UG2_PosFromRect(
        &UG2_BaseObject(wnd)->rect,
        xs, ys, xe, ye);

    /* Is the window visible? */
    if (wnd->base_object.style & STYLE_VISIBLE)
    {
        /* 3D style? */
        if ((wnd->base_object.style & STYLE_3D))
        {
            UG2_Draw3DObjectFrame(
                xs,
                ys,
                xe,
                ye,
                _ug2_window_default_border_theme);
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
        UG2_FillFrame(
            xs,
            ys,
            xe,
            ye,
            wnd->base_object.colors.background);
    }
    else
    {
        /* invisible so fill it with the background color? */
        UG2_FillFrame(
            wnd->base_object.rect.xs,
            wnd->base_object.rect.xs,
            wnd->base_object.rect.xe,
            wnd->base_object.rect.ye,
            UG2_GuiGetDesktopColor());
    }

    return UG_RESULT_OK;
}

static UG2_RESULT _UG2_WindowHandleMessage(UG2_MESSAGE* msg)
{
    if (!msg || !msg->obj) return UG_RESULT_ARG;

    UG2_WINDOW* wnd = UG2_CAST_OBJ_AS_WINDOW(msg->obj);
    UG2_RECT* rect = (UG2_RECT*)msg->data;

    switch (msg->type)
    {
        /* repaint all */
    case MSG_REDRAW:
        wnd->base_object.busy = 1;
        _UG2_WindowHandleRedraw(wnd);
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
        return UG2_SendMessage(UG2_BaseObject(wnd), MSG_REDRAW, 0, 0, 0, NULL);

        /* get the titlebar colors */
    case MSG_TITLEBAR_COLOR_GET:
        if (msg->id == UG2_COLORS_ACTIVE)
            *(UG2_COLOR_FORE_BACK**)msg->data = &wnd->title.colors_active;
        else if (msg->id == UG2_COLORS_INACTIVE)
            *(UG2_COLOR_FORE_BACK**)msg->data = &wnd->title.colors_inactive;
        else
            return UG_RESULT_ARG;
        return UG_RESULT_OK;

        /* get the client rect */
    case MSG_CLIENTRECT:
        if (rect == NULL) return UG_RESULT_ARG;

        *rect = wnd->base_object.rect;

        if (wnd->base_object.style & STYLE_3D)
        {
            rect->xs += 3;
            rect->ys += 3;
            rect->xe -= 3;
            rect->ye -= 3;
        }

        if (wnd->base_object.style & STYLE_TITLEBAR)
        {
            rect->ys += wnd->title.height + 1;
        }
        return UG_RESULT_OK;

    default:
        return UG_RESULT_MSG_UNHANDLED;
    }
}

UG2_RESULT UG2_WindowInitialize(
    UG2_WINDOW* wnd,
    UG2_OBJECT* parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message)
{
    UG2_RESULT res = UG2_GenericObjectInitialize(
        UG2_BaseObject(wnd),
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
        STYLE_CAN_FOCUS |
        (UG2_GuiGetActiveWindow() == NULL ? STYLE_FOCUSED : STYLE_UNFOCUSED);

    /* Initialize window title-bar */
    wnd->title.h_space = 2;
    wnd->title.v_space = 2;
    wnd->title.align = ALIGN_CENTER_LEFT;
    wnd->title.colors_active.foreground = C_TITLE_FORE_COLOR;
    wnd->title.colors_active.background = C_TITLE_BACK_COLOR;
    wnd->title.colors_inactive.foreground = C_INACTIVE_TITLE_FORE_COLOR;
    wnd->title.colors_inactive.background = C_INACTIVE_TITLE_BACK_COLOR;
    wnd->title.height = 15;

    /* if no windows are created, make this one the active window */
    if (parent == NULL) UG2_GuiSetActiveWindow(UG2_BaseObject(wnd));

    return UG_RESULT_OK;
}

// TODO: colos for titlebar, etc
