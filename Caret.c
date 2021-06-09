#include "Caret.h"

void CaretMoveUpS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text) {

    if (caret->yInSymb == 0 && caret->indOfString == 0) {
        return;
    }
    else {
        size_t prevSubStrLen = (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1;
        caret->indOfSym -= getStringLength(text, caret->indOfString - 1);
        caret->indOfString -= 1;

        if (prevSubStrLen > getStringLength(text, caret->indOfString)) {
            caret->indOfSym -= prevSubStrLen - getStringLength(text, caret->indOfString);
            size_t curSubStrLen = (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1;

            int i = 0;
            for (i = 0; i < max((int)prevSubStrLen - (int)curSubStrLen - (int)caret->xInSymb, 0); ++i) {
                int scrollShift = Scroll(hwnd, SB_LINELEFT, SB_HORZ);
                ScrollSH(hwnd, scrollShift, outData);
            }
            caret->xInSymb = max((int)caret->xInSymb - (int)prevSubStrLen + (int)curSubStrLen, 0);
        }
    }
    if (caret->yInSymb == 0) {
        int scrollShift = Scroll(hwnd, SB_LINEUP, SB_VERT);
        ScrollSV(hwnd, scrollShift, outData);
    }
    else {
        caret->yInSymb -= 1;
    }
}

void CaretMoveDownS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text) {
    if (caret->indOfString == text->numOfStrs - 1) {
        return;
    }
    else {
        size_t prevSubStrLen = (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1;
        caret->indOfSym += getStringLength(text, caret->indOfString);
        caret->indOfString += 1;

        if (prevSubStrLen > getStringLength(text, caret->indOfString)) {
            caret->indOfSym -= prevSubStrLen - getStringLength(text, caret->indOfString);
            size_t curSubStrLen = getStringLength(text, caret->indOfString);
            int i;
            for (i = 0; i < max((int)prevSubStrLen - (int)curSubStrLen - (int)caret->xInSymb, 0); ++i) {
                int scrollShift = Scroll(hwnd, SB_LINELEFT, SB_HORZ);
                ScrollSH(hwnd, scrollShift, outData);
            }
            caret->xInSymb = max((int)caret->xInSymb - (int)prevSubStrLen + (int)curSubStrLen, 0);
        }
    }
    if (caret->yInSymb == LineOnPage(outData) - 1) {
        int scrollShift = Scroll(hwnd, SB_LINEDOWN, SB_VERT);
        ScrollSV(hwnd, scrollShift, outData);
    }
    else {
        caret->yInSymb += 1;
    }
}

void CaretMoveLeftS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text) {
    if (caret->xInSymb == 0) {
        if (text->allSyms + caret->indOfSym == text->strings[caret->indOfString]) {
            if (caret->indOfString == 0) {
                return;
            }
            else {
                caret->indOfString -= 1;
                caret->indOfSym -= 1;
                caret->xInSymb = min((int)getStringLength(text, caret->indOfString) - 1, (int)SymInLine(outData) - 1);
                int i;
                for (i = 0; i < max((int)getStringLength(text, caret->indOfString) - (int)SymInLine(outData), 0); ++i) {
                    int scrollShift = Scroll(hwnd, SB_LINERIGHT, SB_HORZ);
                    ScrollSH(hwnd, scrollShift, outData);
                }
                if (caret->yInSymb == 0) {
                    int scrollShift = Scroll(hwnd, SB_LINEUP, SB_VERT);
                    ScrollSV(hwnd, scrollShift, outData);
                }
                else {
                    caret->yInSymb -= 1;
                }
            }
        }
        else {
            int scrollShift = Scroll(hwnd, SB_LINELEFT, SB_HORZ);
            ScrollSH(hwnd, scrollShift, outData);
            caret->indOfSym -= 1;
        }
    }
    else {
        caret->indOfSym -= 1;
        caret->xInSymb -= 1;
    }
}

void CaretMoveRightS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text) {
    if (caret->indOfSym == text->numOfSyms - 1) {
        return;
    }

    if (caret->indOfString != text->numOfStrs - 1  && text->allSyms + caret->indOfSym == text->strings[caret->indOfString + 1] - 1) {

        caret->indOfString += 1;
        caret->indOfSym += 1;
        caret->xInSymb = 0;
        int i;
        for (i = 0; i < max((int)getStringLength(text, caret->indOfString - 1) - (int)SymInLine(outData), 0); ++i) {
            int scrollShift = Scroll(hwnd, SB_LINELEFT, SB_HORZ);
            ScrollSH(hwnd, scrollShift, outData);
        }
        if (caret->yInSymb == LineOnPage(outData) - 1) {
            int scrollShift = Scroll(hwnd, SB_LINEDOWN, SB_VERT);
            ScrollSV(hwnd, scrollShift, outData);
        }
        else {
            caret->yInSymb += 1;
        }
    }
    else  if (caret->xInSymb == SymInLine(outData) - 1) {
        int scrollShift = Scroll(hwnd, SB_LINERIGHT, SB_HORZ);
        ScrollSH(hwnd, scrollShift, outData);
        caret->indOfSym += 1;
    }
    else {
        caret->indOfSym += 1;
        caret->xInSymb += 1;
    }
}


void CaretMoveUpL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text) {
    if ((size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1 > SymInLine(outData)) {
        caret->indOfSym -= SymInLine(outData);
    }
    else if (caret->indOfString == 0) {
        return;
    }
    else {
        size_t prevSubstrLenght = (getStringLength(text, caret->indOfString - 1) - 1) % SymInLine(outData) + 1;
        if (prevSubstrLenght < (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1) {
            caret->indOfSym -= (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1;
            caret->xInSymb = prevSubstrLenght - 1;
        }
        else {
            caret->indOfSym -= prevSubstrLenght;
        }
        caret->indOfString -= 1;
    }
    if (caret->yInSymb == 0) {
        int scrollShift = Scroll(hwnd, SB_LINEUP, SB_VERT);
        ScrollL(hwnd, scrollShift, outData, text);
    } else {
        caret->yInSymb -= 1;
    }
}

void CaretMoveDownL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text) {
    size_t substrLenght = (getStringLength(text, caret->indOfString) - 1) % SymInLine(outData) + 1;
    size_t curStrLenght = (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) % SymInLine(outData) + 1;
    if (getSubstringLength(text, caret->indOfString, text->allSyms + caret->indOfSym) > SymInLine(outData)) {
        caret->indOfSym += SymInLine(outData);
    }
    else if (getSubstringLength(text, caret->indOfString, text->allSyms + caret->indOfSym) <= substrLenght || substrLenght == 0) {
        if (caret->indOfString == text->numOfStrs - 1) {
            return;
        }
        else {
            size_t nextStrLenght = getStringLength(text, caret->indOfString + 1);
            if (nextStrLenght < curStrLenght) {
                caret->indOfSym += nextStrLenght + getStringLength(text, caret->indOfString) - ((size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1);
                caret->xInSymb = nextStrLenght - 1;
            }
            else {
                if (substrLenght == 0) {
                    caret->indOfSym += getStringLength(text, caret->indOfString);
                }
                else {
                    caret->indOfSym += substrLenght;
                }
            }
            caret->indOfString += 1;
        }
    }
    else {
        caret->indOfSym += SymInLine(outData) - curStrLenght + substrLenght;
        caret->xInSymb = substrLenght - 1;
    }

    if (caret->yInSymb == LineOnPage(outData) - 1) {
        int scrollShift = Scroll(hwnd, SB_LINEDOWN, SB_VERT);
        ScrollL(hwnd, scrollShift, outData, text);
    }
    else {
        caret->yInSymb += 1;
    }
}

void CaretMoveLeftL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text) {
    if (caret->xInSymb == 0) {
        if (caret->indOfString == 0 && ((size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1) < SymInLine(outData)) {
            return;
        }
        else {
            if (text->allSyms + caret->indOfSym  == text->strings[caret->indOfString]) {
                caret->indOfSym -= 1;
                caret->indOfString -= 1;
                caret->xInSymb = (getStringLength(text, caret->indOfString) - 1) % SymInLine(outData);
            }
            else {
                caret->indOfSym -= 1;
                caret->xInSymb = SymInLine(outData) - 1;
            }
        }

        if (caret->yInSymb == 0) {
            int scrollShift = Scroll(hwnd, SB_LINEUP, SB_VERT);
            ScrollL(hwnd, scrollShift, outData, text);
        }
        else {
            caret->yInSymb -= 1;
        }
    }
    else {
        caret->indOfSym -= 1;
        caret->xInSymb -= 1;
    }
}

void CaretMoveRightL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text) {
    if (caret->indOfString == text->numOfStrs - 1 && caret->indOfSym == text->numOfSyms - 1) {
        return;
    }
    if (getSubstringLength(text, caret->indOfString, text->allSyms + caret->indOfSym) == 1) {
        caret->indOfSym += 1;
        caret->indOfString += 1;
        caret->xInSymb = 0;

        if (caret->yInSymb == LineOnPage(outData) - 1) {
            int scrollShift = Scroll(hwnd, SB_LINEDOWN, SB_VERT);
            ScrollL(hwnd, scrollShift, outData, text);
        }
        else {
            caret->yInSymb += 1;
        }
    }
    else if (caret->xInSymb == SymInLine(outData) - 1) {
        caret->indOfSym += 1;
        caret->xInSymb = 0;

        if (caret->yInSymb == LineOnPage(outData) - 1) {
            int scrollShift = Scroll(hwnd, SB_LINEDOWN, SB_VERT);
            ScrollL(hwnd, scrollShift, outData, text);
        }
        else {
            caret->yInSymb += 1;
        }
    }
    else {
        caret->indOfSym += 1;
        caret->xInSymb += 1;
    }
}

void CaretProcChangeMode(Caret* caret, OutputData* outData, const Text* text) {
    caret->xInSymb = 0;
    caret->yInSymb = 0;
    caret->indOfString = outData->firstString;
    caret->indOfSym = text->strings[outData->firstString] - text->allSyms;
}

void CaretProcChangeSizeL(HWND hwnd, Caret* caret, OutputData* outData, const Text* text) {
    while (caret->yInSymb >= LineOnPage(outData)) {
        CaretMoveUpL(hwnd, caret, outData, text);
    }
    while (caret->xInSymb >= SymInLine(outData)) {
        CaretMoveLeftL(hwnd, caret, outData, text);
    }
}

void CaretProcChangeSizeS(HWND hwnd, Caret* caret, OutputData* outData, const Text* text) {
    while (caret->yInSymb >= LineOnPage(outData)) {
        CaretMoveUpS(hwnd, caret, outData, text);
    }
    while (caret->xInSymb >= SymInLine(outData)) {
        CaretMoveLeftS(hwnd, caret, outData, text);
    }
}

int CaretProcScrollL(HWND hwnd, int scrollShift, Caret* caret, OutputData* outData, const Text* text) {
    if (scrollShift < 0) {
        int i;
        for (i = 0; i < -scrollShift; ++i) {
            if ((size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1 > SymInLine(outData)) {
                caret->indOfSym -= SymInLine(outData);
            }
            else if (caret->indOfString == 0) {
                return -i;
            }
            else {
                size_t prevSubstrLenght = (getStringLength(text, caret->indOfString - 1) - 1) % SymInLine(outData) + 1;
                if (prevSubstrLenght < (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1) {
                    caret->indOfSym -= (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1;
                    caret->xInSymb = prevSubstrLenght - 1;
                }
                else {
                    caret->indOfSym -= prevSubstrLenght;
                }
                caret->indOfString -= 1;
            }
        }
    } else {
        int i;
        for (i = 0; i < scrollShift; ++i) {
            size_t substrLenght = (getStringLength(text, caret->indOfString) - 1) % SymInLine(outData) + 1;
            size_t curStrLenght = ((size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1) % SymInLine(outData);

            if (getSubstringLength(text, caret->indOfString, text->allSyms + caret->indOfSym) > SymInLine(outData)) {
                caret->indOfSym += SymInLine(outData);
            }
            else if (getSubstringLength(text, caret->indOfString, text->allSyms + caret->indOfSym) <= substrLenght || substrLenght == 0) {
                if (caret->indOfString == text->numOfStrs - 1) {
                    return i;
                }
                else {
                    size_t nextStrLenght = getStringLength(text, caret->indOfString + 1);
                    if (nextStrLenght < curStrLenght) {
                        caret->indOfSym += nextStrLenght + getStringLength(text, caret->indOfString) - ((size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1);
                        caret->xInSymb = nextStrLenght - 1;
                    }
                    else {
                        if (substrLenght == 0) {
                            caret->indOfSym += getStringLength(text, caret->indOfString);
                        }
                        else {
                            caret->indOfSym += substrLenght;
                        }
                    }
                    caret->indOfString += 1;
                }
            }
            else {
                caret->indOfSym += SymInLine(outData) - curStrLenght + substrLenght;
                caret->xInSymb = substrLenght - 1;
            }
        }
    }
    return scrollShift;
}

int CaretProcScrollSH(HWND hwnd, int scrollShift, Caret* caret, OutputData* outData, const Text* text) {
    if (scrollShift > 0) {
        int substringLen = getSubstringLength(text, caret->indOfString, text->allSyms + caret->indOfSym);
        int i;
        for (i = 0; i < scrollShift; ++i) {
            if (substringLen - i == 1) {
                return i;
            } else {
                caret->indOfSym += 1;
            }
        }
    }
    else {
        int substringLen = (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1;
        int i;
        for (i = 0; i < -scrollShift; ++i) {
            if (substringLen - i == 1) {
                return - i;
            } else {
                caret->indOfSym -= 1;
            }
        }
    }
    return scrollShift;
}
int CaretProcScrollSV(HWND hwnd, int scrollShift, Caret* caret, OutputData* outData, const Text* text) {
    if (scrollShift < 0) {
        int i;
        for (i = 0; i < -scrollShift; ++i) {
            if (caret->indOfString == 0) {
                return -i;
            }
            else {
                size_t prevSubStrLen = (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1;
                caret->indOfSym -= getStringLength(text, caret->indOfString - 1);
                caret->indOfString -= 1;

                if (prevSubStrLen > getStringLength(text, caret->indOfString)) {
                    caret->indOfSym -= prevSubStrLen - getStringLength(text, caret->indOfString);
                    size_t curSubStrLen = getStringLength(text, caret->indOfString);

                    SCROLLINFO si;
                    si.cbSize = sizeof(si);
                    si.fMask = SIF_ALL;
                    GetScrollInfo(hwnd, SB_HORZ, &si);
                    SetScrollPos(hwnd, SB_HORZ, si.nPos + max((int)prevSubStrLen - (int)curSubStrLen - (int)caret->xInSymb, 0), TRUE);

                    ScrollSH(hwnd, -max((int)prevSubStrLen - (int)curSubStrLen - (int)caret->xInSymb, 0), outData);
                    caret->xInSymb = max((int)caret->xInSymb - (int)prevSubStrLen + (int)curSubStrLen, 0);
                }
            }
        }
    }
    else {
        int i;
        for (i = 0; i < scrollShift; ++i) {
            if (caret->indOfString == text->numOfStrs - 1) {
                return i;
            }
            else {
                size_t prevSubStrLen = (size_t)(text->allSyms + caret->indOfSym - text->strings[caret->indOfString]) + 1;
                caret->indOfSym += getStringLength(text, caret->indOfString);
                caret->indOfString += 1;

                if (prevSubStrLen > getStringLength(text, caret->indOfString)) {
                    caret->indOfSym -= prevSubStrLen - getStringLength(text, caret->indOfString);
                    size_t curSubStrLen = getStringLength(text, caret->indOfString);

                    SCROLLINFO si;
                    si.cbSize = sizeof(si);
                    si.fMask = SIF_ALL;
                    GetScrollInfo(hwnd, SB_HORZ, &si);
                    SetScrollPos(hwnd, SB_HORZ, si.nPos - max((int)prevSubStrLen - (int)curSubStrLen - (int)caret->xInSymb, 0), TRUE);

                    ScrollSH(hwnd, -max((int)prevSubStrLen - (int)curSubStrLen - (int)caret->xInSymb, 0), outData);

                    caret->xInSymb = max((int)caret->xInSymb - (int)prevSubStrLen + (int)curSubStrLen, 0);
                }
            }
        }
    }
    return scrollShift;
}
