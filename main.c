#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "Manager.h"
#include "EditorMenu.h"


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS | CS_CLASSDC | CS_VREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "EditorMenu"; ;
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           400,                 /* The programs width */
           300,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           lpszArgument                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static EditorParameters* edPar = NULL;

    PAINTSTRUCT ps;
    HDC hdc;
    CREATESTRUCT* tmp;

    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        tmp = (CREATESTRUCT*)lParam;
        edPar = EditorParamInit();
        FillEditorParam(hwnd, edPar, (char* const)(tmp->lpCreateParams));
        break;

    case WM_SIZE:
        SizeChanged(hwnd, lParam, edPar);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        RedrawText(hwnd, hdc, ps, edPar);
        EndPaint(hwnd, &ps);

    case WM_SETFOCUS:
        CreateCaret(hwnd, (HBITMAP) NULL, edPar->outData->symbWidth, edPar->outData->symbHeight);
        SetCaretPos(edPar->caret->xInSymb * edPar->outData->symbWidth, edPar->caret->yInSymb * edPar->outData->symbHeight);
        ShowCaret(hwnd);
        break;

    case WM_VSCROLL:
        ScrollV(hwnd, wParam, edPar);
        SetCaretPos(edPar->caret->xInSymb * edPar->outData->symbWidth, edPar->caret->yInSymb * edPar->outData->symbHeight);
        ShowCaret(hwnd);
        break;

    case WM_HSCROLL:
        ScrollH(hwnd, wParam, edPar);
        SetCaretPos(edPar->caret->xInSymb * edPar->outData->symbWidth, edPar->caret->yInSymb * edPar->outData->symbHeight);
        printf("%i %i %i %i\n", edPar->caret->xInSymb, edPar->caret->yInSymb, edPar->caret->indOfString, edPar->caret->indOfSym);
        ShowCaret(hwnd);
        break;

    case WM_CHAR:
        EditorCharsProc(hwnd, wParam, edPar);
        break;

    case WM_KEYDOWN:
        switch (LOWORD(wParam)) {
        case VK_LEFT:
            CaretMoveLeft(hwnd, edPar);
            SetCaretPos(edPar->caret->xInSymb * edPar->outData->symbWidth, edPar->caret->yInSymb * edPar->outData->symbHeight);
            ShowCaret(hwnd);
            break;

        case VK_RIGHT:
            CaretMoveRight(hwnd, edPar);
            SetCaretPos(edPar->caret->xInSymb * edPar->outData->symbWidth, edPar->caret->yInSymb * edPar->outData->symbHeight);
            printf("%i %i %i %i\n", edPar->caret->xInSymb, edPar->caret->yInSymb, edPar->caret->indOfString, edPar->caret->indOfSym);
            ShowCaret(hwnd);
            break;

        case VK_UP:
            CaretMoveUp(hwnd, edPar);
            SetCaretPos(edPar->caret->xInSymb * edPar->outData->symbWidth, edPar->caret->yInSymb * edPar->outData->symbHeight);
            printf("%i %i %i %i\n", edPar->caret->xInSymb, edPar->caret->yInSymb, edPar->caret->indOfString, edPar->caret->indOfSym);
            ShowCaret(hwnd);
            break;

        case VK_DOWN:
            CaretMoveDown(hwnd, edPar);
            SetCaretPos(edPar->caret->xInSymb * edPar->outData->symbWidth, edPar->caret->yInSymb * edPar->outData->symbHeight);
            printf("%i %i %i %i\n", edPar->caret->xInSymb, edPar->caret->yInSymb, edPar->caret->indOfString, edPar->caret->indOfSym);
            ShowCaret(hwnd);
            break;

        case VK_PRIOR:
            ScrollV(hwnd, SB_PAGEUP, edPar);
            SetCaretPos(edPar->caret->xInSymb * edPar->outData->symbWidth, edPar->caret->yInSymb * edPar->outData->symbHeight);
            ShowCaret(hwnd);
            break;

        case VK_NEXT:
            ScrollV(hwnd, SB_PAGEDOWN, edPar);
            SetCaretPos(edPar->caret->xInSymb * edPar->outData->symbWidth, edPar->caret->yInSymb * edPar->outData->symbHeight);
            ShowCaret(hwnd);
            break;
        case VK_DELETE:
            EditorDelProc(hwnd, edPar);
            break;
            }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_OPEN:
            OpenNewFile(hwnd, edPar);
            break;
        case IDM_CLOSE:
            CloseText(hwnd, edPar);
            break;
        case IDM_EXIT:
            SendMessage(hwnd, WM_CLOSE, 0, 0L);
            break;
        case IDM_CHANDEMO:
            ChangeMode(hwnd, edPar);
            SetCaretPos(edPar->caret->xInSymb * edPar->outData->symbWidth, edPar->caret->yInSymb * edPar->outData->symbHeight);
            ShowCaret(hwnd);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        break;

    default:                      /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }


    return 0;
}
