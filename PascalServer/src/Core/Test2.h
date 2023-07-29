#pragma once

#include "Test.h"

namespace Pascal 
{
	class Test2 : public Test<Test2>
	{
	public:
		TEST_LIST_BEGIN
		TEST_METHOD_ADD;
		TEST_LIST_END

		void Testing();

	};
}