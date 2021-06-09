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
    OutputData* outData;       // структура, содержаща€ данные о параметрах вывода изображени€
    Text* text;                // структура, содержаща€ текст
    int OutMode;               // режим вывода
    bool clearAreaFlag;        // флаг, указывающий на необходимость очистки окна в обработчике WM_PAINT
    Caret* caret;
} EditorParameters;


    /*
        метод выдел€ет пам€ть дл€ структуры EditorParameters
    */
    EditorParameters* EditorParamInit();

    /*
        ввод:
        HWND hwnd
        char* filePat - путь к файлу, содержащему текст
        вывод:
        EditorParameters* edPar
        метод заполн€ет пол€ структуры параметрами по умолчанию и читает текст, если filePath не NULL
    */
    void FillEditorParam(HWND hwnd, EditorParameters* edPar, char* filePath);

    /*
        ввод:
        HWND hwnd
        LPARAM lParam
        вывод:
        EditorParameters* edPar
        метод измен€ет состо€ние edPar и caret в соответствии с изменени€ми размера окна
    */
    void SizeChanged(HWND hwnd, LPARAM lParam, EditorParameters* edPar);

    /*
        ввод:
        HWND hwnd
        LPARAM lParam
        EditorParameters* edPar
        вывод:
        EditorParameters* edPar
        метод перерисовывает части рабочей области, помеченные как invalidRect
    */
    void RedrawText(HWND hwnd, HDC hdc, PAINTSTRUCT ps, EditorParameters* edPar);


    /*
        ввод:
        HWND hwnd
        WPARAM wParam
        EditorParameters* edPar
        вывод:
        EditorParameters* edPar
        Caret* caret
        методы измен€ют состо€ние edPar и caret в ответ на прокрутку и устанавливают новое положение ползунка
    */
    void ScrollV(HWND hwnd, WPARAM wParam, EditorParameters* edPar);
    void ScrollH(HWND hwnd, WPARAM wParam, EditorParameters* edPar);

    /*
        ввод:
        HWND hwnd
        EditorParameters* edPar
        вывод:
        EditorParameters* edPar
        метод измен€ет состо€ние edPar в ответ на изменение режима вывода
    */
    void ChangeMode(HWND hwnd, EditorParameters* edPar);

    /*
        ввод:
        HWND hwnd
        вывод:
        EditorParameters* edPar
        функци€ очищает текущее содержимое структуры пол€, хран€щей текст
        и устанавливает edPar-> clearAreaFlag = TRUE;
    */
    void CloseText(HWND hwnd, EditorParameters* edPar);

    /*
        ввод:
        HWHWND hwnd
        вывод:
        EditorParameters* edPar
        функци€, использу€ стандартный диалог, получает путь к новому файлу и открывает его
    */
    void OpenNewFile(HWND hwnd, EditorParameters* edPar);

    /*
        ввод:
        HWND hwnd
        EditorParameters* edPar
        Caret* caret
        вывод:
        EditorParameters* edPar
        Caret* caret
        ћетоды обрабатывают движени€ каретки, мен€€ данные в edPar и caret
    */
    void CaretMoveUp(HWND hwnd, EditorParameters* edPar);
    void CaretMoveDown(HWND hwnd, EditorParameters* edPar);
    void CaretMoveLeft(HWND hwnd, EditorParameters* edPar);
    void CaretMoveRight(HWND hwnd, EditorParameters* edPar);

    /*
        ввод:
        HWND hwnd
        EditorParameters* edPar
        Caret* caret
        вывод:
        EditorParameters* edPar
        Caret* caret
        ћетоды обрабатывают нажати€ на соответствующие клавиши, мен€€ данные в edPar и caret
    */
    //void EditorBackspaceProc(HWND hwnd, EditorParameters* edPar, Caret* caret);
    void EditorDelProc(HWND hwnd, EditorParameters* edPar);
    void EditorCharsProc(HWND hwnd, WPARAM wparam, EditorParameters* edPar);

#endif
