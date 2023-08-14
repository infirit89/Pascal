#pragma once

#include "Base.h"

#include "EventLoop.h"
#include "Server/Router.h"

#include <functional>

namespace Pascal 
{
    class HttpServer;

    Shared<HttpResponse> DefaultErrorHandler(HttpStatus status);

    class Application
    {
    public:
        using ErrorHandler = std::function<Shared<HttpResponse>(HttpStatus)>;

        static Application& GetInstance() 
        {
            static Application instance;
            return instance;
        }

        void Run();
        void Quit();

        const Shared<EventLoop>& GetEventLoop() const { return m_EventLoop; }

        bool IsRunning() { return m_EventLoop->IsRunning(); }

        void AddSimpleHttpResponseHandler(const std::string& path, const Router::ResponseCallback& callback);
        void AddSimpleHttpResponseHandler(const std::string& path, Router::ResponseCallback&& callback);

        void SetErrorHandler(const ErrorHandler& errorHandler) { m_ErrorHandler = errorHandler; }

    private:
        Shared<HttpResponse> OnHttpRequest(const Shared<HttpRequest>& request);
    
        Application();
        ~Application();

        Unique<HttpServer> m_Server;
        Shared<EventLoop> m_EventLoop;
        Unique<Router> m_HttpResponseRouter;
        ErrorHandler m_ErrorHandler;
    };

    
    inline Application& App() 
    {
        return Application::GetInstance();
    }
}

