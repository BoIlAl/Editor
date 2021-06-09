#ifndef OUTPUTDATA_H_INCLUDED
#define OUTPUTDATA_H_INCLUDED

#include <stdio.h>

typedef struct OutputData_t
{
    size_t workingAreaHeight;   // ������ ������� �������
    size_t workingAreaWidth;    // ������ ������� �������
    size_t symbHeight;          // ������ �������
    size_t symbWidth;           // ������ �������

    size_t firstString;         // ������ ������ � ������, � ������� ������� ������ ����������� ��������� ������ � ������� �������
    size_t firstSymb;           // ������ ������� � ������, � �������� ������� ������ ����������� ��������� ������ � ������� �������
    char* firstSubstr;          // ��������� �� ���������, � ������� ������� ������ ����������� ��������� ������ � ������� �������
    size_t numOfLines;          // ����� ���������� �����, ������� �������� ����� ��� ������� ������� ������� �������
    size_t numOfColums;         // ����� ���������� ��������, ������� �������� ����� ��� ������� ������� ������� �������
} OutputData;


/*
    ����:
    const OutputData* outData - ���������, ���������� ������ � ���������� ������ �����������
    �����:
    size_t - ���������� ��������, ������� ��������� � ����� ������ ��� ������� ������� ����
*/
size_t SymInLine(const OutputData* outData);

/*
    ����:
    const OutputData* outData - ���������, ���������� ������ � ���������� ������ �����������
    �����:
    size_t - ���������� �����, ������� ��������� �� ����� �������� ��� ������� ������� ����
*/
size_t LineOnPage(const OutputData* outData);

#endif // OUTPUTDATA_H_INCLUDED
