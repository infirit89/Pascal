#include "pspch.h"
#include "Application.h"

#include <WinSock2.h>

namespace Pascal 
{
	Application::Application()
		: m_ServerSocket(0)
	{
		Pascal::Logger::Init();

		WSADATA data;
		int wsaSuccess = WSAStartup(MAKEWORD(2, 2), &data);
		PS_ASSERT(!wsaSuccess, "Couldn't initialize winsock");
	}

	Application::~Application()
	{
		int wsaShutdown = WSACleanup();
		PS_ASSERT(!wsaShutdown, "Couldn't shutdown winsock");
	}

	void Application::CreateServer()
	{
		m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		PS_ASSERT(m_ServerSocket, "Couldn't create the server socket");
	}
}
