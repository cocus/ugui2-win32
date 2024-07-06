// ugui-win32.cpp : Define el punto de entrada de la aplicación.
//

#include <stdio.h>
#include <string.h>
#include "windowsx.h"
#include "framework.h"
#include "ugui2-win32.h"

#include "ugui2.h"
#include "ugui2_fonts.h"
#include "ugui2_window.h"
#include "ugui2_progress.h"
#include "ugui2_button.h"
#include "ugui2_checkbox.h"

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

    // Define window styles
    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX | WS_VISIBLE;

    // Calculate the required size of the window rectangle based on desired client area size
    RECT windowRect = { 0, 0, 640, 200 };
    AdjustWindowRectEx(&windowRect, dwStyle, FALSE, 0);

    _main_hwnd = CreateWindowEx(
        0,
        szWindowClass,
        szTitle,
        dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowRect.right - windowRect.left,  // Adjusted width
        windowRect.bottom - windowRect.top,  // Adjusted height
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!_main_hwnd)
    {
        return FALSE;
    }

    CenterWindow(_main_hwnd, GetDesktopWindow());
    ShowWindow(_main_hwnd, nCmdShow);
    UpdateWindow(_main_hwnd);

    return TRUE;
}






#define MAX_OBJS 15

// Global Vars
UG2_DEVICE device;


UG2_WINDOW wnd;
UG2_PROGRESS pgb0;
UG2_BUTTON btn0;
UG2_CHECKBOX chb0;
/*
UG_BUTTON btn1;
UG_BUTTON btn2;
UG_BUTTON btn3;
UG_CHECKBOX chb1;
UG_CHECKBOX chb2;
UG_CHECKBOX chb3;
UG_TEXTBOX txt0;
UG_TEXTBOX txt1;
UG_TEXTBOX txt2;
UG_TEXTBOX txt3;
UG_PROGRESS pgb1;*/
UG2_OBJECT objs[MAX_OBJS];

#define INITIAL_MARGIN 3
#define BTN_WIDTH 100
#define BTN_HEIGHT 30
#define CHB_WIDTH 100
#define CHB_HEIGHT 14

#define OBJ_Y(i) BTN_HEIGHT *i + ((i + 1))
UG2_RESULT btn_handler(UG2_MESSAGE* msg)
{
    if (!msg) return UG_RESULT_ARG;

    if (msg->obj == UG2_BaseObject(&btn0))
    {
        UG_U8 pb = 0;
        if ((msg->type == MSG_TOUCH_UP) ||
            ((msg->type == MSG_KEY_UP) && (msg->id = ' ')))
        {
            UG2_ProgressGetProgress(&pgb0, &pb);

            if (++pb == 101)
            {
                pb = 0;
            }

            UG2_ProgressSetProgress(&pgb0, pb);
        }
    }

    return UG_RESULT_MSG_UNHANDLED;
}
char buffer[64] = { '\0' };

void GUI_DemoSetup(UG2_DEVICE* device)
{
    // Setup Window
    UG2_WindowInitialize(&wnd, NULL, 120, 0, (640-120), 200, NULL);
    UG2_ObjectSetFont(UG2_BaseObject(&wnd), FONT_6X8);
    UG2_ObjectSetText(UG2_BaseObject(&wnd), "uGUI2 - Window Title!");
    UG2_ShowObject(UG2_BaseObject(&wnd));

    // Progress Bar
    UG2_ProgressInitialize(&pgb0, UG2_BaseObject(&wnd), 10, 10, 80, 20, NULL);
#ifndef UGUI2_USE_COLOR_BW
    UG2_ObjectSetForeColor(UG2_BaseObject(&pgb0), C_DARK_BLUE);
#endif
    UG2_ProgressSetProgress(&pgb0, 50);
    UG2_ShowObject(UG2_BaseObject(&pgb0));

    // Buttons
    UG2_ButtonInitialize(&btn0, UG2_BaseObject(&wnd), 10, 35, 80, 40, btn_handler);
    UG2_ObjectSetFont(UG2_BaseObject(&btn0), FONT_6X8);
    UG2_ObjectSetText(UG2_BaseObject(&btn0), "Button!");
    UG2_ShowObject(UG2_BaseObject(&btn0));

    UG2_CheckboxInitialize(&chb0, UG2_BaseObject(&wnd), 10, 80, 80, CHB_HEIGHT, NULL);
    UG2_ObjectSetFont(UG2_BaseObject(&chb0), FONT_6X8);
    UG2_ObjectSetText(UG2_BaseObject(&chb0), "Checkbox");
    UG2_ShowObject(UG2_BaseObject(&chb0));


#if 0



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
}

void GUI_Process()
{
    //UG_Update();
}


// Internal
void esp32_lcd_thing_pset(UG2_POS_T x, UG2_POS_T y, const UG2_COLOR c)
{
#ifdef UGUI2_USE_COLOR_BW
    SetPixel(_hMemDC, x, y, c == C_WHITE ? RGB(0xff, 0xff, 0xff) : RGB(0, 0, 0));
#else
    uint32_t clr_r = (c & 0x00ff0000) >> 16;
    uint32_t clr_g = (c & 0x0000ff00) >> 8;
    uint32_t clr_b = (c & 0x000000ff) >> 0;

    SetPixel(_hMemDC, x, y, RGB(clr_r, clr_g, clr_b));
#endif
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

        // Setup UGUI
        device.x_dim = 640;
        device.y_dim = 200;
        device.pset = &esp32_lcd_thing_pset;
        device.flush = &esp32_lcd_thing_flush;

        // Setup UGUI
        UG2_Init(&device);

        GUI_DemoSetup(&device);

        SetTimer(hWnd, 69, 50, NULL);
        break;
    }
    case WM_TIMER:
    {
        if (wParam == 69)
        {
            esp32_lcd_thing_flush();
            //GUI_Process();
            return 0;
        }
        break;
    }

    case WM_KEYDOWN:
    {
        /* wParam is a VK_*, casting it to a value is not right, but will do */
        UG2_SystemSendMessage(MSG_KEY_DOWN, (UG_U8)(wParam), 0, 0, NULL);
        break;
    }
    case WM_KEYUP:
    {
        /* wParam is a VK_*, casting it to a value is not right, but will do */
        UG2_SystemSendMessage(MSG_KEY_UP, (UG_U8)(wParam), 0, 0, NULL);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        UG2_POINT p = { .x = GET_X_LPARAM(lParam), .y = GET_Y_LPARAM(lParam) };
        UG2_SystemSendMessage(MSG_TOUCH_DOWN, UG2_TOUCH_ID_MAIN, 0, 0, &p);
        break;
    }

    case WM_LBUTTONUP:
    {
        UG2_POINT p = { .x = GET_X_LPARAM(lParam), .y = GET_Y_LPARAM(lParam) };
        UG2_SystemSendMessage(MSG_TOUCH_UP, UG2_TOUCH_ID_MAIN, 0, 0, &p);
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
        BitBlt(hdc, 0, 0, 640, 200, _hMemDC, 0, 0, SRCCOPY);

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
