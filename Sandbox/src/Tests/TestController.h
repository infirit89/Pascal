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

        Pascal::Shared<Pascal::HttpResponse> TestHandler(
                                                const Pascal::Shared<Pascal::HttpRequest>& request,
                                                int test);

        PS_BEGIN_METHOD_LIST
        PS_REGISTER_METHOD_G("/test5?test={number}", 
                            TestController::TestHandler)

        PS_END_METHOD_LIST
    };
}
