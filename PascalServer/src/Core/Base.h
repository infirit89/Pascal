#pragma once

#include <cstdint>

typedef uint32_t ps_socket;
#define PS_INVALID_SOCKET (ps_socket)~0

inline uint64_t operator"" kb(uint64_t x) 
{
	return x * 1024;
}

inline uint64_t operator"" mb(uint64_t x) 
{
	return x * (1024 * 1024);
}

#define PS_HTTP_REQUEST_INITIAL_SIZE 1024
#define PS_HTTP_REQUEST_MAX_SIZE 8mb