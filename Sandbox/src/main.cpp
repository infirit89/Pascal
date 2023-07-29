#include "Core/Application.h"
#include "Core/Test.h"
#include "Core/Test2.h"
#include "Core/Log.h"
#include "Core/Assert.h"

int main() 
{
	int serverPort = Pascal::App().ServerPort;

	PS_TRACE("{}", serverPort);

    system("pause");

	return 0;
}