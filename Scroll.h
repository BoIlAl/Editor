#ifndef SCROLL_H_INCLUDED
#define SCROLL_H_INCLUDED


#include <windows.h>
#include "OutputData.h"

#define MAXSCROLLPOS LOWORD(0xffffffff)

/*
    input
    HWND hwnd
    WPARAM wParam
    int nBar - SB_VERT ��� SB_HORZ
    output
    int - ���������� ������������� �����, ����� ����� ������������� ����� �������� �������
*/

int Scroll(HWND hwnd, WPARAM wParam, int ScrollFlag);

#endif
