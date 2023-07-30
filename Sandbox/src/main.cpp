#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include "Server/HttpServer.h"

using namespace Pascal;

int main() 
{
	App();

	HttpServer server(8080);

	server.run();

	return 0;
}