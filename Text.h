#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ADDING_BUFF_SIZE 128

typedef struct Text_t {
    char** strings;         // ���� �����
    char* allSyms;          // ��������� �� ������ ����� � ������
    size_t numOfStrs;       // ���������� ����� � ������
    size_t numOfSyms;       // ���������� �������� � ������
    size_t maxStrLength;    // ����� ����� ������� ������
    size_t buffSize;        // ������� ����� ������ � �������
} Text;


/*
    ����:
    FILE* file - ���������� ������ ������ ������� �����
    �����:
    Text* text - ������� ��������� ��������� ����� � ��� ��������� � ���������
*/
void ReadText(FILE* file, Text* text);

/*
    ����:
    const Text* text - ���������, �������� �����
    size_t index - ������ ������
    �����:
    size_t - ����� ������ �� �������
*/
size_t getStringLength(const Text* text, size_t index);


/*
    ����:
    const Text* text - ���������, �������� �����
    size_t index - ������ ������
    char* substr_ - ��������� �� ���������
    �����:
    size_t - ���������� �������� ��������� �� ����� ������
*/
size_t getSubstringLength(const Text* text, size_t index, char* substr_);

/*
    ����:
    const Text* text - ���������, �������� �����
    size_t nSymInLine - ���������� �������� � ����� ��� ������� ������� ������� �������
    �����:
    size_t - ���������� �����, ������� ��������� � ������� ������� ��� ������� ������� ����
*/
size_t GetNumOfLines(const Text* text, size_t SymInLine);

/*
    ����:
    const Text* text - ���������, �������� �����
    size_t nSymInLine - ���������� �������� � ����� ��� ������� ������� ������� �������
    char* substr_ - ��������� �� ���������, �� ������� ��������������� ������� �����
    �����:
    size_t - ���������� �����, ���������� ������ ������, ������� ����� ����������
*/

size_t GetNumOfLinesBefore(const Text* text, size_t SymInLine, char* substr_);

/*
    ����:
    Text* text - ���������, �������� �����
    �����:
    Text* text
    ����� ��������� ������, ���������� �� �����
*/

void ExpandBuffer(Text* text);

void freeText(Text* text);


#endif
