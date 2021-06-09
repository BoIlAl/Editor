#ifndef CARET_H_INCLUDED
#define CARET_H_INCLUDED

#include"Viewer.h"
#include "Scroll.h"

typedef struct Caret_t {
    size_t xInSymb;        //Текущее положение каретки на экране по x в символах
    size_t yInSymb;        //Текущее положение каретки на экране по y в символах
    size_t indOfString;    //Индекс строки, в которой находится каретка
    size_t indOfSym;       //Индекс элемента, на которой находится каретка
} Caret;
/*
    Ввод:
    HWND hwnd,
    Caret* caret,
    OutputData* outData,
    const Text* text
    Вывод:
    outData, caret
    Методы обрабатывают сдвиги каретки в двух режимах, меняя ее положения и данные структур
*/
void CaretMoveUpS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);
void CaretMoveDownS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);
void CaretMoveLeftS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);
void CaretMoveRightS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);

void CaretMoveUpL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);
void CaretMoveDownL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);
void CaretMoveLeftL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);
void CaretMoveRightL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);

/*
    Ввод:
    Caret* caret,
    OutputData* outData,
    const Text* text
    Вывод:
    outData, caret
    Метод обрабатывает реакцию каретки на изменение режима просмотра
*/
void CaretProcChangeMode(Caret* caret, OutputData* outData, const Text* text);

/*
    Ввод:
    HWND hwnd
    Caret* caret,
    OutputData* outData,
    const Text* text
    Вывод:
    outData, caret
    Методы обрабатывают реакцию каретки на изменение размеров окна
*/
void CaretProcChangeSizeL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);
void CaretProcChangeSizeS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);

/*
    Ввод:
    HWND hwnd
    int scrollShift - количество линий, которое нужно проскроллить
    Caret* caret,
    OutputData* outData,
    const Text* text
    Вывод:
    outData, caret
    int - количество линий, которое можно проскроллить с учетом положения каретки
    Методы обрабатывают реакцию каретки на скролл окна.
*/
int CaretProcScrollL(HWND hwnd, int scrollShift, Caret* caret, OutputData* outData, const Text* text);
int CaretProcScrollSH(HWND hwnd, int scrollShift, Caret* caret, OutputData* outData, const Text* text);
int CaretProcScrollSV(HWND hwnd, int scrollShift, Caret* caret, OutputData* outData, const Text* text);

#endif // CARET_H_INCLUDED
