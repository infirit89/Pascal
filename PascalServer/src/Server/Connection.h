#pragma once

#include "Core/EventLoop.h"
#include "Core/Base.h"
#include "Core/Buffer.h"
#include "Network/Socket.h"

#include <functional>
#include <memory>

namespace Pascal 
{

    class Connection : public std::enable_shared_from_this<Connection>
    {
    public:
        using RecieveMessageCallback = std::function<void(Shared<Connection>, const Buffer& message)>;
        using WriteCallback = std::function<void(Shared<Connection>)>;
        using CloseCallback = std::function<void(Shared<Connection>)>;


        Connection(const Shared<EventLoop>& eventLoop, ps_socket socketHandle);
        ~Connection();

        void SetReadCallback(const RecieveMessageCallback& callback) 
        {
            m_RecieveMessageCallback = callback;
        }

        void SetReadCallback(RecieveMessageCallback&& callback) 
        {
            m_RecieveMessageCallback = std::move(callback);
        }

        void SetWriteCallback(const WriteCallback& callback) 
        {
            m_WriteCallback = callback;
        }

        void SetCloseCallback(const CloseCallback& callback) 
        {
            m_CloseCallback = callback;
        }

        void EstablishConnection();

        void Send(const char* data, uint32_t size);
        void Send(const std::string& data);
    
    private:
        void HandleClose();
        void HandleWrite();
        void HandleError();
        void HandleRead();

        Shared<EventLoop> m_EventLoop;
        Socket m_Socket;
        Shared<Event> m_Event;
        Buffer m_MessageBuffer;
        RecieveMessageCallback m_RecieveMessageCallback;
        WriteCallback m_WriteCallback;
        CloseCallback m_CloseCallback;
    };
}
