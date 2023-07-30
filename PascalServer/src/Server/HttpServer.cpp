#include "pspch.h"
#include "HttpServer.h"

#include <winsock2.h>

const char* response = "HTTP/1.1 200 OK\r\n";

namespace Pascal 
{
	HttpServer::HttpServer(uint16_t port)
	{
		m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		PS_ASSERT(m_ServerSocket != PS_INVALID_SOCKET, "Cant create socket");

		sockaddr_in serverAddress;
		memset(&serverAddress, 0, sizeof(sockaddr_in));

		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(port);
		serverAddress.sin_addr.s_addr = INADDR_ANY;

		int res = bind(m_ServerSocket, (sockaddr*)&serverAddress, sizeof(sockaddr));
		PS_ASSERT(!res, "Can't bind the socket");

	}

	HttpServer::~HttpServer()
	{
		closesocket(m_ServerSocket);
	}

	void HttpServer::run()
	{
		int res =  listen(m_ServerSocket, 16);
		PS_ASSERT(!res, "Listen failed");

		while (true) 
		{
			ps_socket client = accept(m_ServerSocket, NULL, NULL);
			if (client == PS_INVALID_SOCKET)
				continue;

			std::string buf;
			buf.resize(PS_HTTP_REQUEST_INITIAL_SIZE * 4);


			int bytes = recv(client, &buf[0], PS_HTTP_REQUEST_INITIAL_SIZE * 4, 0);

			PS_TRACE(buf);

			send(client, response, strlen(response), 0);

			closesocket(client);
		}
	}
}

