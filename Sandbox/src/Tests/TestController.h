#pragma once
#include "Server/HttpController.h"
#include "TestUtils.h"

namespace UnitTests 
{
    class TestController : public Pascal::HttpController<TestController> 
    {
    public:
        TestController() 
        {

        }
        
        static void InitTest() 
        {
            SetTest();
            return;
        }


        // void Test();

        // virtual void TestTest() override;
    };
} 