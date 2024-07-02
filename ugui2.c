
#include "ugui2.h"
#include <stdlib.h>

/* Pointer to the gui */
//static UG2_GUI* gui;
static UG2_GUI _gui;



/* -------------------------------------------------------------------------------- */
/* -- INTERNAL API FUNCTIONS                                                         -- */
/* -------------------------------------------------------------------------------- */

static UG_U16 ptr_8to16(const UG_U8* p) {
    UG_U16 d = *p++;
    return ((d << 8) | *p);
}


/*
 *  Load char bitmap address into p, return the font width
 */
static UG_S16 _UG2_GetCharData(UG2_CHAR encoding, const UG_U8** p) {
    static UG2_CHAR last_encoding;
    static UG_S16 last_width;
    static const UG_U8* last_p;
    static UG2_FONT* last_font;
    UG_U16 start = 0;
    UG_U16 skip = 0;
    UG_U16 t = 0;
    UG_U8 range = 0;
    UG_U8 found = 0;

    if (_gui.currentFont.font == last_font && encoding == last_encoding) {       // If called with the same arguments, return cached data
        if (p) {
            *p = last_p;                                                    // Load char bitmap address
        }
        return last_width;
    }

    if (_gui.currentFont.is_old_font) {                                      // Compatibility with old fonts charset
        switch (encoding)
        {
        case 0xF6: encoding = 0x94; break; // ö
        case 0xD6: encoding = 0x99; break; // Ö
        case 0xFC: encoding = 0x81; break; // ü
        case 0xDC: encoding = 0x9A; break; // Ü
        case 0xE4: encoding = 0x84; break; // ä
        case 0xC4: encoding = 0x8E; break; // Ä
        case 0xB5: encoding = 0xE6; break; // µ
        case 0xB0: encoding = 0xF8; break; // °
        }
    }

    if (_gui.currentFont.font_type == FONT_TYPE_LIC_1BPP)
    {
        UG_FONT_LIC* lic_font = (UG_FONT_LIC*)_gui.currentFont.font;

        if (encoding < lic_font->first_char || encoding > lic_font->last_char) {
            return -1;
        }

        for (t = 0; t < lic_font->num_of_chars; t++)
        {
            if (lic_font->info[t].code == encoding)
            { // symbol found
                last_font = _gui.currentFont.font;                // Update cached data
                last_encoding = encoding;
                last_width = lic_font->info[t].width;
                last_p = lic_font->info[t].data;
                if (p) {
                    *p = last_p;                                       // Load char bitmap address
                }
                return(last_width);                                // Return char width
            }
        }
        return -1;                                             // -1 = char not found
    }

    for (; t < _gui.currentFont.number_of_offsets; t++)                         // Seek through the offsets
    {
        UG_U16 curr_offset = ptr_8to16(_gui.currentFont.offsets + (t * 2));    // Offsets are 16-bit, splitted in 2 byte values

        if (curr_offset & 0x8000)                                          // If the offset has the MSB bit set, it means it's the a range start
        {
            start = curr_offset & 0x7FFF;                                     // Store range start
            range = 1;                                                      // Set flag
        }
        else if (range)                                                  // If range previously set, this is the range end
        {
            if (encoding >= start && encoding <= curr_offset)            // If the encoding is between the range
            {
                skip += (encoding - start);                             // Calculate the skip value
                found = 1;
                break;
            }
            else if (encoding < start)                                 // If the encoding is lower than current range start, the char is not in the font
                break;

            skip += ((curr_offset - start) + 1);                        // Encoding not found in the current range, increase skip size and clear range flasg
            range = 0;
        }
        else                                                            // Range not set, this is a single char offset
        {
            if (encoding == curr_offset)                                     // If matching the current offset char
            {
                found = 1;
                break;
            }
            else if (encoding < curr_offset)                                // If the encoding is lower than current range, the char is not in the font
            {
                break;
            }
            skip++;                                                       // Else, increase skip and keep searching
        }
    }

    if (found)                                                         // If char found
    {
        last_font = _gui.currentFont.font;                                     // Update cached data
        last_encoding = encoding;
        last_p = (_gui.currentFont.data + (skip * _gui.currentFont.bytes_per_char));
        if (_gui.currentFont.widths) {                                                // If width table available
            last_width = *(_gui.currentFont.widths + skip);                        // Use width from table
        }
        else {
            last_width = _gui.currentFont.char_width;                            // Else use width from char width
        }


        if (p) {
            *p = last_p;                                                    // Load char bitmap address
        }
        return(last_width);                                             // Return char width
    }
    return -1;                                                        // -1 = char not found
}


static UG_S16 _UG2_PutChar(UG2_CHAR chr, UG_S16 x, UG_S16 y, UG2_COLOR fc, UG2_COLOR bc)
{
    UG2_POS_T x0 = 0, y0 = 0, i, j, k, bn, fpixels = 0, bpixels = 0;
    UG_S16 c;
    UG2_POS_T xd, yd;                                   /* printed char width, hight */
    UG_U32 cnt = 0;
    UG_U8 pix, old_code;
    UG_U8 b, trans = _gui.transparent_font, driver = (_gui.driver[DRIVER_FILL_AREA].state & DRIVER_ENABLED);
    const UG_U8* data;                              // Pointer to current char bitmap
#if defined(UGUI_USE_COLOR_RGB888) || defined(UGUI_USE_COLOR_RGB565)
    UG2_COLOR color;
#endif
    PushPixelsFunc push_pixels = NULL;

    if ((x > _gui.device->x_dim - 1) || (y > _gui.device->y_dim - 1))
        return -1;                                   /* outside area */

    UG_S16 actual_char_width = _UG2_GetCharData(chr, &data);
    if (actual_char_width == -1)
        return -1;                                     // Char not presnt in the font

    bn = _gui.currentFont.char_width;
    if (!bn) {
        return 0;
    }
    bn >>= 3;
    if (_gui.currentFont.char_width % 8) bn++;

    /* The case when the symbol goes beyond the screen */
    xd = actual_char_width;
    if (x + actual_char_width > _gui.device->x_dim)
        xd = _gui.device->x_dim - x;

    yd = _gui.currentFont.char_height;
    if (y + _gui.currentFont.char_height > _gui.device->y_dim)
        yd = _gui.device->y_dim - y;

    /* Is hardware acceleration available? */
    if (driver)
    {
        push_pixels = ((DriverFillAreaFunct)_gui.driver[DRIVER_FILL_AREA].driver)(x, y, x + xd - 1, y + yd - 1);
    }

    if (_gui.currentFont.font_type == FONT_TYPE_1BPP || _gui.currentFont.font_type == FONT_TYPE_LIC_1BPP)
    {
        old_code = !(_gui.currentFont.font_type == FONT_TYPE_LIC_1BPP);
        for (j = 0; j < yd; j++)
        {
            c = 0;
            for (i = 0; old_code ? (i < bn) : 1; i++)
            {
                if (old_code)
                    b = *data++;
                for (k = 0; old_code ? ((k < 8) && c < xd) : (k < actual_char_width); k++)
                {
                    if (!old_code) {
                        if (!(cnt++ % 8))
                            b = *data++;
                        pix = b & 0x80;
                        b <<= 1;
                        if (c >= xd)
                            continue;
                    }
                    else {
                        pix = b & 0x01;
                        b >>= 1;
                    }

                    if (pix)                         // Foreground pixel detected
                    {
                        if (driver)
                        {                              // Accelerated output
                            if (bpixels && !trans)        // Draw accumulated background pixels, only if transparent mode disabled
                            {
                                push_pixels(bpixels, bc);   // Drawing accumulated pixels removes a lot of overhead, drawing speed is at least 3x faster
                                bpixels = 0;
                            }
                            if (!fpixels && trans)        // Store first foreground pixel position for transparent drawing
                            {
                                x0 = x + c;
                                y0 = y + j;
                            }
                            fpixels++;                   // Instead writing every pixel, count consecutive pixels, then send accumulated pixels in a single transaction
                        }
                        else
                        {                              // Not accelerated output
                            _gui.device->pset(x + c, y + j, fc);
                        }
                    }
                    else                             // Background pixel detected
                    {
                        if (driver)
                        {                              // Accelerated output
                            if (fpixels)                  // Draw accumulated foreground pixels
                            {
                                if (!trans)                 // In transparent mode, we don't need to address pixel areas, so just keep pushing pixels
                                {
                                    push_pixels(fpixels, fc);
                                    fpixels = 0;
                                }
                                else {                                // In transparent mode, drawing needs to be broken in smaller parts, avoiding the background areas
                                    while (fpixels)
                                    {
                                        UG_U16 width = (x + xd) - x0;         // Detect available pixels in the current row from current x position
                                        if (x0 == x || fpixels < width)                       // If pixel draw count is lower than available pixels, or drawing at start of the row, drawn as-is
                                        {
                                            push_pixels = ((DriverFillAreaFunct)_gui.driver[DRIVER_FILL_AREA].driver)(x0, y0, x0 + width - 1, y0 + (fpixels / actual_char_width));
                                            push_pixels(fpixels, fc);
                                            fpixels = 0;
                                        }
                                        else                                             // If  pixel draw count is higher than available pixels, there's at least second line, drawn this row first
                                        {
                                            push_pixels = ((DriverFillAreaFunct)_gui.driver[DRIVER_FILL_AREA].driver)(x0, y0, x0 + width - 1, y0);
                                            push_pixels(fpixels, fc);
                                            fpixels -= width;
                                            x0 = x;
                                            y0++;
                                        }
                                    }
                                }
                            }
                            bpixels++;
                        }
                        else if (!trans)                           // Not accelerated output
                        {
                            _gui.device->pset(x + c, y + j, bc);
                        }
                    }
                    c++;
                }
                if (!old_code)
                    break;
            }
        }
        if (driver) {                                            // After finishing, ensure there're no remaining pixels left, make another pass
            if (bpixels && !trans)
            {
                push_pixels(bpixels, bc);
            }
            else if (fpixels)
            {
                if (!trans)
                {
                    push_pixels(fpixels, fc);
                }
                else
                {
                    while (fpixels)
                    {
                        UG_U16 width = (x + xd) - x0;
                        if (x0 == x || fpixels < width)
                        {
                            push_pixels = ((DriverFillAreaFunct)_gui.driver[DRIVER_FILL_AREA].driver)(x0, y0, x0 + width - 1, y0 + (fpixels / actual_char_width));
                            push_pixels(fpixels, fc);
                            fpixels = 0;
                        }
                        else
                        {
                            push_pixels = ((DriverFillAreaFunct)_gui.driver[DRIVER_FILL_AREA].driver)(x0, y0, x0 + width - 1, y0);
                            push_pixels(fpixels, fc);
                            fpixels -= width;
                            x0 = x;
                            y0++;
                        }
                    }
                }
            }
        }
    }
#if defined(UGUI_USE_COLOR_RGB888) || defined(UGUI_USE_COLOR_RGB565)
    else if (_gui.currentFont.font_type == FONT_TYPE_8BPP)
    {
        for (j = 0; j < _gui.currentFont.char_height; j++)
        {
            for (i = 0; i < actual_char_width; i++)
            {
                b = *data++;
                if (j < yd && i < xd)
                {
                    color = ((((fc & 0xFF) * b + (bc & 0xFF) * (256 - b)) >> 8) & 0xFF) |            //Blue component
                        ((((fc & 0xFF00) * b + (bc & 0xFF00) * (256 - b)) >> 8) & 0xFF00) |     //Green component
                        ((((fc & 0xFF0000) * b + (bc & 0xFF0000) * (256 - b)) >> 8) & 0xFF0000); //Red component
                    if (driver)
                    {
                        push_pixels(1, color);                                                          // Accelerated output
                    }
                    else
                    {
                        _gui.device->pset(x + i, y + j, color);                                                // Not accelerated output
                    }
                }
            }
            data += _gui.currentFont.char_width - actual_char_width;
        }
    }
#endif
    return (actual_char_width);
}


/*
 * Updates the current font data
 */
static void _UG2_FontSelect(UG2_FONT* font) {
    if (_gui.currentFont.font == font)
        return;
    _gui.currentFont.font = font;                          // Save Font pointer
    _gui.currentFont.font_type = (FONT_TYPE)(0x7F & *font);// Byte    0: Font_type
    if (_gui.currentFont.font_type == FONT_TYPE_LIC_1BPP)
    {
        UG_FONT_LIC* lic_font = (UG_FONT_LIC*)font;

        _gui.currentFont.is_old_font = 0;
        _gui.currentFont.char_width = lic_font->char_width;
        _gui.currentFont.char_height = lic_font->char_height;
        _gui.currentFont.number_of_chars = lic_font->num_of_chars;
        _gui.currentFont.number_of_offsets = 0;
        _gui.currentFont.bytes_per_char = 0;
        _gui.currentFont.widths = NULL;
        _gui.currentFont.offsets = NULL;
        _gui.currentFont.data = (const UG_U8*)lic_font->info;
        return;
    }
    _gui.currentFont.is_old_font = (0x80 & *font++) && 1;    // Byte    0: Bit 7 indicates old or new font type. 1=old font, 0=new font
    _gui.currentFont.char_width = *font++;                 // Byte    1: Char width
    _gui.currentFont.char_height = *font++;                // Byte    2: Char height
    _gui.currentFont.number_of_chars = ptr_8to16(font);    // Bytes 3+4: Number of chars
    font += 2;
    _gui.currentFont.number_of_offsets = ptr_8to16(font);  // Bytes 5+6: Number of offsets
    font += 2;
    _gui.currentFont.bytes_per_char = ptr_8to16(font);     // Bytes 7+8: Bytes per char
    font += 2;
    if (*font++) {                                    // Byte 9: 1=Width table present, 0=not present
        _gui.currentFont.widths = font;                      // Save pointer to width table
        font += _gui.currentFont.number_of_chars;              // Increase number of chars
    }
    else {
        _gui.currentFont.widths = NULL;                      // No width table
    }
    _gui.currentFont.offsets = font;                       // Save pointer to offset table
    font += (_gui.currentFont.number_of_offsets * 2);        // Increase pointer by number of offsets*2 (2-byte values)
    _gui.currentFont.data = font;                          // Save pointer to bitmap data
}

void UG2_PutText(UG2_TEXT* txt)
{
    if (!txt->font || !txt->str) {
        return;
    }

    UG2_POS_T ye = txt->area.ye;
    UG2_POS_T ys = txt->area.ys;
    UG2_POS_T char_height = UG2_GetFontHeight(txt->font);

    if ((ye - ys) < char_height) {
        return;
    }

    UG2_POS_T sl, rc, wl;
    UG2_POS_T xp, yp;
    UG2_POS_T xs = txt->area.xs;
    UG2_POS_T xe = txt->area.xe;
    UG_U8  align = txt->align;
    UG2_POS_T char_h_space = txt->h_space;
    UG2_POS_T char_v_space = txt->v_space;
    UG2_POS_T w;
    UG2_CHAR chr;
    const char* str = txt->str;
    const char* c = str;

    _UG2_FontSelect(txt->font);

    rc = 1;
    c = str;

    while (1)
    {
#ifdef UGUI_USE_UTF8
        if (!_gui.currentFont.is_old_font) {                // Old font charset compatibility
            chr = _UG_DecodeUTF8(&c);
        }
        else {
            chr = *c++;
        }
#else
        chr = *c++;
#endif
        if (!chr) break;
        if (chr == '\n') rc++;
    }

    yp = 0;
    if (align & (ALIGN_V_CENTER | ALIGN_V_BOTTOM))
    {
        yp = ye - ys + 1;
        yp -= char_height * rc;
        yp -= char_v_space * (rc - 1);
        if (yp < 0) {
            return;
        }
    }
    if (align & ALIGN_V_CENTER) yp >>= 1;
    yp += ys;

    while (1)
    {
        sl = 0;
        c = str;
        wl = 0;
        while (1)
        {
#ifdef UGUI_USE_UTF8
            if (!_gui.currentFont.is_old_font) {                // Old font charset compatibility
                chr = _UG_DecodeUTF8(&c);
            }
            else {
                chr = *c++;
            }
#else
            chr = *c++;
#endif
            if (chr == 0 || chr == '\n') {
                break;
            }
            w = _UG2_GetCharData(chr, NULL);
            if (w == -1) { continue; }
            sl++;
            wl += w + char_h_space;
        }
        wl -= char_h_space;

        xp = xe - xs + 1;
        xp -= wl;
        if (xp < 0) break;

        if (align & ALIGN_H_LEFT) xp = 0;
        else if (align & ALIGN_H_CENTER) xp >>= 1;
        xp += xs;


        while (1) {
#ifdef UGUI_USE_UTF8
            if (!_gui.currentFont.is_old_font) {                // Old font charset compatibility
                chr = _UG_DecodeUTF8(&str);
            }
            else {
                chr = *str++;
            }
#else
            chr = *str++;
#endif
            if (chr == 0) {
                return;
            }
            else if (chr == '\n') {
                break;
            }
            w = _UG2_PutChar(chr, xp, yp, txt->colors.foreground, txt->colors.background);
            if (w != -1)
                xp += w + char_h_space;
        }
        yp += char_height + char_v_space;
    }
}



UG2_COLOR UG2_GuiGetDesktopColor(void)
{
    return _gui.desktop_color;
}

UG2_OBJECT* UG2_GuiGetActiveWindow(void)
{
    return _gui.active_window;
}

void UG2_GuiSetActiveWindow(UG2_OBJECT* wnd)
{
    _gui.active_window = wnd;
}


void UG2_FillFrame(UG2_POS_T x1, UG2_POS_T y1, UG2_POS_T x2, UG2_POS_T y2, const UG2_COLOR c)
{
    UG_S16 n, m;

    if (x2 < x1)
        UG2_swap(x1, x2);
    if (y2 < y1)
        UG2_swap(y1, y2);

    /* Is hardware acceleration available? */
    if (_gui.driver[DRIVER_FILL_FRAME].state & DRIVER_ENABLED)
    {
        //if (((UG2_RESULT(*)(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c))_gui.driver[DRIVER_FILL_FRAME].driver)(x1, y1, x2, y2, c) == UG_RESULT_OK) return;
    }

    for (m = y1; m <= y2; m++)
    {
        for (n = x1; n <= x2; n++)
        {
            _gui.device->pset(n, m, c);
        }
    }
}

void UG2_FillScreen(UG2_COLOR c)
{
    UG2_FillFrame(0, 0, _gui.device->x_dim - 1, _gui.device->y_dim - 1, c);
}


void UG_DrawLine(UG2_POS_T x1, UG2_POS_T y1, UG2_POS_T x2, UG2_POS_T y2, const UG2_COLOR c)
{
    UG2_POS_T n, dx, dy, sgndx, sgndy, dxabs, dyabs, x, y, drawx, drawy;

    /* Is hardware acceleration available? */
    if (_gui.driver[DRIVER_DRAW_LINE].state & DRIVER_ENABLED)
    {
        //if (((UG_RESULT(*)(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c))_gui.driver[DRIVER_DRAW_LINE].driver)(x1, y1, x2, y2, c) == UG_RESULT_OK) return;
    }

    dx = x2 - x1;
    dy = y2 - y1;
    dxabs = (dx > 0) ? dx : -dx;
    dyabs = (dy > 0) ? dy : -dy;
    sgndx = (dx > 0) ? 1 : -1;
    sgndy = (dy > 0) ? 1 : -1;
    x = dyabs >> 1;
    y = dxabs >> 1;
    drawx = x1;
    drawy = y1;

    _gui.device->pset(drawx, drawy, c);

    if (dxabs >= dyabs)
    {
        for (n = 0; n < dxabs; n++)
        {
            y += dyabs;
            if (y >= dxabs)
            {
                y -= dxabs;
                drawy += sgndy;
            }
            drawx += sgndx;
            _gui.device->pset(drawx, drawy, c);
        }
    }
    else
    {
        for (n = 0; n < dyabs; n++)
        {
            x += dxabs;
            if (x >= dyabs)
            {
                x -= dyabs;
                drawx += sgndx;
            }
            drawy += sgndy;
            _gui.device->pset(drawx, drawy, c);
        }
    }
}

void UG2_Draw3DObjectFrame(UG2_POS_T xs, UG2_POS_T ys, UG2_POS_T xe, UG2_POS_T ye, const UG2_COLOR_RECT* frame_colors)
{
    // Frame 0
    UG_DrawLine(xs, ys, xe - 1, ys, frame_colors[0].top);
    UG_DrawLine(xs, ys + 1, xs, ye - 1, frame_colors[0].left);
    UG_DrawLine(xs, ye, xe, ye, frame_colors[0].bottom);
    UG_DrawLine(xe, ys, xe, ye - 1, frame_colors[0].right);
    // Frame 1
    UG_DrawLine(xs + 1, ys + 1, xe - 2, ys + 1, frame_colors[1].top);
    UG_DrawLine(xs + 1, ys + 2, xs + 1, ye - 2, frame_colors[1].left);
    UG_DrawLine(xs + 1, ye - 1, xe - 1, ye - 1, frame_colors[1].bottom);
    UG_DrawLine(xe - 1, ys + 1, xe - 1, ye - 2, frame_colors[1].right);
    // Frame 2
    UG_DrawLine(xs + 2, ys + 2, xe - 3, ys + 2, frame_colors[2].top);
    UG_DrawLine(xs + 2, ys + 3, xs + 2, ye - 3, frame_colors[2].left);
    UG_DrawLine(xs + 2, ye - 2, xe - 2, ye - 2, frame_colors[2].bottom);
    UG_DrawLine(xe - 2, ys + 2, xe - 2, ye - 3, frame_colors[2].right);
}




UG2_RESULT UG2_GenericObjectInitialize(
    UG2_OBJECT* obj,
    UG2_OBJECT* parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message,
    UG2_OBJECT_TYPES_TYPE type)
{
    if (obj == NULL) return UG_RESULT_ARG;

    obj->user_handler = NULL;

    UG2_RectFromDims(&obj->rect, x, y, width, height);

    obj->colors.foreground = C_FORE_COLOR;
    obj->colors.background = C_BACK_COLOR;
    obj->parent = parent;
    obj->child = NULL;
    obj->child_cnt = 0;
    obj->focused_child = NULL;

    obj->handle_message = handle_message;
    obj->type = type;
    obj->style = STYLE_DISABLED | STYLE_HIDDEN;

    return UG_RESULT_OK;
}









UG2_RESULT UG2_SendMessage(UG2_OBJECT* obj, UG_U16 type, UG_U8 id, UG_U8 sub_id, UG_U8 event, void* data)
{
    if (!obj) return UG_RESULT_ARG;

    if (!obj->handle_message) return UG_RESULT_MSG_UNHANDLED;

    UG2_MESSAGE* msg = (UG2_MESSAGE*)malloc(sizeof(UG2_MESSAGE));
    if (!msg) return UG_RESULT_NO_MEM;

    msg->obj = obj;
    msg->data = data;
    msg->event = event;
    msg->id = id;
    msg->sub_id = sub_id;
    msg->type = type;

    // TODO: use queues
    //enqueue((node_t**)_gui.message_pump, (void*)msg);

    UG2_RESULT res = UG_RESULT_MSG_UNHANDLED;
    if (obj->handle_message) obj->handle_message(msg);

    free(msg);

    return res;
}

UG2_RESULT _UG2_DefaultHandleMessage(UG2_MESSAGE* msg)
{
    return UG_RESULT_MSG_UNHANDLED;
}



UG2_RESULT UG2_Init(UG2_DEVICE* device)
{
    UG_U8 i;

    _gui.device = device;
    _gui.char_h_space = 1;
    _gui.char_v_space = 1;
    _gui.font = NULL;
    _gui.currentFont.bytes_per_char = 0;
    _gui.currentFont.char_height = 0;
    _gui.currentFont.char_width = 0;
    _gui.currentFont.number_of_chars = 0;
    _gui.currentFont.number_of_offsets = 0;
    _gui.currentFont.widths = NULL;
    _gui.currentFont.offsets = NULL;
    _gui.currentFont.data = NULL;
    _gui.currentFont.font = NULL;
    _gui.desktop_color = C_DESKTOP_COLOR;
    _gui.colors.foreground = C_WHITE;
    _gui.colors.background = C_BLACK;
    _gui.next_window = NULL;
    _gui.active_window = NULL;
    _gui.last_window = NULL;

    _gui.message_pump = NULL;

    /* Clear drivers */
    for (i = 0; i < NUMBER_OF_DRIVERS; i++)
    {
        _gui.driver[i].driver = NULL;
        _gui.driver[i].state = 0;
    }

    // fill the background with the desktop color
    UG2_FillScreen(_gui.desktop_color);

    return UG_RESULT_OK;
}

