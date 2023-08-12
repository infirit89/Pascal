#pragma once

#include <cstdint>
#include <string>

#include <spdlog/fmt/fmt.h>

namespace Pascal 
{
    // TODO: make it templated?
    class Buffer 
    {
    public:
        Buffer(int capacity = 1);
        Buffer(const std::string& data);

        ~Buffer();
        Buffer(const Buffer& other);

        char* GetWritable()
        {
            return m_Data + m_Size;
        }

        void Write(char* data, uint32_t size);

        template<typename... T>
        void WriteFormatted(fmt::format_string<T...> format, T&&... args) 
        {
            auto result = fmt::format_to_n(GetWritable(), GetCapacity() - GetSize(),
                format, std::forward<T>(args)...);

            if(result.size + GetSize() > GetCapacity()) 
            {
                uint32_t tempCapacity = GetCapacity();
                while(result.size + GetSize() > GetCapacity())
                    tempCapacity *= 2;
                
                Reserve(tempCapacity);
                result = fmt::format_to_n(GetWritable(), GetCapacity() - GetSize(),
                            format, std::forward<T>(args)...);
            }
            m_Size += result.size;

        }

        std::string Read(uint32_t size, uint32_t offset = 0);
        std::string ReadAll() const;

        const char* GetData() const { return m_Data; }

        void Reserve(size_t capacity);

        uint32_t GetSize() const { return m_Size; }
        uint32_t GetCapacity() const { return m_Capacity; }

    private:
        char* m_Data = nullptr;
        uint32_t m_Capacity = 0;
        uint32_t m_Size = 0;

        friend class Connection;
    };
}

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/bundled/format.h>

template <>
struct fmt::formatter<Pascal::Buffer> 
{
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) 
	{
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const Pascal::Buffer& buffer, FormatContext& ctx) -> decltype(ctx.out())
	{
		return format_to(ctx.out(), "{}", buffer.GetData());
	}
};

