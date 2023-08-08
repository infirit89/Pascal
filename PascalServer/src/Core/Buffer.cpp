#include "pspch.h"

#include "Buffer.h"

#include <vector>

namespace Pascal 
{
    Buffer::Buffer(int capacity) 
        : m_Capacity(capacity)
    {
        m_Data = new char[m_Capacity];
        m_Size = 0;
    }

    Buffer::~Buffer() 
    {
        delete[] m_Data;
        m_Size = 0;
        m_Capacity = 0;
    }

    Buffer::Buffer(const Buffer& other) 
        : m_Size(other.m_Size), m_Capacity(other.m_Capacity)
    {
        if(m_Data)
            delete[] m_Data;

        m_Data = new char[m_Capacity];

        for (uint32_t i = 0; i < m_Size; i++)
            m_Data[i] = other.m_Data[i];
    }

    void Buffer::Write(char* data, uint32_t size) 
    {
        if(size + m_Size >= m_Capacity)
            Reserve(m_Size + size);

        memcpy(GetWritable(), data, size);
        m_Size += size;
    }

    std::string Buffer::Read(uint32_t size, uint32_t offset) 
    {
        if(size > m_Size)
            size = m_Size;

        std::string ret(m_Data + offset, size);
        return ret;
    }

    std::string Buffer::ReadAll() const
    {
        std::string ret(m_Data, m_Size);
        return ret;
    }

    void Buffer::Reserve(size_t capacity) 
    {
        if(capacity <= m_Capacity)
            return;

        m_Capacity = capacity;
        char* temp = (char*)realloc(m_Data, m_Capacity);

        PS_ASSERT(temp, "Couldn't realocate the buffer's data");
        if(temp)
            m_Data = temp;
    }
}
