#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include "Caret.h"

/*
    ����:
    const Caret* caret
    Text* text
    �����:
    text
    ������ ������������ ������� �� ��������������� �������, ����� ������ � ���������
*/
//void BackspaceProc(Text* text, const Caret* caret);
void DelProc(Text* text, const Caret* caret);
void CharsProc(Text* text, const Caret* caret, WPARAM wparam);

#endif // EDITOR_H_INCLUDED
