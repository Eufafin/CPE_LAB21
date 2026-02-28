#include <windows.h>
#include <stdio.h>
#include <cstdlib>
HWND textfield,button,textbox1,textbox2;
COLORREF g_bgColor = RGB(100, 149, 237);
char textsaved[20];
char textsaved2[20];
char answer[20];
int err;
double x;
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch(Message) {
        case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Create a solid brush with our custom color
        HBRUSH hBrush = CreateSolidBrush(g_bgColor);

        // Fill the client area
        FillRect(hdc, &ps.rcPaint, hBrush);

        // Clean up
        DeleteObject(hBrush);
        EndPaint(hwnd, &ps);
        return 0;
        break;
    }
        case WM_CREATE:
            textfield = CreateWindow("STATIC","Please input two numbers",
                                        WS_VISIBLE | WS_CHILD | WS_BORDER,
                                        20,20,180,25,
                                        hwnd,NULL,NULL,NULL
                                        );
            textbox1 = CreateWindow("EDIT","",
                                    WS_BORDER|WS_CHILD|WS_VISIBLE,
                                    20,50,180,25,
                                    hwnd,NULL,NULL,NULL);
            textbox2 = CreateWindow("EDIT","",
                                    WS_BORDER|WS_CHILD|WS_VISIBLE,
                                    20,80,180,25,
                                    hwnd,NULL,NULL,NULL);

            button = CreateWindow("BUTTON",
                                    "+",
                                    WS_VISIBLE|WS_CHILD|WS_BORDER,
                                45,110,25,25,
                            hwnd,(HMENU) 1,NULL,NULL);
            button = CreateWindow("BUTTON",
                                    "-",
                                    WS_VISIBLE|WS_CHILD|WS_BORDER,
                                75,110,25,25,
                            hwnd,(HMENU) 2,NULL,NULL);
            button = CreateWindow("BUTTON",
                                    "*",
                                    WS_VISIBLE|WS_CHILD|WS_BORDER,
                                105,110,25,25,
                            hwnd,(HMENU) 3,NULL,NULL);
            button = CreateWindow("BUTTON",
                                    "/",
                                    WS_VISIBLE|WS_CHILD|WS_BORDER,
                                135,110,25,25,
                            hwnd,(HMENU) 4,NULL,NULL);
            break;

        case WM_COMMAND:
{
    double input1, input2, Ans;
    char textsaved[20], textsaved2[20], answer[50];

    GetWindowText(textbox1, textsaved, 20);
    GetWindowText(textbox2, textsaved2, 20);

    input1 = atof(textsaved);
    input2 = atof(textsaved2);

    switch (LOWORD(wParam)) {
        case 1: Ans = input1 + input2; break;
        case 2: Ans = input1 - input2; break;
        case 3: Ans = input1 * input2; break;
        case 4: if(input2 == 0){
                     err = 1;
                }else{
                    Ans = input1 / input2;
                }
                break;
         
        default: return 0;
    }
    err == 1 ? snprintf(answer, sizeof(answer), "Undefined") : snprintf(answer, sizeof(answer), "%.6f", Ans);
    err = 0;
    MessageBox(hwnd, answer, "Result", MB_OK);
}
break;
        /* Upon destruction, tell the main thread to stop */
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
       
        /* All other messages (a lot of them) are processed using default procedures */
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc; /* A properties struct of our window */
    HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
    MSG msg; /* A temporary location for all messages */

    /* zero out the struct and set the stuff we want to modify */
    memset(&wc,0,sizeof(wc));
    wc.cbSize    = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WndProc; /* This is where we will send messages to */
    wc.hInstance     = hInstance;
    wc.hCursor   = LoadCursor(NULL, IDC_ARROW);
   
    /* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = "WindowClass";
    wc.hIcon     = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
    wc.hIconSm   = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    // Create window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "WindowClass",
        "My Calculator",
        WS_VISIBLE|WS_SYSMENU,
        CW_USEDEFAULT, /* x */
        CW_USEDEFAULT, /* y */
        250, /* width */
        200, /* height */
        NULL,NULL,hInstance,NULL);

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    /*
        This is the heart of our program where all input is processed and
        sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
        this loop will not produce unreasonably high CPU usage
    */
    while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
        TranslateMessage(&msg); /* Translate key codes to chars if present */
        DispatchMessage(&msg); /* Send it to WndProc */
    }
    return msg.wParam;
}
