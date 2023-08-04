#pragma once

#include "Core/Base.h"

namespace Pascal 
{
    class Socket 
    {
    public:
        Socket();
        ~Socket();

        void Listen();
        void Bind();

    private:
        ps_socket m_Handle;
    };
}
