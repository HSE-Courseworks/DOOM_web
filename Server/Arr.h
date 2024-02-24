#pragma once

#include <exception>
#include <initializer_list>
#include <iterator>

#include "Server.h"

namespace Server {
    template<typename T, int size_>
    class Arr : public std::array<T, size_>
    {
    public:
        Arr();
        Arr(Arr& exmp);
        Arr(std::initializer_list<T> list);

        T& operator[](int ind);

        void push_back(T& conn);
        int size();

    private:
        int arrSize;
    };

    template<typename T, int size_>
    inline Arr<T, size_>::Arr() : arrSize(0)
    {
    }

    template<typename T, int size_>
    inline Arr<T, size_>::Arr(Arr& exmp)
    {
        arrSize = exmp.arrSize;
        for (int i = 0; i < arrSize; ++i)
            std::array<T, size_>::data()[i] = exmp.data()[i];
    }

    template<typename T, int size_>
    inline Arr<T, size_>::Arr(std::initializer_list<T> list) : std::array<T, size>(list)
    {
    }

    template<typename T, int size_>
    inline T& Arr<T, size_>::operator[](int ind)
    {
        if (ind < 0 || ind >= arrSize)
            throw std::exception("Index out of range.");
        return std::array<T, size_>::data()[ind];
    }

    template<typename T, int size_>
    inline void Arr<T, size_>::push_back(T& conn)
    {
        if (arrSize < MAX_PLAYERS)
            std::array<T, size_>::data()[arrSize++] = conn;
    }

    template<typename T, int size_>
    inline int Arr<T, size_>::size()
    {
        return arrSize;
    }
}