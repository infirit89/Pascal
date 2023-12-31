#include "Core/Log.h"
#include "Core/Application.h"

#include "Core/Assert.h"
#include "Server/HttpServer.h"

#include "Utils/Utils.h"

#include <thread>
#include <iostream>

#include "Tests/INetAddressTest.h"
#include "Tests/HttpRequestParserTest.h"
#include "Tests/HttpResponseBuilderTest.h"
#include "Tests/TestController.h"

#include <vector>
#include <fstream>

#include <json.hpp>

using namespace Pascal;

Shared<HttpResponse> TestHandler(const Shared<HttpRequest>& request) 
{
    Shared<HttpResponse> response = CreateShared<HttpResponse>();

    std::ifstream ifs("Assets/index.html", std::ifstream::in);
    if(ifs)
    {
        ifs.seekg(0, ifs.end);
        size_t fileSize = ifs.tellg();
        ifs.seekg(0, ifs.beg);

        char* fileData = new char[fileSize + 1];

        ifs.read(fileData, fileSize);
        fileData[fileSize] = '\0';

        response->SetBody(fileData, fileSize);

        // PS_TRACE(response->GetBody());

        ifs.close();

        delete[] fileData;
    }

    response->SetStatus(HttpStatus::Success);
    return response;
}

Shared<HttpResponse> JSHandler(const Shared<HttpRequest>& request) 
{
    Shared<HttpResponse> response = CreateShared<HttpResponse>();

    std::ifstream ifs("Assets/app.js", std::ifstream::in);
    if(ifs)
    {
        ifs.seekg(0, ifs.end);
        size_t fileSize = ifs.tellg();
        ifs.seekg(0, ifs.beg);

        char* fileData = new char[fileSize + 1];

        ifs.read(fileData, fileSize);
        fileData[fileSize] = '\0';

        response->SetBody(fileData, fileSize);

        // PS_TRACE(response->GetBody());

        ifs.close();

        delete[] fileData;
    }

    return response;
}

Shared<HttpResponse> TestHandleer2(const Shared<HttpRequest>& request) 
{
    using json = nlohmann::json;
    using namespace nlohmann::literals;

    auto j = R"(
        "updated"
    )"_json; 

    Shared<HttpResponse> response = CreateShared<HttpResponse>(
                                                            j.dump(), 
                                                            HttpStatus::Success);

    response->SetContentType(HttpContentType::Applicaion_JSON);

    return response;
}

Shared<HttpResponse> ErrorHandler(HttpStatus status) 
{
    Shared<HttpResponse> response = CreateShared<HttpResponse>();
    std::ifstream ifs("Assets/error.html", std::ios::in);

    if(ifs) 
    {
        ifs.seekg(0, ifs.end);
        size_t fileSize = ifs.tellg();
        ifs.seekg(0, ifs.beg);

        char* data = new char[fileSize + 1];

        ifs.read(data, fileSize);
        data[fileSize] = '\0';

        std::string fileData = data;
        

        response->SetBody(data, fileSize);

        delete[] data;
    }

    return response;
}

Shared<HttpResponse> TestHandleer3(const Shared<HttpRequest>& request, int test) 
{
    Shared<HttpResponse> response = CreateShared<HttpResponse>();

    return response;
}

int main() 
{
    std::thread t([]() 
    {
        App().MountPath("Assets/");
        App().SetErrorHandler(ErrorHandler);
        App().AddSimpleHttpResponseHandler("/test", &TestHandleer2, HttpMethod::Post);
        // App().AddSimpleHttpResponseHandler("/test5?test={number}", &UnitTests::TestController::TestHandler);
        App().Run();
    });

    while(App().IsRunning()) 
    {
        std::string input;
        std::getline(std::cin, input);

        if(input == "exit") 
        {
            App().GetEventLoop()->QueuePostPollFunction([]() 
            {
                App().Quit();
            });
        }
    }

    t.join();
	return 0;
}
