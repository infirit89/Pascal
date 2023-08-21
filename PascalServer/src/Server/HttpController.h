#pragma once

#include "Core/Application.h"

// #define SET_TEST_BEGIN \
//     static void InitTest() \
//     {

// #define SET_TEST() \
//     SetTest()

// #define SET_TEST_END \
//     }

namespace Pascal 
{
    template<typename T>
    class HttpController
    {
    public:

        HttpController() = default;
        virtual ~HttpController() = default;

        static void SetTest() 
        {
            App().SetTest();
        }
    private:
        class Registrator
        {
        public:
            Registrator() 
            {
                T::InitTest();
            }
        };

        static Registrator s_Registrator;
        virtual void *touch()
        {
            return &s_Registrator;
        }
    };

    template<typename T>
    typename HttpController<T>::Registrator
        HttpController<T>::s_Registrator;
}
