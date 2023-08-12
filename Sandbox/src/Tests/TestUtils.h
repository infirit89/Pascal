#pragma once

#include "Core/Assert.h"

#include <functional>

namespace UnitTests 
{
    class Utils 
    {
    public:
        template<typename T>
        inline static void AssertIfNotEqual(const T& in1, const T& in2) 
        {
            std::equal_to<T> equal;
            PS_ASSERT(equal(in1, in2), "{0}. Failed! Values: {1}; {2}", s_TestIndex, in1, in2);

            PS_INFO("{0}. Passed!", s_TestIndex);
            s_TestIndex++;
        }

    private:
        static int s_TestIndex;
    };
}
