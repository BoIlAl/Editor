#ifndef SCROLL_H_INCLUDED
#define SCROLL_H_INCLUDED


#include <windows.h>
#include "OutputData.h"

#define MAXSCROLLPOS LOWORD(0xffffffff)

/*
    input
    HWND hwnd
    WPARAM wParam
    int nBar - SB_VERT или SB_HORZ
    output
    int -  оличество проскроленных линий, метод также устанавливает новое значение скролла
*/

int Scroll(HWND hwnd, WPARAM wParam, int ScrollFlag);

#endif
