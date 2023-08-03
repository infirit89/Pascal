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

    };

    
    inline Application& App() 
    {
        return Application::GetInstance();
    }
}

