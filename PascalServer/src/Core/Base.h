#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>

namespace Pascal 
{
	typedef uint32_t ps_socket;
	#define PS_INVALID_SOCKET (ps_socket)~0

	inline unsigned long long operator"" _kb(unsigned long long x) 
	{
		return x * 1024;
	}

	inline unsigned long long operator"" _mb(unsigned long long x) 
	{
		return x * (1024 * 1024);
	}

	#define PS_HTTP_REQUEST_INITIAL_SIZE 1_kb
	#define PS_HTTP_REQUEST_MAX_SIZE 8_mb

	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T, typename... Args>
	Unique<T> CreateUnique(Args... args) 
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T, typename... Args>
	Shared<T> CreateShared(Args... args) 
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	#define BIT(x) 1 << x

	#define DEFINE_ENUM_BITMASK_OPERATORS(EnumType) 						\
		inline constexpr EnumType operator&(EnumType lhs, EnumType rhs) 	\
		{ 																	\
			return static_cast<HttpMethod>( 								\
				static_cast<std::underlying_type_t<EnumType>>(lhs) & 		\
				static_cast<std::underlying_type_t<EnumType>>(rhs)); 		\
		} 																	\
		inline constexpr EnumType operator|(EnumType lhs, EnumType rhs) 	\
		{ 																	\
			return static_cast<HttpMethod>( 								\
				static_cast<std::underlying_type_t<EnumType>>(lhs) | 		\
				static_cast<std::underlying_type_t<EnumType>>(rhs)); 		\
		} 																	\
		inline constexpr EnumType operator^(EnumType lhs, EnumType rhs) 	\
		{ 																	\
			return static_cast<HttpMethod>( 								\
				static_cast<std::underlying_type_t<EnumType>>(lhs) ^ 		\
				static_cast<std::underlying_type_t<EnumType>>(rhs)); 		\
		} 																	\
		inline constexpr EnumType& operator^=(EnumType& lhs, EnumType rhs) 	\
		{ 																	\
			lhs = static_cast<HttpMethod>( 									\
				static_cast<std::underlying_type_t<EnumType>>(lhs) ^ 		\
				static_cast<std::underlying_type_t<EnumType>>(rhs)); 		\
			return lhs; 													\
		}
}
