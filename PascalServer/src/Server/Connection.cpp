#include "pspch.h"
#include "Connection.h"

#include <openssl/err.h>

namespace Pascal 
{
    static const uint32_t s_DefaultRequestSize = PS_HTTP_REQUEST_INITIAL_SIZE * 2;

    Connection::Connection(
                        const Shared<EventLoop>& eventLoop, 
                        ps_socket socketHandle,
                        SSL_CTX* context) 
        : m_EventLoop(eventLoop), 
        m_Socket(socketHandle), 
        m_MessageBuffer(s_DefaultRequestSize)
    {
        m_Event = CreateShared<Event>(socketHandle);

        m_Event->SetCloseCallback(std::bind(&Connection::HandleClose, this));
        m_Event->SetReadCallback(std::bind(&Connection::HandleRead, this));
        m_Event->SetWriteCallback(std::bind(&Connection::HandleWrite, this));
        m_Event->SetErrorCallback(std::bind(&Connection::HandleError, this));

        m_SSL = SSL_new(context);

        m_ReadBio = BIO_new(BIO_s_mem());
        m_WriteBio = BIO_new(BIO_s_mem());

        SSL_set_bio(m_SSL, m_ReadBio, m_WriteBio);
    }

    bool Connection::AcceptSSL() 
    {
        int ret = SSL_accept(m_SSL);

        if(ret != 1) 
        {
            int error = SSL_get_error(m_SSL, ret);

            if(error == SSL_ERROR_WANT_READ) 
            {
                PS_ERROR("want to read should try to negotiate");
                SendTLSData();
                return false;
            }
            else if (error == SSL_ERROR_WANT_WRITE) 
            {
                PS_ERROR("want to write should try to negotiate");
                SendTLSData();
                return false;
            }
            
            PS_ERROR(ERR_error_string(ERR_get_error(), NULL));
            return false;
        }

        PS_INFO("Handshake successful");
        return true;
    }

    void Connection::EstablishConnection() 
    {
        m_EventLoop->QueuePostPollFunction([this]() 
        {
            m_Event->EnableReading();
            m_EventLoop->AddEventDescription(m_Event.get());

            SSL_set_accept_state(m_SSL);
        });
    }

    Connection::~Connection() 
    {
        SSL_shutdown(m_SSL);
        SSL_free(m_SSL);
        PS_INFO("Closing connection for socket {0}", m_Socket.GetHandle());
    }

    void Connection::Send(const char* data, uint32_t size) 
    {
        int n = SSL_write(m_SSL, data, size);
        if(n <= 0 && size != 0)
            return;

        SendTLSData();

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
        PS_TRACE("I cum");

        if(m_WriteCallback)
            m_WriteCallback(shared_from_this());
    }

    void Connection::HandleError() 
    {
        PS_ERROR("Error: {}", strerror(errno));
    }

    void Connection::ProccessSSLData() 
    {
        while(true) 
        {
            int pendingBytes = BIO_pending(m_ReadBio);
            if(pendingBytes <= 0)
                return;

            m_MessageBuffer.Reserve(
                                std::min<size_t>(
                                    PS_HTTP_REQUEST_MAX_SIZE, 
                                    m_MessageBuffer.GetSize() + pendingBytes));

            int n = SSL_read(
                            m_SSL,
                            m_MessageBuffer.GetWritable(),
                            m_MessageBuffer.GetWriteableSize());

            if(n <= 0) 
            {
                int error = SSL_get_error(m_SSL, n);
                PS_ERROR(error);
                PS_ERROR(ERR_error_string(ERR_get_error(), NULL));
                return;
            }
            else 
            {
                m_MessageBuffer.m_Size += n;

                if(m_RecieveMessageCallback)
                    m_RecieveMessageCallback(shared_from_this(), m_MessageBuffer);
            }
        }
    }

    void Connection::WriteRaw(const char* data, uint32_t size) 
    {
        int writtenSize = write(m_Socket.GetHandle(), data, size);
        if(writtenSize == -1) 
        {
            PS_ERROR(strerror(errno));
        }

        PS_TRACE("Data Size: {0}; Written Size: {1}", size, writtenSize);
    }

    void Connection::SendTLSData() 
    {
        void* data = nullptr;
        int len = BIO_get_mem_data(m_WriteBio, &data);

        if(len <= 0 || !data)
            return;
        
        WriteRaw((const char*)data, len);
        BIO_reset(m_WriteBio);
    }

    void Connection::HandleRead() 
    {
        int bytes;

        bytes = recv(m_Socket.GetHandle(), m_MessageBuffer.GetWritable(), 
        m_MessageBuffer.GetCapacity() - m_MessageBuffer.GetSize(), 0);

        if(bytes <= 0) 
        {
            HandleClose();
            return;
        }

        if(bytes > m_MessageBuffer.GetCapacity()) 
        {
            PS_ERROR("too long");
            return;
        }

        if(bytes > 0) 
        {
            m_MessageBuffer.m_Size += bytes;
            m_MessageBuffer.m_Data[m_MessageBuffer.m_Size] = '\0';
        }

        while(m_MessageBuffer.GetReadableSize() > 0) 
        {
            int n = BIO_write(
                            m_ReadBio,
                            m_MessageBuffer.Peek(),
                            m_MessageBuffer.GetReadableSize());

            if(n <= 0) 
            {
                PS_ERROR("cum");
                return;
            }

            m_MessageBuffer.Advance(n);

            if(!SSL_is_init_finished(m_SSL)) 
            {
                bool success = AcceptSSL();
                if(success)
                    ProccessSSLData();
            }
            else
                ProccessSSLData();
        }
    }
}