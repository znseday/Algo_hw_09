#include <iostream>
#include <chrono>
#include <thread>

#include "HashTable_DA.h"

#include "moveable.h"
#include "UnitUniqueRandom.h"

using namespace std;

using ClockType = std::chrono::steady_clock;

bool IsDebugOutput = true;
bool IsDebugOutputM = false;

bool SimpleTest()
{
    cout << "SimpleTest started" << endl;

    HashTable_DA<string, string> Table(0.5, 1);

    chrono::time_point<ClockType> TimeStart = ClockType::now();

    Table.Add("Cat", "Apple");
    Table.Add("Dog", "Banana");
    Table.Add("Hen", "Milk");
    Table.Add("Snake", "Pear");
    Table.Add("Spider", "Peach");
    Table.Add("Cow", "Grape");
    Table.Add("Fly", "Pizza");

    cout << Table << endl;

    string KeyString = "Snake";
    auto p = Table.Find(KeyString);
    if (p) cout << KeyString << " = " << *p << endl;
    else cout << KeyString << " not found "<< endl;

    KeyString = "Dog";
    p = Table.Find(KeyString);
    if (p) cout << KeyString << " = " << *p << endl;
    else cout << KeyString << " not found "<< endl;

    KeyString = "Spider";
    p = Table.Find(KeyString);
    if (p) cout << KeyString << " = " << *p << endl;
    else cout << KeyString << " not found "<< endl;

    KeyString = "Fly";
    p = Table.Find(KeyString);
    if (p) cout << KeyString << " = " << *p << endl;
    else cout << KeyString << " not found "<< endl;

    KeyString = "Cow";
    p = Table.Find(KeyString);
    if (p) cout << KeyString << " = " << *p << endl;
    else cout << KeyString << " not found "<< endl;

    KeyString = "Cat";
    p = Table.Find(KeyString);
    if (p) cout << KeyString << " = " << *p << endl;
    else cout << KeyString << " not found "<< endl;



    if ( !Table.Del("Cow") ) cout << KeyString << "Cow not found "<< endl;

    cout << Table << endl;

    if ( !Table.Add("Cow", "Pizza2") ) cout << "Duplicate!" << endl;

    cout << Table << endl;
    chrono::time_point<ClockType> TimeEnd = ClockType::now();
    double Time = double(std::chrono::duration_cast<std::chrono::nanoseconds>(TimeEnd - TimeStart).count()) / 1.0e9;


    cout << "SimpleTest finished, time, s = " << Time << endl;

    return true;
}


bool TestWithMyClass()
{
    cout << "TestWithMyClass started" << endl;

    HashTable_DA<Moveable, Moveable> Table(0.5, 1);

    const size_t N = 10;

    MyUniqueRandom RndToAdd(N);
    MyUniqueRandom RndToDel(N);

    MyUniqueRandom RndToAdd_2(N*2);

    chrono::time_point<ClockType> TimeStart = ClockType::now();

    for (size_t i = 0; i < N; ++i)
    {
        int kv = RndToAdd.GetNext();
        if ( !Table.Add(to_string(kv).c_str(), to_string(kv*kv).c_str()) )
            cout << "Error: wrong adding" << endl;
    }

    for (size_t i = 0; i < N; ++i)
    {
        int kv = RndToDel.GetNext();
        if ( !Table.Del(to_string(kv).c_str()) )
            cout << "Error: wrong Deleting" << endl;
    }

    for (size_t i = 0; i < N/2; ++i)
    {
        int kv = RndToAdd_2.GetNext();
        if ( !Table.Add(to_string(kv).c_str(), to_string(kv*kv).c_str()) )
            cout << "Error: wrong adding" << endl;
    }

    chrono::time_point<ClockType> TimeEnd = ClockType::now();
    double Time = double(std::chrono::duration_cast<std::chrono::nanoseconds>(TimeEnd - TimeStart).count()) / 1.0e9;


    cout << Table << endl;
    cout << "TestWithMyClass finished, time, s = " << Time << endl;

    return true;
}










