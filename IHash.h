#ifndef IHASH_H
#define IHASH_H

extern bool IsDebugOutput;

#define MY_DEBUG_ONLY(x) { if(IsDebugOutput) {x} }

template <typename K, typename V>
class IHash
{
public:

    virtual bool Add(const K &_key, const V &_value) = 0;

//    virtual bool Find(const K &_key) = 0;
    virtual V * Find(const K &_key) = 0;

    virtual bool Del(const K &_key) = 0;

    virtual void Rehash() = 0;

    virtual double GetMaxLoadFactor() const = 0;

    virtual double GetLoadFactor() const = 0;

};



#endif // IHASH_H
