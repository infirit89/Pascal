#include "Core/Log.h"
#include "Core/Application.h"

#include "Core/Assert.h"
#include "Server/HttpServer.h"

#include <thread>

#include "Tests/INetAddressTest.h"

using namespace Pascal;

int main() 
{
    (void)App();

    HttpServer server;
    server.Run();

	return 0;
}