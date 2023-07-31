#pragma once

#include "Base.h"

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

        int ServerPort = 0;
        

    private:
        Application();
        ~Application();

        void CreateServer();

        ps_socket m_ServerSocket = 0;
        //std::string m_IPAddress;
    };

    
    inline Application& App() 
    {
        return Application::GetInstance();
    }
}

