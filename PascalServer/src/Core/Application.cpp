#include "pspch.h"
#include "Application.h"

#include "Server/HttpServer.h"

namespace Pascal 
{
	Shared<HttpResponse> DefaultErrorHandler(HttpStatus status) 
	{
		return CreateShared<HttpResponse>(status);
	}

	Application::Application()
	{
		Pascal::Logger::Init();
		m_EventLoop = CreateShared<EventLoop>();
		m_Server = CreateUnique<HttpServer>(m_EventLoop);
		m_Server->SetHttpMessageCallback(
										std::bind(&Application::OnHttpRequest, 
										this, std::placeholders::_1));

		m_HttpResponseRouter = CreateShared<BasicRouter>();
		m_StaticFileRouter = CreateShared<StaticFileRouter>();

		m_RouterManager = CreateUnique<RouterManager>();
		m_RouterManager->AddRouter(m_HttpResponseRouter);
		m_RouterManager->AddRouter(m_StaticFileRouter);

		m_ErrorHandler = &DefaultErrorHandler;
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

	void Application::MountPath(const std::filesystem::path& path) 
	{
		PS_INFO("Mounting path: {0}", path);
		m_StaticFileRouter->AddPath(path);
	}

	Shared<HttpResponse> Application::OnHttpRequest(const Shared<HttpRequest>& request) 
	{
		PS_TRACE("Routing request {0}", request);

		Router::RoutingError error;
		Shared<HttpResponse> response = m_RouterManager->TryRoute(request, error);

		if(!response)
		{
			switch (error)
			{
			case Router::RoutingError::Forbidden:
				PS_ERROR("Failed to route request; Request forbidden");
				response = m_ErrorHandler(HttpStatus::Forbidden);
				break;
			case Router::RoutingError::MethodNotAllowed:
				PS_ERROR("Failed to route request; Request method not allowed");
				response = m_ErrorHandler(HttpStatus::MethodNotAllowed);
				break;
			case Router::RoutingError::ResourceNotFound:
				PS_ERROR("Failed to route request; Resource not found");
				response = m_ErrorHandler(HttpStatus::NotFound);
				break;
			}
		}

		return response;
	}
}
