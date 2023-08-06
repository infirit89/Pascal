#include "Core/Log.h"
#include "Core/Application.h"

#include "Core/Assert.h"
#include "Server/HttpServer.h"

#include <thread>
#include <iostream>

#include "Tests/INetAddressTest.h"

using namespace Pascal;

int main() 
{

    std::thread t([]() 
    {
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