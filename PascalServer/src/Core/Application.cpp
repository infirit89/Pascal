#include "pspch.h"
#include "Application.h"

namespace Pascal 
{
	Application::Application()
	{
		Pascal::Logger::Init();
		m_EventLoop = CreateShared<EventLoop>();
		m_Server = CreateUnique<HttpServer>(m_EventLoop);
	}

	Application::~Application()
	{
		m_Server.reset();
		m_EventLoop.reset();
	}

	void Application::Run() 
	{
		m_Server->Run();
	}

	void Application::Quit() 
	{
		m_EventLoop->Quit();
	}
}
