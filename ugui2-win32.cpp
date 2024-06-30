// ugui-win32.cpp : Define el punto de entrada de la aplicación.
//

#include <stdio.h>
#include <string.h>
#include "windowsx.h"
#include "framework.h"
#include "ugui2-win32.h"

#include "ugui2.h"

#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Colocar código aquí.

    // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_UGUI2WIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realizar la inicialización de la aplicación:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UGUI2WIN32));

    MSG msg;

    // Bucle principal de mensajes:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: Registra la clase de ventana.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UGUI2WIN32));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_UGUI2WIN32);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL CenterWindow(HWND hwndWindow, HWND hwndParent)
{
    RECT rectWindow, rectParent;

    // make the window relative to its parent
    if (hwndParent != NULL)
    {
        GetWindowRect(hwndWindow, &rectWindow);
        GetWindowRect(hwndParent, &rectParent);

        int nWidth = rectWindow.right - rectWindow.left;
        int nHeight = rectWindow.bottom - rectWindow.top;

        int nX = ((rectParent.right - rectParent.left) - nWidth) / 2 + rectParent.left;
        int nY = ((rectParent.bottom - rectParent.top) - nHeight) / 2 + rectParent.top;

        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        // make sure that the dialog box never moves outside of the screen
        if (nX < 0) nX = 0;
        if (nY < 0) nY = 0;
        if (nX + nWidth > nScreenWidth) nX = nScreenWidth - nWidth;
        if (nY + nHeight > nScreenHeight) nY = nScreenHeight - nHeight;

        MoveWindow(hwndWindow, nX, nY, nWidth, nHeight, FALSE);

        return TRUE;
    }

    return FALSE;
}

static HWND _main_hwnd = NULL;
static HDC _hMemDC = NULL;
static HBITMAP _hbmScreen = NULL;
//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: Guarda el identificador de instancia y crea la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Almacenar identificador de instancia en una variable global

    _main_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 260, NULL, NULL, hInstance, NULL);

    if (!_main_hwnd)
    {
        return FALSE;
    }
    CenterWindow(_main_hwnd, GetDesktopWindow());
    ShowWindow(_main_hwnd, nCmdShow);
    UpdateWindow(_main_hwnd);

    HDC wndhDC = GetDC(_main_hwnd);
    _hMemDC = CreateCompatibleDC(wndhDC);

    // Create a compatible bitmap from the Window DC.
    _hbmScreen = CreateCompatibleBitmap(wndhDC, 640, 200);

    if (!_hbmScreen)
    {
        MessageBox(_main_hwnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
    }
    else
    {
        // Select the compatible bitmap into the compatible memory DC.
        SelectObject(_hMemDC, _hbmScreen);
    }


    return TRUE;
}






#define MAX_OBJS 15

// Global Vars
UG2_DEVICE device;
UG2_GUI ugui;

// to ugui2.c

#include "ugui2_queue.h"


/* Pointer to the gui */
static UG2_GUI* gui;

UG_S16 UG2_Init(UG2_GUI* g, UG2_DEVICE* device)
{
    UG_U8 i;

    g->device = device;
    g->char_h_space = 1;
    g->char_v_space = 1;
    g->font = NULL;
    g->currentFont.bytes_per_char = 0;
    g->currentFont.char_height = 0;
    g->currentFont.char_width = 0;
    g->currentFont.number_of_chars = 0;
    g->currentFont.number_of_offsets = 0;
    g->currentFont.widths = NULL;
    g->currentFont.offsets = NULL;
    g->currentFont.data = NULL;
    g->currentFont.font = NULL;
    g->desktop_color = C_AQUA;
    g->fore_color = C_WHITE;
    g->back_color = C_BLACK;
    g->next_window = NULL;
    g->active_window = NULL;
    g->last_window = NULL;

    g->message_pump = NULL;

    /* Clear drivers */
    for (i = 0; i < NUMBER_OF_DRIVERS; i++)
    {
        g->driver[i].driver = NULL;
        g->driver[i].state = 0;
    }

    gui = g;
    return 1;
}

void UG2_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG2_COLOR c)
{
    UG_S16 n, m;

    if (x2 < x1)
        UG2_swap(x1, x2);
    if (y2 < y1)
        UG2_swap(y1, y2);

    /* Is hardware acceleration available? */
    if (gui->driver[DRIVER_FILL_FRAME].state & DRIVER_ENABLED)
    {
        //if (((UG2_RESULT(*)(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c))gui->driver[DRIVER_FILL_FRAME].driver)(x1, y1, x2, y2, c) == UG_RESULT_OK) return;
    }

    for (m = y1; m <= y2; m++)
    {
        for (n = x1; n <= x2; n++)
        {
            gui->device->pset(n, m, c);
        }
    }
}


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
UG_S16 _UG2_GetCharData(UG2_CHAR encoding, const UG_U8** p) {
    static UG2_CHAR last_encoding;
    static UG_S16 last_width;
    static const UG_U8* last_p;
    static UG2_FONT* last_font;
    UG_U16 start = 0;
    UG_U16 skip = 0;
    UG_U16 t = 0;
    UG_U8 range = 0;
    UG_U8 found = 0;

    if (gui->currentFont.font == last_font && encoding == last_encoding) {       // If called with the same arguments, return cached data
        if (p) {
            *p = last_p;                                                    // Load char bitmap address
        }
        return last_width;
    }

    if (gui->currentFont.is_old_font) {                                      // Compatibility with old fonts charset
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

    if (gui->currentFont.font_type == FONT_TYPE_LIC_1BPP)
    {
        UG_FONT_LIC* lic_font = (UG_FONT_LIC*)gui->currentFont.font;

        if (encoding < lic_font->first_char || encoding > lic_font->last_char) {
            return -1;
        }

        for (t = 0; t < lic_font->num_of_chars; t++)
        {
            if (lic_font->info[t].code == encoding)
            { // symbol found
                last_font = gui->currentFont.font;                // Update cached data
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

    for (; t < gui->currentFont.number_of_offsets; t++)                         // Seek through the offsets
    {
        UG_U16 curr_offset = ptr_8to16(gui->currentFont.offsets + (t * 2));    // Offsets are 16-bit, splitted in 2 byte values

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
        last_font = gui->currentFont.font;                                     // Update cached data
        last_encoding = encoding;
        last_p = (gui->currentFont.data + (skip * gui->currentFont.bytes_per_char));
        if (gui->currentFont.widths) {                                                // If width table available
            last_width = *(gui->currentFont.widths + skip);                        // Use width from table
        }
        else {
            last_width = gui->currentFont.char_width;                            // Else use width from char width
        }


        if (p) {
            *p = last_p;                                                    // Load char bitmap address
        }
        return(last_width);                                             // Return char width
    }
    return -1;                                                        // -1 = char not found
}


UG_S16 _UG2_PutChar(UG2_CHAR chr, UG_S16 x, UG_S16 y, UG2_COLOR fc, UG2_COLOR bc)
{
    UG2_POS_T x0 = 0, y0 = 0, i, j, k, bn, fpixels = 0, bpixels = 0;
    UG_S16 c;
    UG2_POS_T xd, yd;                                   /* printed char width, hight */
    UG_U32 cnt = 0;
    UG_U8 pix, old_code;
    UG_U8 b, trans = gui->transparent_font, driver = (gui->driver[DRIVER_FILL_AREA].state & DRIVER_ENABLED);
    const UG_U8* data;                              // Pointer to current char bitmap
#if defined(UGUI_USE_COLOR_RGB888) || defined(UGUI_USE_COLOR_RGB565)
    UG2_COLOR color;
#endif
    PushPixelsFunc push_pixels = NULL;

    if ((x > gui->device->x_dim - 1) || (y > gui->device->y_dim - 1))
        return -1;                                   /* outside area */

    UG_S16 actual_char_width = _UG2_GetCharData(chr, &data);
    if (actual_char_width == -1)
        return -1;                                     // Char not presnt in the font

    bn = gui->currentFont.char_width;
    if (!bn) {
        return 0;
    }
    bn >>= 3;
    if (gui->currentFont.char_width % 8) bn++;

    /* The case when the symbol goes beyond the screen */
    xd = actual_char_width;
    if (x + actual_char_width > gui->device->x_dim)
        xd = gui->device->x_dim - x;

    yd = gui->currentFont.char_height;
    if (y + gui->currentFont.char_height > gui->device->y_dim)
        yd = gui->device->y_dim - y;

    /* Is hardware acceleration available? */
    if (driver)
    {
        push_pixels = ((DriverFillAreaFunct)gui->driver[DRIVER_FILL_AREA].driver)(x, y, x + xd - 1, y + yd - 1);
    }

    if (gui->currentFont.font_type == FONT_TYPE_1BPP || gui->currentFont.font_type == FONT_TYPE_LIC_1BPP)
    {
        old_code = !(gui->currentFont.font_type == FONT_TYPE_LIC_1BPP);
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
                            gui->device->pset(x + c, y + j, fc);
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
                                            push_pixels = ((DriverFillAreaFunct)gui->driver[DRIVER_FILL_AREA].driver)(x0, y0, x0 + width - 1, y0 + (fpixels / actual_char_width));
                                            push_pixels(fpixels, fc);
                                            fpixels = 0;
                                        }
                                        else                                             // If  pixel draw count is higher than available pixels, there's at least second line, drawn this row first
                                        {
                                            push_pixels = ((DriverFillAreaFunct)gui->driver[DRIVER_FILL_AREA].driver)(x0, y0, x0 + width - 1, y0);
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
                            gui->device->pset(x + c, y + j, bc);
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
                            push_pixels = ((DriverFillAreaFunct)gui->driver[DRIVER_FILL_AREA].driver)(x0, y0, x0 + width - 1, y0 + (fpixels / actual_char_width));
                            push_pixels(fpixels, fc);
                            fpixels = 0;
                        }
                        else
                        {
                            push_pixels = ((DriverFillAreaFunct)gui->driver[DRIVER_FILL_AREA].driver)(x0, y0, x0 + width - 1, y0);
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
    else if (gui->currentFont.font_type == FONT_TYPE_8BPP)
    {
        for (j = 0; j < gui->currentFont.char_height; j++)
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
                        gui->device->pset(x + i, y + j, color);                                                // Not accelerated output
                    }
                }
            }
            data += gui->currentFont.char_width - actual_char_width;
        }
    }
#endif
    return (actual_char_width);
}


/*
 * Updates the current font data
 */
void _UG2_FontSelect(UG2_FONT* font) {
    if (gui->currentFont.font == font)
        return;
    gui->currentFont.font = font;                          // Save Font pointer
    gui->currentFont.font_type = (FONT_TYPE)(0x7F & *font);// Byte    0: Font_type
    if (gui->currentFont.font_type == FONT_TYPE_LIC_1BPP)
    {
        UG_FONT_LIC* lic_font = (UG_FONT_LIC*)font;

        gui->currentFont.is_old_font = 0;
        gui->currentFont.char_width = lic_font->char_width;
        gui->currentFont.char_height = lic_font->char_height;
        gui->currentFont.number_of_chars = lic_font->num_of_chars;
        gui->currentFont.number_of_offsets = 0;
        gui->currentFont.bytes_per_char = 0;
        gui->currentFont.widths = NULL;
        gui->currentFont.offsets = NULL;
        gui->currentFont.data = (const UG_U8*)lic_font->info;
        return;
    }
    gui->currentFont.is_old_font = (0x80 & *font++) && 1;    // Byte    0: Bit 7 indicates old or new font type. 1=old font, 0=new font
    gui->currentFont.char_width = *font++;                 // Byte    1: Char width
    gui->currentFont.char_height = *font++;                // Byte    2: Char height
    gui->currentFont.number_of_chars = ptr_8to16(font);    // Bytes 3+4: Number of chars
    font += 2;
    gui->currentFont.number_of_offsets = ptr_8to16(font);  // Bytes 5+6: Number of offsets
    font += 2;
    gui->currentFont.bytes_per_char = ptr_8to16(font);     // Bytes 7+8: Bytes per char
    font += 2;
    if (*font++) {                                    // Byte 9: 1=Width table present, 0=not present
        gui->currentFont.widths = font;                      // Save pointer to width table
        font += gui->currentFont.number_of_chars;              // Increase number of chars
    }
    else {
        gui->currentFont.widths = NULL;                      // No width table
    }
    gui->currentFont.offsets = font;                       // Save pointer to offset table
    font += (gui->currentFont.number_of_offsets * 2);        // Increase pointer by number of offsets*2 (2-byte values)
    gui->currentFont.data = font;                          // Save pointer to bitmap data
}

void _UG2_PutText(UG2_TEXT* txt)
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
        if (!gui->currentFont.is_old_font) {                // Old font charset compatibility
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
            if (!gui->currentFont.is_old_font) {                // Old font charset compatibility
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
            if (!gui->currentFont.is_old_font) {                // Old font charset compatibility
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
            w = _UG2_PutChar(chr, xp, yp, txt->color_foreground, txt->color_background);
            if (w != -1)
                xp += w + char_h_space;
        }
        yp += char_height + char_v_space;
    }
}


void UG_DrawLine(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG2_COLOR c)
{
    UG_S16 n, dx, dy, sgndx, sgndy, dxabs, dyabs, x, y, drawx, drawy;

    /* Is hardware acceleration available? */
    if (gui->driver[DRIVER_DRAW_LINE].state & DRIVER_ENABLED)
    {
        //if (((UG_RESULT(*)(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c))gui->driver[DRIVER_DRAW_LINE].driver)(x1, y1, x2, y2, c) == UG_RESULT_OK) return;
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

    gui->device->pset(drawx, drawy, c);

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
            gui->device->pset(drawx, drawy, c);
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
            gui->device->pset(drawx, drawy, c);
        }
    }
}

UG2_RESULT UG2_SendMessage(UG2_OBJECT* obj, UG_U16 type, UG_U8 id, UG_U8 sub_id, UG_U8 event, void* data)
{
    UG2_MESSAGE* msg = (UG2_MESSAGE*)malloc(sizeof(UG2_MESSAGE));
    if (!msg) return UG_RESULT_FAIL;
    msg->obj = obj;


    enqueue(gui->message_pump, (void*)msg);
}

// ugui2.c end


// to ugui2_window.c
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
    UG2_COLOR color_foreground;
    UG2_COLOR color_background;
    UG2_COLOR ifc;
    UG2_COLOR ibc;
    UG2_POS_T height;
} UG_TITLE;

/* Window structure */
struct S_WINDOW
{
    UG2_OBJECT hdr; /* this is an object after all */

    UG_U16 state;
    UG2_COLOR color_border;
    UG2_COLOR color_background;

    UG_TITLE title;

    UG2_HandleMessage user_handler;
};

typedef struct S_WINDOW                               UG2_WINDOW;
#define UG2_CAST_WINDOW_AS_OBJ(wnd) (UG2_OBJECT*)(wnd)
#define UG2_CAST_OBJ_AS_WINDOW(obj) (UG2_WINDOW*)(obj)


/* -------------------------------------------------------------------------------- */
/* -- WINDOW FUNCTIONS                                                           -- */
/* -------------------------------------------------------------------------------- */


static UG2_RESULT _UG2_WindowDrawTitleBar(UG2_WINDOW* wnd)
{
    UG2_TEXT txt;
    UG2_POS_T xs, ys, xe, ye;

    if (!wnd) return UG_RESULT_ARG;

    xs = wnd->hdr.area_abs.xs;
    ys = wnd->hdr.area_abs.ys;
    xe = wnd->hdr.area_abs.xe;
    ye = wnd->hdr.area_abs.ye;

    /* 3D style? */
    if (wnd->hdr.style & STYLE_3D)
    {
        xs += 3;
        ys += 3;
        xe -= 3;
        ye -= 3;
    }

    /* Is the window active or inactive? */
    if (wnd->hdr.style & STYLE_FOCUSED)
    {
        txt.color_background = wnd->title.color_background;
        txt.color_foreground = wnd->title.color_foreground;
    }
    else
    {
        txt.color_background = wnd->title.ibc;
        txt.color_foreground = wnd->title.ifc;
    }

    /* Draw title */
    UG2_FillFrame(xs, ys, xe, ys + wnd->title.height - 1, txt.color_background);

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
    _UG2_PutText(&txt);

    /* Draw line */
    UG_DrawLine(xs, ys + wnd->title.height, xe, ys + wnd->title.height, C_WHITE_63);
    return UG_RESULT_OK;
}

static UG2_RESULT _UG2_WindowHandleMessage(UG2_MESSAGE* msg)
{
    if (!msg || !msg->obj) return UG_RESULT_ARG;

    UG2_WINDOW* wnd = UG2_CAST_OBJ_AS_WINDOW(msg->obj);

    switch (msg->type)
    {
    case MSG_UPDATE: /* repaint all */
        wnd->hdr.busy = 1;
        if (wnd->hdr.style & STYLE_TITLEBAR)
        {
            _UG2_WindowDrawTitleBar(wnd);
        }
        wnd->hdr.busy = 0;

        return UG_RESULT_OK;
    default:
        break;
    }

    return UG_RESULT_MSG_UNHANDLED;
}

UG2_RESULT UG2_WindowInitialize(UG2_WINDOW* wnd,
    UG2_OBJECT* parent,
    UG2_POS_T x,
    UG2_POS_T y,
    UG2_POS_T width,
    UG2_POS_T height,
    UG2_HandleMessage handle_message)
{
    UG_U8 i;
    UG2_OBJECT* obj = NULL;

    if (wnd == NULL) return UG_RESULT_ARG;

    /* if no windows are created */
    if (parent == NULL) gui->active_window = UG2_CAST_WINDOW_AS_OBJ(wnd);

    /* Initialize window */
    wnd->color_border = 0x5c5a58;
    wnd->color_background = C_WHITE;
    wnd->user_handler = handle_message;

    wnd->hdr.handle_message = &_UG2_WindowHandleMessage;
    wnd->hdr.area_abs.xs = x;
    wnd->hdr.area_abs.ys = y;
    wnd->hdr.area_abs.xe = x + width;
    wnd->hdr.area_abs.ye = y + height;
    wnd->hdr.parent = parent;
    wnd->hdr.child = NULL;
    wnd->hdr.focused_child = NULL;
    wnd->hdr.type = OBJ_TYPE_WINDOW;

    wnd->hdr.style = STYLE_ENABLED | STYLE_TITLEBAR | STYLE_FLAT;

    /* Initialize window title-bar */
    wnd->title.str = NULL;

    wnd->title.h_space = 2;
    wnd->title.v_space = 2;
    wnd->title.align = ALIGN_CENTER_LEFT;
    wnd->title.color_foreground = wnd->color_background;
    wnd->title.color_background = wnd->color_border;
    wnd->title.ifc = 0;// C_INACTIVE_TITLE_FORE_COLOR;
    wnd->title.ibc = 0;// C_INACTIVE_TITLE_BACK_COLOR;
    wnd->title.height = 15;

    return UG_RESULT_OK;
}

UG2_RESULT UG2_WindowShow(UG2_WINDOW* wnd)
{
    if (wnd == NULL) return UG_RESULT_ARG;
    UG2_SendMessage()
}





// ugui2_window.c end








UG2_WINDOW wnd;
/*UG_BUTTON btn0;
UG_BUTTON btn1;
UG_BUTTON btn2;
UG_BUTTON btn3;
UG_CHECKBOX chb0;
UG_CHECKBOX chb1;
UG_CHECKBOX chb2;
UG_CHECKBOX chb3;
UG_TEXTBOX txt0;
UG_TEXTBOX txt1;
UG_TEXTBOX txt2;
UG_TEXTBOX txt3;
UG_PROGRESS pgb0;
UG_PROGRESS pgb1;*/
UG2_OBJECT objs[MAX_OBJS];

#define INITIAL_MARGIN 3
#define BTN_WIDTH 100
#define BTN_HEIGHT 30
#define CHB_WIDTH 100
#define CHB_HEIGHT 14

#define OBJ_Y(i) BTN_HEIGHT *i + ((i + 1))
void windowHandler(UG2_MESSAGE* msg)
{
    static UG_U16 x0, y0;

    (void)x0;
    (void)y0;

    // decode_msg(msg);
}
#if 0

#if defined(UGUI_USE_TOUCH)
    if (msg->type == MSG_TYPE_OBJECT)
    {
        UG_OBJECT* obj = msg->src;
        if (obj)
        {
            if (obj->touch_state & OBJ_TOUCH_STATE_CHANGED)
                printf("|CHANGED");
            if (obj->touch_state & OBJ_TOUCH_STATE_PRESSED_ON_OBJECT)
                printf("|PRESSED_ON_OBJECT");
            if (obj->touch_state & OBJ_TOUCH_STATE_PRESSED_OUTSIDE_OBJECT)
                printf("|PRESSED_OUTSIDE_OBJECT");
            if (obj->touch_state & OBJ_TOUCH_STATE_RELEASED_ON_OBJECT)
                printf("|RELEASED_ON_OBJECT");
            if (obj->touch_state & OBJ_TOUCH_STATE_RELEASED_OUTSIDE_OBJECT)
                printf("|RELEASED_OUTSIDE_OBJECT");
            if (obj->touch_state & OBJ_TOUCH_STATE_IS_PRESSED_ON_OBJECT)
                printf("|IS_PRESSED_ON_OBJECT");
            if (obj->touch_state & OBJ_TOUCH_STATE_IS_PRESSED)
                printf("|IS_PRESSED");
            if (obj->touch_state & OBJ_TOUCH_STATE_INIT)
                printf("|INIT");
            printf("\n");
            if (obj->touch_state & OBJ_TOUCH_STATE_IS_PRESSED)
            {
                x0 = UG_GetGUI()->touch.xp;
                y0 = UG_GetGUI()->touch.yp;
            }
        }

        if (UG_ProgressGetProgress(&wnd, PGB_ID_0) == 100)
            UG_ProgressSetProgress(&wnd, PGB_ID_0, 0);
        else
            UG_ProgressSetProgress(&wnd, PGB_ID_0, UG_ProgressGetProgress(&wnd, PGB_ID_0) + 1);

        if (UG_ProgressGetProgress(&wnd, PGB_ID_1) == 100)
            UG_ProgressSetProgress(&wnd, PGB_ID_1, 0);
        else
            UG_ProgressSetProgress(&wnd, PGB_ID_1, UG_ProgressGetProgress(&wnd, PGB_ID_1) + 1);
    }
#endif
}
#endif
char buffer[64] = { '\0' };

void GUI_Setup(UG2_DEVICE* device)
{
    // Setup Window
    UG2_WindowInitialize(&wnd, NULL, 120, 0, (640-120), 200, NULL);
#if 0
    wnd.xs = 120;
    UG_WindowSetTitleTextFont(&wnd, FONT_6X8);
    UG_WindowSetTitleText(&wnd, "App Title");

    // Buttons
    UG_ButtonCreate(&wnd, &btn0, BTN_ID_0, UGUI_POS(INITIAL_MARGIN, OBJ_Y(0), BTN_WIDTH, BTN_HEIGHT));
    UG_ButtonSetFont(&wnd, BTN_ID_0, FONT_6X8);
    UG_ButtonSetText(&wnd, BTN_ID_0, "Btn 3D");
    UG_ButtonSetStyle(&wnd, BTN_ID_0, BTN_STYLE_3D);

    UG_OBJECT* obj = _UG_SearchObject(&wnd, OBJ_TYPE_BUTTON, BTN_ID_0);
    UG_SetFocus(obj);

    UG_ButtonCreate(&wnd, &btn1, BTN_ID_1, UGUI_POS(INITIAL_MARGIN, OBJ_Y(1), BTN_WIDTH, BTN_HEIGHT));
    UG_ButtonSetFont(&wnd, BTN_ID_1, FONT_6X8);
    UG_ButtonSetText(&wnd, BTN_ID_1, "Btn 2D T");
    UG_ButtonSetStyle(&wnd, BTN_ID_1, BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS);

    UG_ButtonCreate(&wnd, &btn2, BTN_ID_2, UGUI_POS(INITIAL_MARGIN, OBJ_Y(2), BTN_WIDTH, BTN_HEIGHT));
    UG_ButtonSetFont(&wnd, BTN_ID_2, FONT_6X8);
    UG_ButtonSetText(&wnd, BTN_ID_2, "Btn 3D Alt");
    UG_ButtonSetStyle(&wnd, BTN_ID_2, BTN_STYLE_3D | BTN_STYLE_USE_ALTERNATE_COLORS);
    UG_ButtonSetAlternateForeColor(&wnd, BTN_ID_2, C_BLACK);
    UG_ButtonSetAlternateBackColor(&wnd, BTN_ID_2, C_WHITE);

    UG_ButtonCreate(&wnd, &btn3, BTN_ID_3, UGUI_POS(INITIAL_MARGIN, OBJ_Y(3), BTN_WIDTH, BTN_HEIGHT));
    UG_ButtonSetFont(&wnd, BTN_ID_3, FONT_6X8);
    UG_ButtonSetText(&wnd, BTN_ID_3, "Btn NoB");
    UG_ButtonSetStyle(&wnd, BTN_ID_3, BTN_STYLE_NO_BORDERS | BTN_STYLE_TOGGLE_COLORS);

    // Checkboxes
    UG_CheckboxCreate(&wnd, &chb0, CHB_ID_0, UGUI_POS(INITIAL_MARGIN + 50 + BTN_WIDTH, OBJ_Y(0) + 7, CHB_WIDTH, CHB_HEIGHT));
    UG_CheckboxSetFont(&wnd, CHB_ID_0, FONT_6X8);
    UG_CheckboxSetText(&wnd, CHB_ID_0, "CHB");
    UG_CheckboxSetStyle(&wnd, CHB_ID_0, CHB_STYLE_3D);
    UG_CheckboxSetAlignment(&wnd, CHB_ID_0, ALIGN_TOP_LEFT);
#if !defined(UGUI_USE_COLOR_BW)
    UG_CheckboxSetBackColor(&wnd, CHB_ID_0, C_PALE_TURQUOISE);
#endif

    UG_CheckboxCreate(&wnd, &chb1, CHB_ID_1, UGUI_POS(INITIAL_MARGIN + 50 + BTN_WIDTH, OBJ_Y(1) + 7, CHB_WIDTH, CHB_HEIGHT));
    UG_CheckboxSetFont(&wnd, CHB_ID_1, FONT_6X8);
    UG_CheckboxSetText(&wnd, CHB_ID_1, "CHB");
    UG_CheckboxSetStyle(&wnd, CHB_ID_1, CHB_STYLE_2D | CHB_STYLE_TOGGLE_COLORS);
    UG_CheckboxSetAlignment(&wnd, CHB_ID_1, ALIGN_CENTER);
    UG_CheckboxShow(&wnd, CHB_ID_1);

    UG_CheckboxCreate(&wnd, &chb2, CHB_ID_2, UGUI_POS(INITIAL_MARGIN + 50 + BTN_WIDTH, OBJ_Y(2) + 7, CHB_WIDTH, CHB_HEIGHT));
    UG_CheckboxSetFont(&wnd, CHB_ID_2, FONT_6X8);
    UG_CheckboxSetText(&wnd, CHB_ID_2, "CHB");
    UG_CheckboxSetStyle(&wnd, CHB_ID_2, CHB_STYLE_3D | CHB_STYLE_USE_ALTERNATE_COLORS);
    UG_CheckboxSetAlignment(&wnd, CHB_ID_2, ALIGN_BOTTOM_LEFT);
    UG_CheckboxShow(&wnd, CHB_ID_2);

    UG_CheckboxCreate(&wnd, &chb3, CHB_ID_3, UGUI_POS(INITIAL_MARGIN + 50 + BTN_WIDTH, OBJ_Y(3) + 7, CHB_WIDTH, CHB_HEIGHT));
    UG_CheckboxSetFont(&wnd, CHB_ID_3, FONT_6X8);
    UG_CheckboxSetText(&wnd, CHB_ID_3, "CHB");
    UG_CheckboxSetStyle(&wnd, CHB_ID_3, CHB_STYLE_NO_BORDERS | CHB_STYLE_TOGGLE_COLORS);
    UG_CheckboxSetAlignment(&wnd, CHB_ID_3, ALIGN_BOTTOM_RIGHT);
    UG_CheckboxShow(&wnd, CHB_ID_3);

    // Texts
    UG_TextboxCreate(&wnd, &txt0, TXB_ID_0, UGUI_POS(INITIAL_MARGIN * 2 + BTN_WIDTH + CHB_WIDTH, OBJ_Y(0), 100, 15));
    UG_TextboxSetFont(&wnd, TXB_ID_0, FONT_4X6);
    UG_TextboxSetText(&wnd, TXB_ID_0, "Small TEXT");
#if !defined(UGUI_USE_COLOR_BW)
    UG_TextboxSetBackColor(&wnd, TXB_ID_0, C_PALE_TURQUOISE);
#endif

    UG_TextboxCreate(&wnd, &txt1, TXB_ID_1, UGUI_POS(INITIAL_MARGIN + 50 + BTN_WIDTH + CHB_WIDTH, OBJ_Y(1) - 15, 200, 30));
    UG_TextboxSetFont(&wnd, TXB_ID_1, FONT_12X20);
    snprintf(buffer, sizeof(buffer), "%s", "date goes here"); //, esp_app_get_description()->time);
    UG_TextboxSetText(&wnd, TXB_ID_1, buffer);
#if !defined(UGUI_USE_COLOR_BW)
    UG_TextboxSetBackColor(&wnd, TXB_ID_1, C_PALE_TURQUOISE);
#endif
    UG_TextboxSetAlignment(&wnd, TXB_ID_1, ALIGN_TOP_RIGHT);

    UG_TextboxCreate(&wnd, &txt2, TXB_ID_2, UGUI_POS(INITIAL_MARGIN * 2 + BTN_WIDTH + CHB_WIDTH, OBJ_Y(2) - 15, 100, 45));
    UG_TextboxSetFont(&wnd, TXB_ID_2, FONT_24X40);
    UG_TextboxSetText(&wnd, TXB_ID_2, "Text");
#if !defined(UGUI_USE_COLOR_BW)
    UG_TextboxSetBackColor(&wnd, TXB_ID_2, C_PALE_TURQUOISE);
#endif

    UG_TextboxCreate(&wnd, &txt3, TXB_ID_3, UGUI_POS(INITIAL_MARGIN * 2 + BTN_WIDTH + CHB_WIDTH, OBJ_Y(3), 100, 53));
    UG_TextboxSetFont(&wnd, TXB_ID_3, FONT_32X53);
    UG_TextboxSetText(&wnd, TXB_ID_3, "50");
#if !defined(UGUI_USE_COLOR_BW)
    UG_TextboxSetBackColor(&wnd, TXB_ID_3, C_PALE_TURQUOISE);
#endif

    // Progress Bar
    UG_ProgressCreate(&wnd, &pgb0, PGB_ID_0, UGUI_POS(INITIAL_MARGIN, OBJ_Y(4) + 20, 157, 20));
    UG_ProgressSetProgress(&wnd, PGB_ID_0, 50);
    UG_ProgressSetForeColor(&wnd, PGB_ID_0, C_LIGHT_GREEN);

    UG_ProgressCreate(&wnd, &pgb1, PGB_ID_1, UGUI_POS(159 + INITIAL_MARGIN * 2, OBJ_Y(4) + 25, 156, 10));
    UG_ProgressSetStyle(&wnd, PGB_ID_1, PGB_STYLE_2D | PGB_STYLE_FORE_COLOR_MESH);
    UG_ProgressSetProgress(&wnd, PGB_ID_1, 75);
#endif
    UG2_WindowShow(&wnd);
}

void GUI_Process()
{
    //UG_Update();
}


// Internal
void esp32_lcd_thing_pset(UG_S16 x, UG_S16 y, UG2_COLOR c)
{
    uint32_t clr_r = (c & 0x00ff0000) >> 16;
    uint32_t clr_g = (c & 0x0000ff00) >> 8;
    uint32_t clr_b = (c & 0x000000ff) >> 0;


    SetPixel(_hMemDC, x, y, RGB(clr_r, clr_g, clr_b));
}

void esp32_lcd_thing_flush(void)
{
    RedrawWindow(_main_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
    // ESP_LOGI(TAG, "flush");
    //  i2s_parallel_flip_to_buffer(&I2S1, DrawBufID);
    //  DrawBufID ^= 1;
}


//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: Procesa mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {

        // Setup UGUI
        device.x_dim = 640;
        device.y_dim = 200;
        device.pset = &esp32_lcd_thing_pset;
        device.flush = &esp32_lcd_thing_flush;

        // Setup UGUI
        UG2_Init(&ugui, &device);

        // clear background
        //UG_FillScreen(C_WHITE);

        GUI_Setup(&device);

        SetTimer(hWnd, 69, 50, NULL);
        break;
    }
    case WM_TIMER:
    {
        if (wParam == 69)
        {
            //GUI_Process();
            return 0;
        }
        break;
    }

    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_TAB:
            //UG_FocusNext(&wnd);// _UG_SearchObject(&wnd, OBJ_TYPE_BUTTON, BTN_ID_0);
            break;
        }

        break;
    }
    case WM_LBUTTONDOWN:
    {
        //UG_TouchUpdate(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), TOUCH_STATE_PRESSED);
        break;
    }

    case WM_LBUTTONUP:
    {
        //UG_TouchUpdate(-1, -1, TOUCH_STATE_RELEASED);
        break;
    }

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Analizar las selecciones de menú:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Agregar cualquier código de dibujo que use hDC aquí...
        BitBlt(GetDC(_main_hwnd), 0, 0, 640, 200, _hMemDC, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
