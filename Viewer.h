#ifndef VIEWER_H_INCLUDED
#define VIEWER_H_INCLUDED

#include "Text.h"
#include "OutputData.h"


/*
    Ввод:
    HDC hdc
    PAINTSTRUCT ps
    const OutputData* outData
    const Text* text
    Вывод:
    Методы перерисовывают текст для случаев без верствки (S), с верствкой (L)
*/
void TextOutS(HDC hdc, PAINTSTRUCT ps, const OutputData* outData, const Text* text);
void TextOutL(HDC hdc, PAINTSTRUCT ps, const OutputData* outData, const Text* text);

/*
    Ввод:
    const Text* text
    OutputData* outData
    Вывод:
    OutputData* outData
    Методы перестраивают структуру при переключении режима
*/
void ProcChangeModeS(OutputData* outData, const Text* text);
void ProcChangeModeL(OutputData* outData, const Text* text);

/*
    Ввод:
    LPARAM lParam
    OutputData* outData
    const Text* text
    Вывод:
    outData
    Методы перестраивают структуру при изменении размеров окна
*/
void ProcChangeSizeH(LPARAM lParam, OutputData* outData);
void ProcChangeSizeL(LPARAM lParam, OutputData* outData, const Text* text);

/*
    Ввод:
    HWND hwnd
    int scrollShift - количество линий, которые нужно проскролить
    OutputData* outData
    const Text* text
    Вывод:
    OutputData* outData
    Методы скролят окно и перестраивают структуру в соответсвии величиной скролла
*/
void ScrollSV(HWND hwnd, int scrollShift, OutputData* outData);
void ScrollSH(HWND hwnd, int scrollShift, OutputData* outData);
void ScrollL(HWND hwnd, int scrollShift, OutputData* outData, const Text* text);

/*
    Ввод:
    HWND hwnd
    OutputData* outData
    const Text* text
    Вывод:
    Методы устанавливают новые значения для параметров скролла
*/
void SetScrollParamS(HWND hwnd, const OutputData* outData);
void SetScrollParamL(HWND hwnd, const OutputData* outData, const Text* text);



#endif // VIEWER_H_INCLUDED
