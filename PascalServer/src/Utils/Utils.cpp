#include "pspch.h"
#include "Utils.h"

namespace Pascal 
{
    namespace Utils 
    {
        static unsigned char CharToHex(const char& ch) 
        {
            if (ch >= '0' && ch <= '9')
                return ch - '0';
            if (ch >= 'a' && ch <= 'f')
                return 10 + ch - 'a';
            if (ch >= 'A' && ch <= 'F')
                return 10 + ch - 'A';

		    return 0;

        }

        std::string DecodeURIComponent(const std::string& component) 
        {
            std::string decodedComponent;
            decodedComponent.reserve(component.size());
            for (size_t i = 0; i < component.size(); i++)
            {
                if(component[i] == '%') 
                {
                    char ch = CharToHex(component[++i]) << 4;
                    ch |= CharToHex(component[++i]);
                    decodedComponent += ch;
                }
                else 
                {
                    decodedComponent += component[i];
                }
            }

            return decodedComponent;
            
        }
    }
}
