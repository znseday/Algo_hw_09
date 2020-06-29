#ifndef MOVEABLE_H
#define MOVEABLE_H


#define MOVE_ENABLED

extern bool IsDebugOutputM;

#define MY_DEBUG_ONLY_M(x) { if(IsDebugOutputM) {x}  }

#include <iostream>

class Moveable
{
private:
    char *s = nullptr;

public:

    const char *GetS() const {return s;}

    Moveable();

    Moveable(const char *_s);

    Moveable(const Moveable& ob);

    Moveable& operator=(const Moveable& ob);

#ifdef MOVE_ENABLED
    // пусто         данные
    Moveable(Moveable&& ob) noexcept;
    // данные                  данные
    Moveable& operator=(Moveable&& ob) noexcept;
#endif
    ~Moveable();

    void PrintMe() &;
    void PrintMe() &&;

    friend std::ostream & operator<<(std::ostream &_stream, const Moveable &ob);
    friend bool operator<(const Moveable &a, const Moveable &b);

    friend bool operator==(const Moveable &a, const Moveable &b);
};




// custom specialization of std::hash can be injected in namespace std
namespace std
{
    template<> struct hash<Moveable>
    {
        std::size_t operator()(Moveable const &ob) const noexcept
        {
            return std::hash<std::string>{}(ob.GetS());
        }
    };
}




#endif // MOVEABLE_H
