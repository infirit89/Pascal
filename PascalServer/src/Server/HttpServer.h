#pragma once

#include "Core/Base.h"

namespace Pascal 
{
	class HttpServer 
	{
	public:
		HttpServer(uint16_t port);
		~HttpServer();

		void run();

	private:
		ps_socket m_ServerSocket;
	};
}