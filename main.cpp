#include <iostream>

//#include "HashTable_DA.h"
#include "Tests.h"

using namespace std;

// Хэш-таблицы, часть I
// 1. Реализовать хеш-таблицу, использующую метод цепочек
// 2. Или реализовать хеш-таблицу с открытой адресацией
// - дополнительно: реализовать "ленивое" удаление
// - реализовать квадратичный пробинг
// Критерии оценки: 3 балла максимум за основное задание, 2 за дополнительное.

int main()
{
    if ( SimpleTest() )
        cout << "SimpleTest done success" << endl;
    else
        cout << "SimpleTest failed!" << endl;

    if ( TestWithMyClass() )
        cout << "TestWithMyClass done success" << endl;
    else
        cout << "TestWithMyClass failed!" << endl;

    return 0;
}
