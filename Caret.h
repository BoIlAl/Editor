#ifndef CARET_H_INCLUDED
#define CARET_H_INCLUDED

#include"Viewer.h"
#include "Scroll.h"

typedef struct Caret_t {
    size_t xInSymb;        //������� ��������� ������� �� ������ �� x � ��������
    size_t yInSymb;        //������� ��������� ������� �� ������ �� y � ��������
    size_t indOfString;    //������ ������, � ������� ��������� �������
    size_t indOfSym;       //������ ��������, �� ������� ��������� �������
} Caret;
/*
    ����:
    HWND hwnd,
    Caret* caret,
    OutputData* outData,
    const Text* text
    �����:
    outData, caret
    ������ ������������ ������ ������� � ���� �������, ����� �� ��������� � ������ ��������
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
    ����:
    Caret* caret,
    OutputData* outData,
    const Text* text
    �����:
    outData, caret
    ����� ������������ ������� ������� �� ��������� ������ ���������
*/
void CaretProcChangeMode(Caret* caret, OutputData* outData, const Text* text);

/*
    ����:
    HWND hwnd
    Caret* caret,
    OutputData* outData,
    const Text* text
    �����:
    outData, caret
    ������ ������������ ������� ������� �� ��������� �������� ����
*/
void CaretProcChangeSizeL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);
void CaretProcChangeSizeS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text);

/*
    ����:
    HWND hwnd
    int scrollShift - ���������� �����, ������� ����� ������������
    Caret* caret,
    OutputData* outData,
    const Text* text
    �����:
    outData, caret
    int - ���������� �����, ������� ����� ������������ � ������ ��������� �������
    ������ ������������ ������� ������� �� ������ ����.
*/
int CaretProcScrollL(HWND hwnd, int scrollShift, Caret* caret, OutputData* outData, const Text* text);
int CaretProcScrollSH(HWND hwnd, int scrollShift, Caret* caret, OutputData* outData, const Text* text);
int CaretProcScrollSV(HWND hwnd, int scrollShift, Caret* caret, OutputData* outData, const Text* text);

#endif // CARET_H_INCLUDED
