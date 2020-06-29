#include "moveable.h"

#include <iostream>
#include <cstring>

using namespace std;



Moveable::Moveable()
{
    MY_DEBUG_ONLY_M (cout << "Default constructor" << endl;)
}

Moveable::Moveable(const char *_s)
{
    s = new char[strlen(_s) + 1];
    strcpy(s, _s);
    MY_DEBUG_ONLY_M (cout << "Param constructor where s = " << _s << endl;)
}

Moveable::Moveable(const Moveable& ob)
{
    if (ob.s)
    {
        s = new char[strlen(ob.s) + 1];
        strcpy(s, ob.s);
    }
    MY_DEBUG_ONLY_M(
        if (s)
            cout << "Copy constructor from = " << s << endl;
        else
            cout << "Copy constructor from = nullptr" << endl;
    )

}

Moveable& Moveable::operator=(const Moveable& ob)
{
    if (s)
    {
        delete[]s;
        s = nullptr;
    }
    if (ob.s)
    {
        s = new char[strlen(ob.s) + 1];
        strcpy(s, ob.s);
    }

    MY_DEBUG_ONLY_M(
        if (s)
            cout << "Copy assignment, s = " << s << endl;
        else
            cout << "Copy assignment, s = nullptr" << endl;
    )

    return *this;
}

#ifdef MOVE_ENABLED

// пусто                  данные
Moveable::Moveable(Moveable&& ob) noexcept
{
    MY_DEBUG_ONLY_M(
        if (ob.s)
           cout << "Move constructor from = " << ob.s << endl;
       else
           cout << "Move constructor from = nullptr" << endl;
    )

    s = ob.s;
    ob.s = nullptr;
}

// данные                            данные
Moveable& Moveable::operator=(Moveable&& ob) noexcept
{
    MY_DEBUG_ONLY_M(
        if (ob.s)
            cout << "Move assignment, s = " << ob.s << endl;
        else
            cout << "Move assignment, s = nullptr" << endl;
    )

    if (s)
        delete[]s;

    s = ob.s;
    ob.s = nullptr;

    return *this;
}

#endif

Moveable::~Moveable()
{
    if (s)
    {
        MY_DEBUG_ONLY_M( cout << "Destructor, where s = " << s << endl; )
        delete[]s;
    }
    else
        MY_DEBUG_ONLY_M( cout << "Destructor, where s = nullptr" << endl; )
}

void Moveable::PrintMe() &
{
    if (s)
        cout << "My Printing (l-value), where s = " << s << endl;
    else
        cout << "My Printing (l-value), where s = nullptr" << endl;
}

void Moveable::PrintMe() &&
{
    if (s)
        cout << "My Printing (r-value), where s = " << s << endl;
    else
        cout << "My Printing (r-value), where s = nullptr" << endl;
}

std::ostream & operator<<(std::ostream &_stream, const Moveable &ob)
{
    if (ob.s)
        _stream << ob.s;
    else
        _stream << "nullptr";

    return _stream;
}

bool operator<(const Moveable &a, const Moveable &b)
{
    return strcmp(a.s, b.s) < 0;
}

bool operator==(const Moveable &a, const Moveable &b)
{
    if ( !a.s || !b.s)
        return false;


    return strcmp(a.s, b.s) == 0;
}




