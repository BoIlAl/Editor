#ifndef VIEWER_H_INCLUDED
#define VIEWER_H_INCLUDED

#include "Text.h"
#include "OutputData.h"


/*
    ����:
    HDC hdc
    PAINTSTRUCT ps
    const OutputData* outData
    const Text* text
    �����:
    ������ �������������� ����� ��� ������� ��� �������� (S), � ��������� (L)
*/
void TextOutS(HDC hdc, PAINTSTRUCT ps, const OutputData* outData, const Text* text);
void TextOutL(HDC hdc, PAINTSTRUCT ps, const OutputData* outData, const Text* text);

/*
    ����:
    const Text* text
    OutputData* outData
    �����:
    OutputData* outData
    ������ ������������� ��������� ��� ������������ ������
*/
void ProcChangeModeS(OutputData* outData, const Text* text);
void ProcChangeModeL(OutputData* outData, const Text* text);

/*
    ����:
    LPARAM lParam
    OutputData* outData
    const Text* text
    �����:
    outData
    ������ ������������� ��������� ��� ��������� �������� ����
*/
void ProcChangeSizeH(LPARAM lParam, OutputData* outData);
void ProcChangeSizeL(LPARAM lParam, OutputData* outData, const Text* text);

/*
    ����:
    HWND hwnd
    int scrollShift - ���������� �����, ������� ����� �����������
    OutputData* outData
    const Text* text
    �����:
    OutputData* outData
    ������ ������� ���� � ������������� ��������� � ����������� ��������� �������
*/
void ScrollSV(HWND hwnd, int scrollShift, OutputData* outData);
void ScrollSH(HWND hwnd, int scrollShift, OutputData* outData);
void ScrollL(HWND hwnd, int scrollShift, OutputData* outData, const Text* text);

/*
    ����:
    HWND hwnd
    OutputData* outData
    const Text* text
    �����:
    ������ ������������� ����� �������� ��� ���������� �������
*/
void SetScrollParamS(HWND hwnd, const OutputData* outData);
void SetScrollParamL(HWND hwnd, const OutputData* outData, const Text* text);



#endif // VIEWER_H_INCLUDED
