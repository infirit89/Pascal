#pragma once

#include <cstdint>

namespace Pascal 
{
    // TODO: make it templated?
    class Buffer 
    {
    public:
        Buffer(int capacity = 1);
        ~Buffer();
        Buffer(const Buffer& other);

        char* GetWritable()
        {
            return m_Data + m_Size;
        }

        void Write(char* data, uint32_t size);
        std::string Read(uint32_t size, uint32_t offset = 0);
        std::string ReadAll() const;

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
