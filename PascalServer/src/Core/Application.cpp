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
		m_Server->SetHttpMessageCallback(std::bind(&Application::OnHttpRequest, this, std::placeholders::_1));
		m_HttpResponseRouter = CreateUnique<Router>();
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

	void Application::AddSimpleHttpResponseHandler(const std::string& path, const Router::ResponseCallback& callback) 
	{
		PS_INFO("Adding handler for route: {0}", path);
		m_HttpResponseRouter->AddRoute(path, callback);
	}

	void Application::AddSimpleHttpResponseHandler(const std::string& path, Router::ResponseCallback&& callback) 
	{
		PS_INFO("Adding handler for route: {0}", path);
		m_HttpResponseRouter->AddRoute(path, callback);
	}

	Shared<HttpResponse> Application::OnHttpRequest(const Shared<HttpRequest>& request) 
	{
		PS_TRACE("Routing request {0}", request);

		Shared<HttpResponse> response = m_HttpResponseRouter->Route(request);

		if(!response) 
		{
			PS_ERROR("Failed to route request; Resource not found");
			response = m_ErrorHandler(HttpStatus::NotFound);
		}

		return response;
	}
}
