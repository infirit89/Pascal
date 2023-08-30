#include "pspch.h"
#include "HttpServer.h"

#include "HttpRequestParser.h"
#include "HttpResponseBuilder.h"

#include <iostream>

#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

namespace Pascal 
{
    const char* response = "HTTP/1.1 200 OK\r\n";

    HttpServer::HttpServer(const Shared<EventLoop>& eventLoop)
        : m_ListenerSocket(Socket::CreateSocketNonBlocking(AF_INET)),
          m_EventLoop(eventLoop)
    {
        m_ListenerSocket.SetReuseAddress(true);
        m_ListenerSocket.SetReusePort(true);

        INetAddress address(8080);
        m_ListenerSocket.Bind(address);

        m_ListenerEvent = CreateShared<Event>(m_ListenerSocket.GetHandle());
        m_ListenerEvent->SetReadCallback(
                                        std::bind(
                                                &HttpServer::HandleAccept, 
                                                this));

        m_RecvStr.resize(PS_HTTP_REQUEST_INITIAL_SIZE * 2);

        m_ListenerSocket.Listen();

        m_ListenerEvent->EnableReading();
        m_EventLoop->AddEventDescription(m_ListenerEvent.get());

        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();
        ERR_load_BIO_strings();
        ERR_load_crypto_strings();

        m_Context = SSL_CTX_new(TLS_server_method());

        int ret = SSL_CTX_use_certificate_file(
                                            m_Context,
                                            "Assets/localhost.crt",
                                            SSL_FILETYPE_PEM);

        PS_ASSERT(ret == 1, "Couldn't load the certificate file");

        ret = SSL_CTX_use_PrivateKey_file(
                                            m_Context,
                                            "Assets/localhost.decrypted.key",
                                            SSL_FILETYPE_PEM);

        PS_ASSERT(ret == 1, "Couldn't load the private key file");

        ret = SSL_CTX_check_private_key(m_Context);
        PS_ASSERT(ret == 1, "Private key doesn't match");
    }

    HttpServer::~HttpServer()
    {
        m_EventLoop.reset();
        m_ListenerEvent.reset();
        SSL_CTX_free(m_Context);
    }

    void HttpServer::HandleAccept() 
    {
        INetAddress clientAddress;
        ps_socket clientHandle = m_ListenerSocket.Accept(clientAddress);

        PS_ASSERT(clientHandle != PS_INVALID_SOCKET, "Couldn't accept a valid client");

        std::shared_ptr<Connection> connection =
                                    std::make_shared<Connection>(
                                                                m_EventLoop, 
                                                                clientHandle,
                                                                m_Context);

        connection->SetReadCallback(
                                std::bind(
                                        &HttpServer::HandleReadPeer,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2));

        connection->SetCloseCallback(
                                std::bind(
                                    &HttpServer::HandleClosePeer,
                                    this,
                                    std::placeholders::_1));

        m_Connections.insert(connection);

        connection->EstablishConnection();
    }

    void HttpServer::HandleReadPeer(Shared<Connection> connection, Buffer& buffer) 
    {
        HttpRequestParser::Status status;
        PS_TRACE(buffer);

        Shared<HttpRequest> request = HttpRequestParser::ParseRequest(buffer, status);

        Shared<HttpResponse> response;

        switch(status) 
        {
        case HttpRequestParser::Status::Success:
            response = m_OnHttpRequest(request);
            break;
        case HttpRequestParser::Status::IllformedRequest:
            response = CreateShared<HttpResponse>(HttpStatus::BadRequest);
            break;
        case HttpRequestParser::Status::HttpVersionNotSupported:
            response = CreateShared<HttpResponse>(HttpStatus::VersionNotSupported);
            break;
        case HttpRequestParser::Status::UnexpectedMethod:
            response = CreateShared<HttpResponse>(HttpStatus::MethodNotAllowed);
            break;
        case HttpRequestParser::Status::URITooLong:
            response = CreateShared<HttpResponse>(HttpStatus::UriTooLong);
            break;
        }

        SendResponse(connection, response);
    }
    
    void HttpServer::SendResponse(
                                const Shared<Connection>& connection,
                                const Shared<HttpResponse>& response) 
    {
        std::string responseData = HttpResponseBuilder::BuildResponse(response);

        connection->Send(responseData);
    }

    void HttpServer::HandleClosePeer(Shared<Connection> connection) 
    {
        m_Connections.erase(connection);
    }

    void HttpServer::Run() 
    {
        m_EventLoop->Run();
    }
}