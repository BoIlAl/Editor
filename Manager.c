#include"Manager.h"
#define BGC 0x00FFFFFF

/*
    input
    HWND hwnd
    int sizeOFNBuf - buffer size
    output
    PSTR OFNBuffer - buffer for the path to the new file
    WINBOOL - TRUE if the user selected a new file
              FALSE if the user pressed "cancel" or chose a path that does not fit in the buffer
*/
bool getNewFileName(HWND hwnd, PSTR OFNBuffer, int sizeOFNBuf) {
    OPENFILENAME ofn;

    ZeroMemory(&ofn, sizeof(ofn));

    OFNBuffer[0] = '\0';
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "All Files\0*.TXT;*.C;*.H;*.CPP\0";
    ofn.lpstrFile = OFNBuffer;
    ofn.nMaxFile = sizeOFNBuf;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = "txt";

    return GetOpenFileName(&ofn);
}

/*
    the function clears the entire client area
*/
void clearClientArea(HWND hwnd, HDC hdc) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH hBrush = CreateSolidBrush(BGC);
    FillRect(hdc, &rect, hBrush);
}

EditorParameters* EditorParamInit() {
    EditorParameters* edPar = (EditorParameters*)malloc(sizeof(EditorParameters));
    edPar->text = (Text*)malloc(sizeof(Text));
    edPar->outData = (OutputData*)malloc(sizeof(OutputData));
    edPar->caret = (Caret*)malloc(sizeof(Caret));
    return edPar;
}

void FillEditorParam(HWND hwnd, EditorParameters* edPar, char* filePath)
{
    Text* text = edPar->text;
    OutputData* outData = edPar->outData;

    edPar->OutMode = CLOSED;
    if (filePath != NULL) {

        FILE* file = fopen(filePath, "rb");
        if (file != NULL) {
            ReadText(file, text);
            fclose(file);
            edPar->OutMode = SMODE;
        }
        else {
            text->maxStrLength = 0;
            text->numOfStrs = 0;
            text->numOfSyms = 0;
            HMENU hmenu = GetMenu(hwnd);
            EnableMenuItem(hmenu, IDM_CLOSE, MF_GRAYED);
        }
    }

    edPar->clearAreaFlag = FALSE;

    RECT rect;
    GetClientRect(hwnd, &rect);
    outData->workingAreaHeight = rect.bottom - rect.top;
    outData->workingAreaWidth = rect.right - rect.left;

    HDC hdc = GetDC(hwnd);
    SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    ReleaseDC(hwnd, hdc);
    outData->symbHeight = tm.tmHeight + tm.tmExternalLeading;
    outData->symbWidth = tm.tmMaxCharWidth;

    outData->firstString = 0;
    outData->firstSymb = 0;

    edPar->caret->indOfString = 0;
    edPar->caret->indOfSym = 0;
    edPar->caret->xInSymb = 0;
    edPar->caret->yInSymb = 0;

    ProcChangeModeS(outData, text);
    SetScrollParamS(hwnd, outData);
}

void CloseText(HWND hwnd, EditorParameters* edPar) {
    if (edPar->OutMode == CLOSED)
        return;
    freeText(edPar->text);
    edPar->clearAreaFlag = TRUE;
    edPar->OutMode = CLOSED;
    edPar->caret->indOfString = 0;
    edPar->caret->indOfSym = 0;
    edPar->caret->xInSymb = 0;
    edPar->caret->yInSymb = 0;

    InvalidateRect(hwnd, NULL, FALSE);
    EnableScrollBar(hwnd, SB_HORZ, ESB_DISABLE_BOTH);
    EnableScrollBar(hwnd, SB_VERT, ESB_DISABLE_BOTH);
    HMENU hmenu = GetMenu(hwnd);
    EnableMenuItem(hmenu, IDM_CLOSE, MF_GRAYED);
}

size_t NumOfLinesBeforeSubstr(const Text* text, const OutputData* outData, int indOfStr) {
    int i;
    int numOfLine = 0;
    if (outData->firstString == indOfStr) {
        return 0;
    }

    if (getSubstringLength(text, outData->firstString, outData->firstSubstr) % SymInLine(outData) == 0) {
        numOfLine += (getSubstringLength(text, outData->firstString, outData->firstSubstr)) / SymInLine(outData);
    } else {
        numOfLine += (getSubstringLength(text, outData->firstString, outData->firstSubstr)) / SymInLine(outData) + 1;
    }

    for (i = outData->firstString + 1; i < indOfStr; i++) {
        if (getStringLength(text, i) % SymInLine(outData) == 0) {
            numOfLine += (getStringLength(text, i)) / SymInLine(outData);
        } else {
            numOfLine += (getStringLength(text, i)) / SymInLine(outData) + 1;
        }
    }
    return numOfLine;
}


void SizeChanged(HWND hwnd, LPARAM lParam, EditorParameters* edPar) {
    if (edPar->OutMode == SMODE) {
        int firsrStr = edPar->outData->firstString;
        ProcChangeSizeH(lParam, edPar->outData);
        SetScrollParamS(hwnd, edPar->outData);
        CaretProcChangeSizeS(hwnd, edPar->caret, edPar->outData, edPar->text);
        if (firsrStr != edPar->outData->firstString) {
            edPar->caret->yInSymb += 1;
        }

    }
    else if (edPar->OutMode == LOMODE) {
        while ((size_t)(edPar->text->allSyms + edPar->caret->indOfSym - edPar->text->strings[edPar->caret->indOfString]) >= SymInLine(edPar->outData)) {
           CaretMoveUpL(hwnd, edPar->caret, edPar->outData, edPar->text);
        }
        int prevNumOfSubstr = NumOfLinesBeforeSubstr(edPar->text, edPar->outData, edPar->caret->indOfString);

        ProcChangeSizeL(lParam, edPar->outData, edPar->text);
        SetScrollParamL(hwnd, edPar->outData, edPar->text);

        int numOfSubstr = NumOfLinesBeforeSubstr(edPar->text, edPar->outData, edPar->caret->indOfString);

        CaretProcChangeSizeL(hwnd, edPar->caret, edPar->outData, edPar->text);



        if (prevNumOfSubstr != numOfSubstr) {
            edPar->caret->yInSymb -= (int)(prevNumOfSubstr - numOfSubstr);
        }

        edPar->clearAreaFlag = TRUE;
    }
    InvalidateRect(hwnd, NULL, FALSE);
}

/*
    input
    HWND hwnd
    HDC hdc
    PAINTSTRUCT ps
    DRAWABLETEXT* dText
    the function redraws invalid sections of the client area
*/
void RedrawText(HWND hwnd, HDC hdc, PAINTSTRUCT ps, EditorParameters* edPar) {
    if (edPar->clearAreaFlag == TRUE) {
        clearClientArea(hwnd, hdc);
    }
    if (edPar->OutMode == SMODE) {
        TextOutS(hdc, ps, edPar->outData, edPar->text);
    }
    else if (edPar->OutMode == LOMODE) {
        TextOutL(hdc, ps, edPar->outData, edPar->text);
    }
    edPar->clearAreaFlag = FALSE;
}

void ScrollV(HWND hwnd, WPARAM wParam, EditorParameters* edPar) {
    if (edPar->OutMode == SMODE) {
        int scrollShift = Scroll(hwnd, wParam, SB_VERT);
        int scrollShiftNew = CaretProcScrollSV(hwnd, scrollShift, edPar->caret, edPar->outData, edPar->text);

        SCROLLINFO si;
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hwnd, SB_VERT, &si);

        SetScrollPos(hwnd, SB_VERT, si.nPos + scrollShiftNew - scrollShift, TRUE);
        ScrollSV(hwnd, scrollShiftNew, edPar->outData);
    }
    else if (edPar->OutMode == LOMODE) {
        int scrollShift = Scroll(hwnd, wParam, SB_VERT);
        int scrollShiftNew = CaretProcScrollL(hwnd, scrollShift, edPar->caret, edPar->outData, edPar->text);

        SCROLLINFO si;
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hwnd, SB_VERT, &si);

        SetScrollPos(hwnd, SB_VERT, si.nPos + scrollShiftNew - scrollShift, TRUE);
        ScrollL(hwnd, scrollShiftNew, edPar->outData, edPar->text);
    }
}


void ScrollH(HWND hwnd, WPARAM wParam, EditorParameters* edPar) {
    if (edPar->OutMode == SMODE) {
        int scrollShift = Scroll(hwnd, wParam, SB_HORZ);
        int scrollShiftNew  = CaretProcScrollSH(hwnd, scrollShift, edPar->caret, edPar->outData, edPar->text);

        SCROLLINFO si;
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hwnd, SB_HORZ, &si);


        SetScrollPos(hwnd, SB_HORZ, si.nPos + scrollShiftNew - scrollShift, TRUE);
        ScrollSH(hwnd, scrollShiftNew, edPar->outData);
    }
}


void ChangeMode(HWND hwnd, EditorParameters* edPar)
{
    if (edPar->OutMode == LOMODE)
    {
        ProcChangeModeS(edPar->outData, edPar->text);
        SetScrollParamS(hwnd, edPar->outData);
        edPar->OutMode = SMODE;
    }
    else if (edPar->OutMode == SMODE)
    {
        ProcChangeModeL(edPar->outData, edPar->text);
        SetScrollParamL(hwnd, edPar->outData, edPar->text);
        edPar->OutMode = LOMODE;
    }
    CaretProcChangeMode(edPar->caret, edPar->outData, edPar->text);
    edPar->clearAreaFlag = TRUE;
    InvalidateRect(hwnd, NULL, TRUE);
}

void OpenNewFile(HWND hwnd, EditorParameters* edPar) {
    char fileNameBuf[1024];
    if (getNewFileName(hwnd, fileNameBuf, sizeof(fileNameBuf)) == FALSE) {
        return;
    }

    if (edPar->OutMode != CLOSED) {
        CloseText(hwnd, edPar);
    }
    FillEditorParam(hwnd, edPar, fileNameBuf);
    InvalidateRect(hwnd, NULL, TRUE);

    HMENU hmenu = GetMenu(hwnd);
    EnableMenuItem(hmenu, IDM_CLOSE, MF_ENABLED);
}


void CaretMoveUp(HWND hwnd, EditorParameters* edPar) {
    if (edPar->OutMode == LOMODE) {
        CaretMoveUpL(hwnd, edPar->caret, edPar->outData, edPar->text);
    }
    else if (edPar->OutMode == SMODE) {
        CaretMoveUpS(hwnd, edPar->caret, edPar->outData, edPar->text);
    }
}

void CaretMoveDown(HWND hwnd, EditorParameters* edPar) {
    if (edPar->OutMode == LOMODE) {
        CaretMoveDownL(hwnd, edPar->caret, edPar->outData, edPar->text);
    }
    else if (edPar->OutMode == SMODE) {
        CaretMoveDownS(hwnd, edPar->caret, edPar->outData, edPar->text);
    }
}

void CaretMoveLeft(HWND hwnd, EditorParameters* edPar) {
    if (edPar->OutMode == LOMODE) {
        CaretMoveLeftL(hwnd, edPar->caret, edPar->outData, edPar->text);
    }
    else if (edPar->OutMode == SMODE) {
        CaretMoveLeftS(hwnd, edPar->caret, edPar->outData, edPar->text);
    }
}
void CaretMoveRight(HWND hwnd, EditorParameters* edPar) {
    if (edPar->OutMode == LOMODE) {
        CaretMoveRightL(hwnd, edPar->caret, edPar->outData, edPar->text);
    }
    else if (edPar->OutMode == SMODE) {
        CaretMoveRightS(hwnd, edPar->caret, edPar->outData, edPar->text);
    }
}

void EditorDelProc(HWND hwnd, EditorParameters* edPar) {
    int shift = edPar->outData->firstSubstr - edPar->text->allSyms;
    DelProc(edPar->text, edPar->caret);

    edPar->outData->firstSubstr = edPar->text->allSyms + shift;

    if (edPar->OutMode == SMODE && edPar->text->maxStrLength != edPar->outData->numOfColums) {
        edPar->outData->numOfColums = edPar->text->maxStrLength;
        SetScrollParamS(hwnd, edPar->outData);
    }
    if (edPar->OutMode == LOMODE && edPar->outData->numOfLines != GetNumOfLines(edPar->text, edPar->outData->numOfColums)) {
        edPar->outData->numOfLines = GetNumOfLines(edPar->text, edPar->outData->numOfColums);
        SetScrollParamL(hwnd, edPar->outData, edPar->text);
    }

    edPar->clearAreaFlag = TRUE;
    InvalidateRect(hwnd, NULL, TRUE);
}

void EditorCharsProc(HWND hwnd, WPARAM wparam, EditorParameters* edPar) {
    int shift = edPar->outData->firstSubstr - edPar->text->allSyms;
    CharsProc(edPar->text, edPar->caret, wparam);

    edPar->outData->firstSubstr = edPar->text->allSyms + shift;

    if (edPar->OutMode == SMODE && edPar->text->maxStrLength != edPar->outData->numOfColums) {
        edPar->outData->numOfColums = edPar->text->maxStrLength;
        SetScrollParamS(hwnd, edPar->outData);
    }
    if (edPar->OutMode == LOMODE && edPar->outData->numOfLines != GetNumOfLines(edPar->text, edPar->outData->numOfColums)) {
        edPar->outData->numOfLines = GetNumOfLines(edPar->text, edPar->outData->numOfColums);
        SetScrollParamL(hwnd, edPar->outData, edPar->text);
    }

    edPar->clearAreaFlag = TRUE;
    InvalidateRect(hwnd, NULL, TRUE);
}
