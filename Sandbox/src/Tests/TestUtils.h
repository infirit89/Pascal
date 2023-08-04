#pragma once

#include "Core/Assert.h"

#include <functional>

namespace UnitTests 
{
    class UnitTestUtils 
    {
    public:
        template<typename T>
        inline static void AssertIfNotEqual(const T& in1, const T& in2) 
        {
            std::equal_to<T> equal;
            PS_ASSERT(equal(in1, in2), "{0}. Failed!", s_TestIndex);

            PS_INFO("{0}. Passed!", s_TestIndex);
            s_TestIndex++;
        }

    private:
        static int s_TestIndex;
    };

    int UnitTestUtils::s_TestIndex = 1;

}
