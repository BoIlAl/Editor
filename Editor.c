#include "Editor.h"

void DelProc(Text* text, const Caret* caret) {
    if (getSubstringLength(text, caret->indOfString, text->allSyms + caret->indOfSym) == 1) {
        return;
    }
    size_t i;
    if (getSubstringLength(text, caret->indOfString, text->allSyms + caret->indOfSym) == 2 && caret->indOfString != text->numOfStrs - 1) {
        text->numOfStrs -= 1;
        for (i = caret->indOfString + 1; i < text->numOfStrs; ++i) {
            text->strings[i] = text->strings[i + 1];
        }

    }
    for (i = caret->indOfSym + 1; i < text->numOfSyms - 1; ++i) {
        text->allSyms[i] = text->allSyms[i + 1];
    }
    for (i = caret->indOfString + 1; i < text->numOfStrs; ++i) {
        text->strings[i] -= 1;
    }
    text->numOfSyms -= 1;
    size_t maxStr = 0;
    for (i = 0; i < text->numOfStrs; ++i) {
        if (getStringLength(text, i) > maxStr) {
            maxStr = getStringLength(text, i);
        }
    }
    text->maxStrLength = maxStr;

}

void CharsProc(Text* text,  const Caret* caret, WPARAM wparam) {
    if (text->numOfSyms == text->buffSize) {
        ExpandBuffer(text);
    }
    size_t i;
    for (i =  text->numOfSyms; i > caret->indOfSym; --i) {
        text->allSyms[i] = text->allSyms[i - 1];
    }
    for (i = caret->indOfString + 1; i < text->numOfStrs; ++i) {
        text->strings[i] += 1;
    }

    text->allSyms[caret->indOfSym] = (char)(wparam);
    text->numOfSyms += 1;

    if (getStringLength(text, caret->indOfString) > text->maxStrLength) {
        text->maxStrLength = getStringLength(text, caret->indOfString);
    }
}
