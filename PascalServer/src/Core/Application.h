#pragma once

#include "Base.h"
#include "Log.h"

#include "EventLoop.h"
#include "Server/StaticFileRouter.h"
#include "Server/BasicRouter.h"

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

        template<typename Function>
        void AddSimpleHttpResponseHandler(
                                        const std::string& path,
                                        Function&& callback,
                                        HttpMethod allowedMethods = HttpMethod::Get) 
        {
            PS_INFO("Adding handler for route: {0}; Allowed methods: {1}", path, allowedMethods);
            auto binder = 
                    CreateShared<HttpParameterBinder<Function>>(
                                                            std::forward<Function>(callback));
            
            m_HttpResponseRouter->AddRoute(path, binder, allowedMethods);
        }

        void MountPath(const std::filesystem::path& path);

        void SetErrorHandler(const ErrorHandler& errorHandler) 
        { m_ErrorHandler = errorHandler; }

    private:
        Shared<HttpResponse> OnHttpRequest(const Shared<HttpRequest>& request);
    
        Application();
        ~Application();

        Unique<HttpServer> m_Server;
        Shared<EventLoop> m_EventLoop;
        Unique<RouterManager> m_RouterManager;

        Shared<BasicRouter> m_HttpResponseRouter;
        Shared<StaticFileRouter> m_StaticFileRouter;
        ErrorHandler m_ErrorHandler;
    };

    
    inline Application& App() 
    {
        return Application::GetInstance();
    }
}

