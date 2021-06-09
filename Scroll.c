#include "Scroll.h"

int Scroll(HWND hwnd, WPARAM wParam, int ScrollFlag) {

    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hwnd, ScrollFlag, &si);
    int prevScrollPos = si.nPos;

    switch (LOWORD(wParam)) {
    case SB_LINEUP:
        si.nPos -= 1;
        break;
    case SB_LINEDOWN:
        si.nPos += 1;
        break;
    case SB_PAGEUP:
        si.nPos -= si.nPage;
        break;
    case SB_PAGEDOWN:
        si.nPos += si.nPage;
        break;
    case SB_THUMBTRACK:
        si.nPos = si.nTrackPos;
        break;
    default:
        break;
    }

    si.nPos = max(si.nMin, min(si.nMax - si.nPage + 1, si.nPos));

    if (si.nPos != prevScrollPos) {
        SetScrollPos(hwnd, ScrollFlag, si.nPos, TRUE);
    }

    return  si.nPos - prevScrollPos;
}
