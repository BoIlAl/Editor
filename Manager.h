#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED



#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Editor.h"
#include "EditorMenu.h"

#define CLOSED 1
#define SMODE 2
#define LOMODE 3

typedef struct EditorParameters_t {
    OutputData* outData;       // ���������, ���������� ������ � ���������� ������ �����������
    Text* text;                // ���������, ���������� �����
    int OutMode;               // ����� ������
    bool clearAreaFlag;        // ����, ����������� �� ������������� ������� ���� � ����������� WM_PAINT
    Caret* caret;
} EditorParameters;


    /*
        ����� �������� ������ ��� ��������� EditorParameters
    */
    EditorParameters* EditorParamInit();

    /*
        ����:
        HWND hwnd
        char* filePat - ���� � �����, ����������� �����
        �����:
        EditorParameters* edPar
        ����� ��������� ���� ��������� ����������� �� ��������� � ������ �����, ���� filePath �� NULL
    */
    void FillEditorParam(HWND hwnd, EditorParameters* edPar, char* filePath);

    /*
        ����:
        HWND hwnd
        LPARAM lParam
        �����:
        EditorParameters* edPar
        ����� �������� ��������� edPar � caret � ������������ � ����������� ������� ����
    */
    void SizeChanged(HWND hwnd, LPARAM lParam, EditorParameters* edPar);

    /*
        ����:
        HWND hwnd
        LPARAM lParam
        EditorParameters* edPar
        �����:
        EditorParameters* edPar
        ����� �������������� ����� ������� �������, ���������� ��� invalidRect
    */
    void RedrawText(HWND hwnd, HDC hdc, PAINTSTRUCT ps, EditorParameters* edPar);


    /*
        ����:
        HWND hwnd
        WPARAM wParam
        EditorParameters* edPar
        �����:
        EditorParameters* edPar
        Caret* caret
        ������ �������� ��������� edPar � caret � ����� �� ��������� � ������������� ����� ��������� ��������
    */
    void ScrollV(HWND hwnd, WPARAM wParam, EditorParameters* edPar);
    void ScrollH(HWND hwnd, WPARAM wParam, EditorParameters* edPar);

    /*
        ����:
        HWND hwnd
        EditorParameters* edPar
        �����:
        EditorParameters* edPar
        ����� �������� ��������� edPar � ����� �� ��������� ������ ������
    */
    void ChangeMode(HWND hwnd, EditorParameters* edPar);

    /*
        ����:
        HWND hwnd
        �����:
        EditorParameters* edPar
        ������� ������� ������� ���������� ��������� ����, �������� �����
        � ������������� edPar-> clearAreaFlag = TRUE;
    */
    void CloseText(HWND hwnd, EditorParameters* edPar);

    /*
        ����:
        HWHWND hwnd
        �����:
        EditorParameters* edPar
        �������, ��������� ����������� ������, �������� ���� � ������ ����� � ��������� ���
    */
    void OpenNewFile(HWND hwnd, EditorParameters* edPar);

    /*
        ����:
        HWND hwnd
        EditorParameters* edPar
        Caret* caret
        �����:
        EditorParameters* edPar
        Caret* caret
        ������ ������������ �������� �������, ����� ������ � edPar � caret
    */
    void CaretMoveUp(HWND hwnd, EditorParameters* edPar);
    void CaretMoveDown(HWND hwnd, EditorParameters* edPar);
    void CaretMoveLeft(HWND hwnd, EditorParameters* edPar);
    void CaretMoveRight(HWND hwnd, EditorParameters* edPar);

    /*
        ����:
        HWND hwnd
        EditorParameters* edPar
        Caret* caret
        �����:
        EditorParameters* edPar
        Caret* caret
        ������ ������������ ������� �� ��������������� �������, ����� ������ � edPar � caret
    */
    //void EditorBackspaceProc(HWND hwnd, EditorParameters* edPar, Caret* caret);
    void EditorDelProc(HWND hwnd, EditorParameters* edPar);
    void EditorCharsProc(HWND hwnd, WPARAM wparam, EditorParameters* edPar);

#endif
