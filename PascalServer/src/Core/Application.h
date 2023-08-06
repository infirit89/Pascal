#pragma once

#include "Base.h"

#include "Server/HttpServer.h"

namespace Pascal 
{
    class Application
    {
    public:
        static Application& GetInstance() 
        {
            static Application instance;
            return instance;
        }

        void Run();
        void Quit();

        const Shared<EventLoop>& GetEventLoop() const { return m_EventLoop; }

        bool IsRunning() { return m_EventLoop->IsRunning(); }

    private:
        Application();
        ~Application();

        Unique<HttpServer> m_Server;
        Shared<EventLoop> m_EventLoop;

    };

    
    inline Application& App() 
    {
        return Application::GetInstance();
    }
}

