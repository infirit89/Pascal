#include "Core/Log.h"
#include "Core/Application.h"

#include "Core/Assert.h"
#include "Server/HttpServer.h"

#include <thread>
#include <iostream>

#include "Tests/INetAddressTest.h"
#include "Tests/HttpRequestParserTest.h"
#include "Tests/HttpResponseBuilderTest.h"

#include <vector>
#include <fstream>

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

        response->SetBody(fileData);

        // PS_TRACE(response->GetBody());

        ifs.close();

        delete[] fileData;
    }

    // response->SetStatus(HttpStatus::Success);
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

        response->SetBody(fileData);

        // PS_TRACE(response->GetBody());

        ifs.close();

        delete[] fileData;
    }

    return response;
}

Shared<HttpResponse> TestHandleer2(const Shared<HttpRequest>& request) 
{
    Shared<HttpResponse> response = CreateShared<HttpResponse>();
    return response;
}

int main() 
{
    std::thread t([]() 
    {
        App().AddSimpleHttpResponseHandler("/", TestHandler);
        App().AddSimpleHttpResponseHandler("/app.js", JSHandler);
        App().AddSimpleHttpResponseHandler("/test", TestHandleer2);
        App().Run();
    });

    while(App().IsRunning()) 
    {
        std::string input;
        std::cin >> input;

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