#include "Viewer.h"

char* stepToEnd(const Text* text, const OutputData* outData, int lineOffset, int* iStr) {
    *iStr = outData->firstString;
    char* substr = outData->firstSubstr;
    int i;
    for (i = 0; i < lineOffset; i++) {
        if (getSubstringLength(text, *iStr, substr) > SymInLine(outData)) {
            substr += SymInLine(outData);
        }
        else {
            *iStr += 1;
            if ((*iStr) >= text->numOfStrs) {
                return NULL;
            }
            substr = text->strings[*iStr];
        }
    }
    return substr;
}

char* stepToBegin(const Text* text, const OutputData* outData, int lineOffset, int* iStr) {
    *iStr = outData->firstString;
    char* substr = outData->firstSubstr;
    int i;
    for (i = 0; i < -lineOffset; i++) {
        if (getStringLength(text, *iStr) - getSubstringLength(text, *iStr, substr) >= SymInLine(outData)) {
            substr -= SymInLine(outData);
        }
        else {
            *iStr -= 1;
            if (*iStr < 0) {
                return NULL;
            }
            int shift = getStringLength(text, *iStr) % SymInLine(outData) == 0 ? 1 : 0;
            substr = text->strings[*iStr] + (getStringLength(text, *iStr) / SymInLine(outData) - shift) * SymInLine(outData);
        }
    }

    return substr;
}

/*
    ¬вод:
    const Text* text
    const OutputData* outData
    int lineOffset -  оличество линий, которые следует отступить
    ¬ывод:
    int* iStr - Ќовый индекс текущей строки
    char* - подстрока, с которой начинаетс€ отрисовка
    ‘ункци€ вычисл€ет индекс новой строки и
    новой подстроки с которой начнетс€ отображение
*/
char* stepBackN(const Text* text, const OutputData* outData, int lineOffset, int* iStr) {
    char* newSubstr = outData->firstSubstr;
    *iStr = outData->firstString;
    if (lineOffset > 0) {
        newSubstr = stepToEnd(text, outData, lineOffset, iStr);
    }
    else if (lineOffset < 0) {
        newSubstr = stepToBegin(text, outData, lineOffset, iStr);
    }

    return newSubstr;
}

void TextOutS(HDC hdc, PAINTSTRUCT ps, const OutputData * outData, const Text* text) {
    size_t iFirstStr = outData->firstString + ps.rcPaint.top / outData->symbHeight;
    size_t iLastStr = min(text->numOfStrs - 1, outData->firstString + ps.rcPaint.bottom / outData->symbHeight);
    size_t iFirstSym = outData->firstSymb + ps.rcPaint.left / outData->symbWidth;
    size_t iLastSym;

    int x = ps.rcPaint.left - (ps.rcPaint.left % outData->symbWidth);
    int y = ps.rcPaint.top - (ps.rcPaint.top % outData->symbHeight);
    int iStr;
    for (iStr = iFirstStr; iStr <= iLastStr; iStr++) {
        iLastSym = min((int)getStringLength(text, iStr), (int)iFirstSym + ps.rcPaint.right / (int)outData->symbWidth);
        int lenght = iLastSym - iFirstSym;

        if (lenght > 0) {
            char* curSubstr = &(text->strings[iStr][iFirstSym]);
            TextOutA(hdc, x, y, curSubstr, lenght);
        }
        y += outData->symbHeight;
    }
}

void TextOutL(HDC hdc, PAINTSTRUCT ps, const OutputData* outData, const Text* text) {
    size_t y = (ps.rcPaint.top / outData->workingAreaHeight) * outData->workingAreaHeight;
    size_t yEnd = ps.rcPaint.bottom;
    size_t strOfset = y / outData->symbHeight;
    int iCurStr;
    char* curSubstr = stepBackN(text, outData, strOfset, &iCurStr);
    size_t lineSize = SymInLine(outData);

    while (y < yEnd) {
        size_t len = getSubstringLength(text, iCurStr, curSubstr);
        size_t nWholeLine = len / lineSize;
        int i;
        for (i = 0; i < nWholeLine && y < yEnd; i++) {
            TextOut(hdc, 0, y, curSubstr, lineSize);
            y += outData->symbHeight;
            curSubstr += lineSize;
        }

        if (len % lineSize != 0 && y < yEnd) {
            TextOut(hdc, 0, y, curSubstr, getSubstringLength(text, iCurStr, curSubstr));
            y += outData->symbHeight;
        }
        iCurStr++;
        if (iCurStr >= text->numOfStrs) {
            break;
        }
        curSubstr = text->strings[iCurStr];
    }
}

void ProcChangeModeS(OutputData* outData, const Text* text) {
    outData->numOfLines = text->numOfStrs;
    outData->numOfColums = text->maxStrLength;
    outData->firstSymb = 0;
    outData->firstString = max(0, min((int)outData->firstString, (int)outData->numOfLines - (int)LineOnPage(outData)));
}

void ProcChangeModeL(OutputData* outData, const Text* text) {
    outData->numOfColums = SymInLine(outData);
    outData->numOfLines = GetNumOfLines(text, outData->numOfColums);
    outData->firstSubstr = text->strings[outData->firstString];
}


void ProcChangeSizeH(LPARAM lParam, OutputData* outData) {
    outData->workingAreaHeight = max(HIWORD(lParam), outData->symbHeight);
    outData->workingAreaWidth = max(LOWORD(lParam), outData->symbWidth);

    outData->firstString = max(0, min(outData->firstString, outData->numOfLines - LineOnPage(outData)));
    outData->firstSymb = max(0, min(outData->firstSymb, outData->numOfColums - SymInLine(outData)));
}

void ProcChangeSizeL(LPARAM lParam, OutputData* outData, const Text* text) {
    int shiftLines = 0;
    int prevWorkingAreaWidth = outData->workingAreaWidth;
    outData->workingAreaWidth = max(LOWORD(lParam), outData->symbWidth);
    outData->workingAreaHeight = max(HIWORD(lParam), outData->symbHeight);
    outData->numOfColums = SymInLine(outData);

    if (prevWorkingAreaWidth != LOWORD(lParam)) {
        int prevNSymInLine = (prevWorkingAreaWidth / outData->symbWidth);
        shiftLines = (outData->firstSubstr - text->strings[outData->firstString]) / prevNSymInLine;
        outData->numOfLines = GetNumOfLines(text, outData->numOfColums);
        outData->firstSubstr = text->strings[outData->firstString];

        if (outData->firstString + 1 < text->numOfStrs &&
            text->strings[outData->firstString] <= outData->firstSubstr + shiftLines * SymInLine(outData) &&
            outData->firstSubstr + shiftLines * SymInLine(outData) < text->strings[outData->firstString + 1]) {
            outData->firstSubstr += shiftLines * SymInLine(outData);
        }
        else if (outData->firstString + 1 < text->numOfStrs) {
            outData->firstString++;
            outData->firstSubstr = text->strings[outData->firstString];
        }
    }

    int iStr;
    if (outData->numOfLines <= LineOnPage(outData)) {
        outData->firstSubstr = text->strings[0];
        outData->firstString = 0;
    }
    else if (stepBackN(text, outData, LineOnPage(outData), &iStr) == NULL) {
        char* lastStr = text->strings[text->numOfStrs - 1];
        outData->firstSubstr = lastStr + (getStringLength(text, text->numOfStrs - 1) / outData->numOfColums) * outData->numOfColums;
        outData->firstString = text->numOfStrs - 1;
        outData->firstSubstr = stepBackN(text, outData, -(int)LineOnPage(outData) + 1, &iStr);
        outData->firstString = iStr;
    }
}


void ScrollSV(HWND hwnd, int scrollShift, OutputData* outData) {
    if (outData->numOfLines <= LineOnPage(outData)) {
        return;
    }
    if (scrollShift != 0) {
        ScrollWindow(hwnd, 0, -scrollShift * outData->symbHeight, NULL, NULL);
        outData->firstString = max(0, min(outData->firstString + scrollShift, outData->numOfLines - LineOnPage(outData)));
        UpdateWindow(hwnd);
    }
}

void ScrollSH(HWND hwnd, int scrollShift, OutputData* outData) {
    if (outData->numOfColums <= SymInLine(outData)) {
        return;
    }
    if (scrollShift != 0) {
        ScrollWindow(hwnd, -scrollShift * outData->symbWidth, 0, NULL, NULL);
        outData->firstSymb = max(0, min(outData->firstSymb + scrollShift, outData->numOfColums - SymInLine(outData)));
        UpdateWindow(hwnd);
    }
}

void ScrollL(HWND hwnd, int scrollShift, OutputData* outData, const Text* text) {
    if (outData->numOfLines <= LineOnPage(outData)) {
        return;
    }
    if (scrollShift != 0) {
        int iStr;
        char* curStr = stepBackN(text, outData, scrollShift, &iStr);
        if (curStr != NULL) {
            ScrollWindow(hwnd, 0, -scrollShift * outData->symbHeight, NULL, NULL);
            outData->firstSubstr = curStr;
            outData->firstString = iStr;
            UpdateWindow(hwnd);
        }
    }
}

void SetScrollParamS(HWND hwnd, const OutputData* outData)
{
    SCROLLINFO si;
    si.cbSize = sizeof(si);

    si.fMask = SIF_ALL;
    si.nMin = 0;
    si.nPage = LineOnPage(outData);
    si.nMax = max((outData->numOfLines - 1), si.nPage);
    si.nPos = max(si.nMin, min(si.nMax, outData->firstString));
    SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

    if (outData->numOfLines <= LineOnPage(outData)) {
        EnableScrollBar(hwnd, SB_VERT, ESB_DISABLE_BOTH);
    }
    else {
        EnableScrollBar(hwnd, SB_VERT, ESB_ENABLE_BOTH);
    }

    si.fMask = SIF_ALL;
    si.nMin = 0;
    si.nPage = SymInLine(outData);
    si.nMax = max((outData->numOfColums - 1), si.nPage);
    si.nPos = max(si.nMin, min(si.nMax, outData->firstSymb));
    SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

    if (outData->numOfColums <= SymInLine(outData)) {
        EnableScrollBar(hwnd, SB_HORZ, ESB_DISABLE_BOTH);
    }
    else {
        EnableScrollBar(hwnd, SB_HORZ, ESB_ENABLE_BOTH);
    }
}

void SetScrollParamL(HWND hwnd, const OutputData* outData, const Text* text) {
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    si.nMin = 0;
    si.nPage = LineOnPage(outData);
    si.nMax = max((outData->numOfLines - 1), si.nPage);
    si.nPos = (int)GetNumOfLinesBefore(text, outData->numOfColums, outData->firstSubstr);

    SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

    EnableScrollBar(hwnd, SB_HORZ, ESB_DISABLE_BOTH);
    if (outData->numOfLines <= LineOnPage(outData)) {
        EnableScrollBar(hwnd, SB_VERT, ESB_DISABLE_BOTH);
    }
    else {
        EnableScrollBar(hwnd, SB_VERT, ESB_ENABLE_BOTH);
    }
}
