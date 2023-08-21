#include "TestController.h"


namespace UnitTests 
{
    using namespace Pascal;

    Shared<HttpResponse> TestController::TestHandler(
                                                const Shared<HttpRequest>& request,
                                                int test) 
    {
        Shared<HttpResponse> response = CreateShared<HttpResponse>();
        PS_WARN(test);

        return response;
    }
}
