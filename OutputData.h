#ifndef OUTPUTDATA_H_INCLUDED
#define OUTPUTDATA_H_INCLUDED

#include <stdio.h>

typedef struct OutputData_t
{
    size_t workingAreaHeight;   // ¬ысота рабочей области
    size_t workingAreaWidth;    // Ўирина рабочей области
    size_t symbHeight;          // ¬ысота символа
    size_t symbWidth;           // Ўирина символа

    size_t firstString;         // индекс строки в тексте, с которой следует начать отображение фрагмента текста в рабочей области
    size_t firstSymb;           // индекс символа в строке, с которого следует начать отображение фрагмента текста в рабочей области
    char* firstSubstr;          // указатель на подстроку, с которой следует начать отображение фрагмента текста в рабочей области
    size_t numOfLines;          // общее количество строк, которое занимает текст при текущем размере рабочей области
    size_t numOfColums;         // общее количество столбцов, которое занимает текст при текущем размере рабочей области
} OutputData;


/*
    ¬вод:
    const OutputData* outData - структура, содержаща€ данные о параметрах вывода изображени€
    ¬ывод:
    size_t - количество символов, которые умещаютс€ в одной строке при текущем размере окна
*/
size_t SymInLine(const OutputData* outData);

/*
    ¬вод:
    const OutputData* outData - структура, содержаща€ данные о параметрах вывода изображени€
    ¬ывод:
    size_t - количество строк, которые умещаютс€ на одной странице при текущем размере окна
*/
size_t LineOnPage(const OutputData* outData);

#endif // OUTPUTDATA_H_INCLUDED
