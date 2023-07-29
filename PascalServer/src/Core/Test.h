#pragma once

#include "Application.h"

#include <iostream>

#define TEST_LIST_BEGIN \
	static void InitTest() \
	{


#define TEST_LIST_END \
	}

#define TEST_METHOD_ADD TestFunc()

namespace Pascal 
{
	template<typename T>
	class Test 
	{
	public:

	protected:

		static void TestFunc()
		{
			std::cout << "test\n";
			App().ServerPort = 2;
		}

    private:
        class methodRegistrator
        {
        public:
            methodRegistrator()
            {
                T::InitTest();
            }
        };
        // use static value to register controller method in framework before
        // main();
        static methodRegistrator registrator_;
        virtual void *touch()
        {
            return &registrator_;
        }
	};

    template <typename T>
    typename Test<T>::methodRegistrator
            Test<T>::registrator_;

}