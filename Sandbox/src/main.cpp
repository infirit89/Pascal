#include "Core/Log.h"

using namespace Pascal;

int main() 
{
    Logger::Init();

    PS_TRACE("test");
    PS_INFO("test");
    PS_WARN("test");
    PS_ERROR("test");

	return 0;
}