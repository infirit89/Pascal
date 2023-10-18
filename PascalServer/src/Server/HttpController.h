#pragma once

#include "Core/Application.h"

#define PS_BEGIN_METHOD_LIST \
    static void InitMethods() \
    {

#define PS_REGISTER_METHOD(path, callback, allowedMethods) \
    Pascal::App().AddSimpleHttpResponseHandler(path, &callback, allowedMethods);

#define PS_REGISTER_METHOD_G(path, callback) \
    Pascal::App().AddSimpleHttpResponseHandler(path, &callback, Pascal::HttpMethod::Get);

#define PS_REGISTER_METHOD_P(path, callback) \
    Pascal::App().AddSimpleHttpResponseHandler(path, &callback, Pascal::HttpMethod::Post);

#define PS_REGISTER_METHOD_U(path, callback) \
    Pascal::App().AddSimpleHttpResponseHandler(path, &callback, Pascal::HttpMethod::Put);

#define PS_REGISTER_METHOD_D(path, callback) \
    Pascal::App().AddSimpleHttpResponseHandler(path, &callback, Pascal::HttpMethod::Delete);


#define PS_END_METHOD_LIST \
    return; \
    }

namespace Pascal 
{
    template<typename T>
    class HttpController
    {
    public:

        HttpController() = default;
        virtual ~HttpController() = default;

    private:
        class Registrator
        {
        public:
            Registrator() 
            {
                T::InitMethods();
            }
        };

        static Registrator s_Registrator;
        virtual void *touch()
        {
            return &s_Registrator;
        }
    };

    template<typename T>
    typename HttpController<T>::Registrator
        HttpController<T>::s_Registrator;
}
