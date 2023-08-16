#include "pspch.h"
#include "Connection.h"

namespace Pascal 
{
    static const uint32_t s_DefaultRequestSize = PS_HTTP_REQUEST_INITIAL_SIZE * 2;

    Connection::Connection(
                        const Shared<EventLoop>& eventLoop, 
                        ps_socket socketHandle) 
        : m_EventLoop(eventLoop), 
        m_Socket(socketHandle), 
        m_MessageBuffer(s_DefaultRequestSize)
    {
        m_Event = CreateShared<Event>(socketHandle);

        m_Event->SetCloseCallback(std::bind(&Connection::HandleClose, this));
        m_Event->SetReadCallback(std::bind(&Connection::HandleRead, this));
        m_Event->SetWriteCallback(std::bind(&Connection::HandleWrite, this));
        m_Event->SetErrorCallback(std::bind(&Connection::HandleError, this));
    }

    void Connection::EstablishConnection() 
    {
        m_EventLoop->QueuePostPollFunction([this]() 
        {
            m_Event->EnableReading();
            m_EventLoop->AddEventDescription(m_Event.get());
        });
    }

    Connection::~Connection() 
    {
        PS_INFO("Closing connection for socket {0}", m_Socket.GetHandle());
    }

    void Connection::Send(const char* data, uint32_t size) 
    {
        // send(m_Socket.GetHandle(), data, size, 0);
        int writtenSize = write(m_Socket.GetHandle(), data, size);
        if(writtenSize == -1) 
        {
            PS_ERROR(strerror(errno));
        }
        

        PS_TRACE("Data Size: {0}; Written Size: {1}", size, writtenSize);

        m_Socket.CloseWriting();
    }

    void Connection::Send(const std::string& data) 
    {
        Send(data.c_str(), data.size());
    }

    void Connection::HandleClose() 
    {
        m_EventLoop->RemoveEventDescription(m_Event.get());

        auto thisPtr = shared_from_this();

        if(m_CloseCallback)
            m_CloseCallback(thisPtr);
    }

    void Connection::HandleWrite() 
    {
        if(m_WriteCallback)
            m_WriteCallback(shared_from_this());
    }

    void Connection::HandleError() 
    {

    }

    void Connection::HandleRead() 
    {
        int bytes;

        bytes = recv(m_Socket.GetHandle(), m_MessageBuffer.GetWritable(), 
        m_MessageBuffer.GetCapacity() - m_MessageBuffer.GetSize(), 0);

        if(bytes > m_MessageBuffer.GetCapacity())
            PS_ERROR("too long");

        if(bytes > 0) 
        {
            m_MessageBuffer.m_Size += bytes;
            m_MessageBuffer.m_Data[m_MessageBuffer.m_Size] = '\0';
        }


        if(bytes == 0) 
        {
            HandleClose();
            return;
        }

        if(m_RecieveMessageCallback)
            m_RecieveMessageCallback(shared_from_this(), m_MessageBuffer);
    }
}