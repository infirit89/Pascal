#include "Core/Log.h"
#include "Core/Application.h"

#include "Core/Assert.h"
#include "Server/HttpServer.h"

#include <thread>

using namespace Pascal;

int main() 
{
    (void)App();

    PS_TRACE(std::this_thread::get_id());

    HttpServer server;

    server.Run();

	return 0;
}