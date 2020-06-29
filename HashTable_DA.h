#ifndef HASHTABLE_DA_H
#define HASHTABLE_DA_H

#include "IHash.h"

#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>

template <typename K, typename V>
class HashTable_DA : public IHash<K, V>
{
protected:

    size_t MaxLimit = 0;
    size_t Size = 0;

    double MaxLoadFactor = 0.5; // Если правильно понял, то для квадратичного проббинга нужно <= 0.5
    double LoadFactor = 0;

    size_t StartSize = 6; // Четное число, чтобы хорошо "сочеталось" с MaxLoadFactor = 0.5

    static constexpr double ExpandFactor = 2;
    static constexpr size_t ProbbingTryFactor = 10;

    struct RecordType
    {
        K Key;
        V *Value = nullptr;
        bool Deleted = false;
    };

    std::vector<RecordType> Table;

    void ReCalcLimit() {MaxLimit = Table.size()*MaxLoadFactor;}

//    std::function<size_t(const T &)> Hash;

    size_t Probbing(const K &_key, size_t i) const;

    bool Add_inside(const K &_key, V *_value); // Для рехеширования

    HashTable_DA & operator=(HashTable_DA &&) noexcept = default; // Только для внутреннего использования

    RecordType * Find_inside(const K &_key);

public:

    //HashTable_DA() = delete;
    HashTable_DA(double _maxLoadFactor = 0.5, size_t _startSize = 6);

    HashTable_DA(const HashTable_DA &) = delete;
    HashTable_DA(HashTable_DA &&) noexcept = delete;

    HashTable_DA & operator=(const HashTable_DA &) = delete;

    virtual bool Add(const K &_key, const V &_value) override;

    virtual V * Find(const K &_key) override;
    virtual bool Del(const K &_key) override;

    virtual void Rehash() override;

    virtual double GetMaxLoadFactor() const override {return MaxLoadFactor;};

    virtual double GetLoadFactor() const override {return LoadFactor;};


    void ClearAll();

    ~HashTable_DA() {ClearAll();}

    friend std::ostream & operator<<(std::ostream &s, const HashTable_DA &_table)
    {
        s << "n" << "\t" << "deleted" << "\t" << "key" << "\t" << "value" << std::endl;

        size_t i = 0;
        for (const auto &item : _table.Table)
        {
            s << i << "\t" << item.Deleted << "\t" << item.Key << "\t" << (item.Value ? *item.Value : "empty") << std::endl;

            i++;
        }

        return s;
    }

};
//--------------------------------------------------------

template <typename K, typename V>
HashTable_DA<K,V>::HashTable_DA(double _maxLoadFactor, size_t _startSize) : MaxLoadFactor(_maxLoadFactor), StartSize(_startSize)
{
    Table.resize(StartSize);
    ReCalcLimit();
}
//--------------------------------------------------------

template <typename K, typename V>
void HashTable_DA<K,V>::ClearAll()
{
    for (auto & item : Table)
    {
        if (item.Value)
        {
            delete item.Value;
            item.Value = nullptr;
        }
        item.Deleted = false;
    }
    Size = 0;
}
//--------------------------------------------------------

template <typename K, typename V>
size_t HashTable_DA<K,V>::Probbing(const K &_key, size_t i) const
{
    constexpr size_t M = 997; // Как его выбрать, если не знаем максимального размера таблицы?

    // Пересчитываем хеш каждый раз - теоретически можно как-нибудь закешировать

    return ((std::hash<K>{}(_key) + i*i) % M) % Table.size();
}
//--------------------------------------------------------

template <typename K, typename V>
bool HashTable_DA<K,V>::Add_inside(const K &_key, V *_value)
{
    size_t i;
    for (i = 0; i < Table.size() * ProbbingTryFactor; ++i) // сколько давать попыток на проббинг?
    {
        size_t ind = Probbing(_key, i);

        if ( Table[ind].Value ) // если в таблице уже есть элемент с таким хешем
        {
            ind = Probbing(_key, i);
        }
        else // пусто - переносим из старой таблицы таблицы
        {
            MY_DEBUG_ONLY( std::cout << "(Add_inside) Probbing try # " << i << std::endl; )

            Table[ind].Key = _key;
            Table[ind].Value = _value; // воруем указатель

            Size++;

            return true;
        }
    }

    if (i == Table.size()) // Для подстраховки и самоконтроля
        throw std::runtime_error("i == Table.size(), probbing failed");

    return false;
}
//--------------------------------------------------------

template <typename K, typename V>
bool HashTable_DA<K,V>::Add(const K &_key, const V &_value)
{
    size_t i;  

    for (i = 0; i < Table.size() * ProbbingTryFactor; ++i) // Сколько создават попыток для проббинга ???????
    {
        size_t ind = Probbing(_key, i);

        if ( Table[ind].Value && !Table[ind].Deleted ) // если в таблице уже есть элемент с таким хешем
        {
            if ( Table[ind].Key == _key) // попали на дубликат по _key
                return false;

//            ind = (ind+1) % Table.size(); // заменить на пробинг
            ind = Probbing(_key, i);
            continue;
        }
        else // пусто - создаем элемент
        {
            MY_DEBUG_ONLY( std::cout << "(Add) Probbing try # " << i << std::endl; )

            if ( Table[ind].Deleted ) // Попали на элемент, который помечен как удаленный
            {
                delete Table[ind].Value; // Удаляем старый
                Table[ind].Deleted = false;
                Size--;
            }

            Table[ind].Key = _key;
            Table[ind].Value = new V(_value); // Здесь допущение, что T имеет конструктор копирования

            Size++;

            if (Size >= MaxLimit) // Делаем упрощенный вариант, что таблица не может быть заполнена на 100 %
                Rehash();

            return true;
        }
    }

    if (i == Table.size()) // Для подстраховки и самоконтроля
        throw std::runtime_error("i == Table.size(), probbing failed");

    return false;
}
//--------------------------------------------------------

template <typename K, typename V>
V * HashTable_DA<K,V>::Find(const K &_key)
{
    auto p = Find_inside(_key);
    if ( p )
        return p->Value;
    else
        return nullptr;
}
//--------------------------------------------------------

template <typename K, typename V>
typename HashTable_DA<K,V>::RecordType * HashTable_DA<K,V>::Find_inside(const K &_key)
{
    size_t i;

    size_t indFirstDeleted;
    bool isFoundDeleted = false;

    for (i = 0; i < Table.size() * ProbbingTryFactor; ++i) // сколько давать попыток на проббинг?
    {
        size_t ind = Probbing(_key, i);

        if ( Table[ind].Deleted ) // если попали на Удаленный
        {
            indFirstDeleted = ind;
            isFoundDeleted = true;

            ind = Probbing(_key, i);
            continue;
        }


        if ( _key ==  Table[ind].Key)
        {
            MY_DEBUG_ONLY( std::cout << "(Find_inside) Probbing try # " << i << std::endl; )

            if ( Table[ind].Value )
            {
                if (isFoundDeleted)
                {
                    std::swap(Table[ind], Table[indFirstDeleted]);
                    return &Table[indFirstDeleted];
                }

                return &Table[ind];
            }
            else // попали на пустоту - сразу выходим?
            {
                return nullptr;
            }
        }
        else // Ключи не совпали - делаем проббинг дальше
        {
            ind = Probbing(_key, i);
        }
    }

    if (i == Table.size()) // Для подстраховки и самоконтроля
        throw std::runtime_error("i == Table.size(), probbing failed");

    return nullptr;
}
//--------------------------------------------------------

template <typename K, typename V>
bool HashTable_DA<K,V>::Del(const K &_key)
{
    auto pFind = Find_inside(_key);

    if ( pFind )
    {
        pFind->Deleted = true;
        //Size--;
        return true;
    }
    else
        return false;
}
//--------------------------------------------------------

template <typename K, typename V>
void HashTable_DA<K,V>::Rehash()
{
    MY_DEBUG_ONLY( std::cout << "ReHash" << std::endl; )

//    std::vector<RecordType> NewTable;
//    Table.resize(StartSize);

    HashTable_DA<K,V> NewHashTable(MaxLoadFactor, StartSize*ExpandFactor);

    for (auto & item : Table )
    {
        if ( !item.Value )
            continue;

        if ( item.Deleted )
        {
            delete item.Value;
        }
        else
        {
            NewHashTable.Add_inside(item.Key, item.Value);
        }
    }

    *this = std::move(NewHashTable); // Чтобы вызвать move для ключей, если они перемещаемого типа

    ReCalcLimit();

    MY_DEBUG_ONLY( std::cout << *this << std::endl; )
}


#endif // HASHTABLE_DA_H
