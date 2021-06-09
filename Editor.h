#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include "Caret.h"

/*
    Ввод:
    const Caret* caret
    Text* text
    Вывод:
    text
    Методы обрабатывают нажатия на соответствующие клавиши, меняя данные в структуре
*/
//void BackspaceProc(Text* text, const Caret* caret);
void DelProc(Text* text, const Caret* caret);
void CharsProc(Text* text, const Caret* caret, WPARAM wparam);

#endif // EDITOR_H_INCLUDED
