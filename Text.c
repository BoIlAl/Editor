#include"Text.h"

void ReadText(FILE* file, Text* text) {
    fseek(file, 0, SEEK_END);
    text->numOfSyms = ftell(file);
    fseek(file, 0, SEEK_SET);
    text->allSyms = (char*)malloc(sizeof(char) * (text->numOfSyms + ADDING_BUFF_SIZE));
    text->buffSize = text->numOfSyms + ADDING_BUFF_SIZE;
    fread(text->allSyms, sizeof(char), text->numOfSyms, file);

    size_t numOfR = 0;
    int i;
    for (i = 0; i < text->numOfSyms - numOfR; i++) {
        if (text->allSyms[i + numOfR] == '\r') {
            numOfR += 1;
        }
        text->allSyms[i] = text->allSyms[i + numOfR];
    }
    text->numOfSyms = text->numOfSyms - numOfR;

    text->numOfStrs = 0;
    size_t strLen = 0;
    text->maxStrLength = 0;
    for (i = 0; i < text->numOfSyms; i++) {
        strLen++;
        if (text->allSyms[i] == '\n') {
            text->numOfStrs++;
            if (text->maxStrLength < strLen) {
                text->maxStrLength = strLen;
            }
            strLen = 0;
        }
    }
    if (text->allSyms[text->numOfSyms - 1] != '\n') {
        text->numOfStrs++;
    }

    text->strings = (char**)malloc(sizeof(char*) * text->numOfStrs);
    text->strings[0] = text->allSyms;
    size_t indOfStr = 0;
    for (i = 0; i < text->numOfSyms; i++) {
        if (text->allSyms[i] == '\n' || i == text->numOfSyms - 1) {
            indOfStr++;
            if (indOfStr < text->numOfStrs) {
                text->strings[indOfStr] = &(text->allSyms[i + 1]);
            }
        }
    }
}

size_t getStringLength(const Text* text, size_t index) {
    if (index == text->numOfStrs - 1) {
        return text->numOfSyms - (size_t)(text->strings[index] - text->strings[0]);
    }
    return text->strings[index + 1] - text->strings[index];
}

size_t getSubstringLength(const Text* text, size_t indexOfStr, char* _substr) {
    return getStringLength(text, indexOfStr) - (size_t)(_substr - text->strings[indexOfStr]);
}

size_t GetNumOfLines(const Text* text, size_t symInLine) {
    size_t numOfLine = 0;

    int i;
    for (i = 0; i < text->numOfStrs; i++) {
        numOfLine += getStringLength(text, i) / symInLine;
        if (getStringLength(text, i) % symInLine != 0) {
            numOfLine += 1;
        }
    }
    return numOfLine;
}

size_t GetNumOfLinesBefore(const Text* text, size_t symInLine, char* substr_) {
    size_t numOfLine = 0;

    int i;
    for (i = 0; i < text->numOfStrs; i++) {
        if (text->strings[i] <= substr_ && substr_ < text->strings[i] + getStringLength(text, i)) {

            numOfLine += (substr_ - text->strings[i]) / symInLine;
            if ((substr_ - text->strings[i]) % symInLine != 0) {
                numOfLine += 1;
            }
            break;
        }

        numOfLine += getStringLength(text, i) / symInLine;
        if (getStringLength(text, i) % symInLine != 0) {
            numOfLine += 1;
        }
    }
    return numOfLine;
}

void ExpandBuffer(Text* text) {
    char* newBuff = malloc((text->buffSize + ADDING_BUFF_SIZE) * sizeof(char));

    memcpy(newBuff, text->allSyms, text->buffSize);
    free(text->allSyms);
    text->allSyms = newBuff;
    text->strings[0] = text->allSyms;
    size_t indOfStr = 0, i;
    for (i = 0; i < text->numOfSyms; i++) {
        if (text->allSyms[i] == '\n' || i == text->numOfSyms - 1) {
            indOfStr++;
            if (indOfStr < text->numOfStrs) {
                text->strings[indOfStr] = &(text->allSyms[i + 1]);
            }
        }
    }
    text->buffSize += ADDING_BUFF_SIZE;
}


void freeText(Text* text) {
    free(text->strings);
    free(text->allSyms);
}
