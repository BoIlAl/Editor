#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ADDING_BUFF_SIZE 128

typedef struct Text_t {
    char** strings;         // Весь текст
    char* allSyms;          // Указатели на начало строк в тексте
    size_t numOfStrs;       // Количество строк в тексте
    size_t numOfSyms;       // Количество символов в тексте
    size_t maxStrLength;    // Длина самой длинной строки
    size_t buffSize;        // Текущая длина буфера с текстом
} Text;


/*
    Ввод:
    FILE* file - дескриптор потока откуда берется текст
    Вывод:
    Text* text - Функция заполняет сохраняет текст и его параметры в структуру
*/
void ReadText(FILE* file, Text* text);

/*
    Ввод:
    const Text* text - структура, хранящая текст
    size_t index - индекс строки
    Вывод:
    size_t - длина строки по индексу
*/
size_t getStringLength(const Text* text, size_t index);


/*
    Ввод:
    const Text* text - структура, хранящая текст
    size_t index - индекс строки
    char* substr_ - указатель на подстроку
    Вывод:
    size_t - количество символов подстроки до конца строки
*/
size_t getSubstringLength(const Text* text, size_t index, char* substr_);

/*
    Ввод:
    const Text* text - структура, хранящая текст
    size_t nSymInLine - количество символов в линии при текущем размере рабочей области
    Вывод:
    size_t - количество строк, которые умещаются в рабочей области при текущем размере окна
*/
size_t GetNumOfLines(const Text* text, size_t SymInLine);

/*
    Ввод:
    const Text* text - структура, хранящая текст
    size_t nSymInLine - количество символов в линии при текущем размере рабочей области
    char* substr_ - указатель на подстроку, на которой останавливается подсчет строк
    Вывод:
    size_t - количество строк, занимаемых частью текста, лежащей перед подстракой
*/

size_t GetNumOfLinesBefore(const Text* text, size_t SymInLine, char* substr_);

/*
    Ввод:
    Text* text - структура, хранящая текст
    Вывод:
    Text* text
    Метод расширяет буффер, выделенный на текст
*/

void ExpandBuffer(Text* text);

void freeText(Text* text);


#endif
