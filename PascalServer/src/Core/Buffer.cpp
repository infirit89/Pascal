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

    Buffer::Buffer(const std::string& data) 
        : m_Capacity(data.size() + 1), m_Size(data.size() + 1)
    {
        m_Data = new char[m_Capacity];

        for(uint32_t i = 0; i < data.size(); ++i)
            m_Data[i] = data[i];

        m_Data[data.size()] = '\0';
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
        uint32_t temp = m_Capacity;
        while(size + m_Size >= temp)
            temp *= 2;

        Reserve(temp);

        memcpy(GetWritable(), data, size);
        m_Size += size;
    }

    void Buffer::Write(const std::string& data) 
    {
        uint32_t temp = m_Capacity;
        while(data.size() + m_Size > temp)
            temp *= 2;

        Reserve(temp);

        memcpy(GetWritable(), data.c_str(), data.size());
        m_Size += data.size();
        m_Data[m_Size] = '\0';
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

    void Buffer::Advance(uint32_t offset) 
    {
        if(m_Head + offset > m_Size)
            return;

        m_Head += offset;
    }
}
