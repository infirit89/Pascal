#include "HttpResponseBuilderTest.h"

#include "Server/HttpResponseBuilder.h"

namespace UnitTests 
{
    using namespace Pascal;

    void HttpResponseBuilderTest1() 
    {
        Shared<HttpResponse> response = CreateShared<HttpResponse>();

        response->SetStatus(HttpStatus::Success);

        std::string responseStr = HttpResponseBuilder::BuildResponse(response);

        PS_TRACE(responseStr);
    }
}