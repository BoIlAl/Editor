#include "OutputData.h"

size_t SymInLine(const OutputData* outData) {
    return outData->workingAreaWidth / outData->symbWidth;
}

size_t LineOnPage(const OutputData* outData) {
    return outData->workingAreaHeight / outData->symbHeight;
}
