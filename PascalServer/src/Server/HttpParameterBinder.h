#pragma once

#include "Utils/FunctionTraits.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

#include <deque>

namespace Pascal 
{
    class HttpParameterBinderBase 
    {
    public:
        virtual Shared<HttpResponse> Handle(
                                std::deque<std::string>& params,
                                const Shared<HttpRequest>& request) = 0; 

        virtual ~HttpParameterBinderBase() { }
    };

    template<typename Function>
    class HttpParameterBinder : public HttpParameterBinderBase
    {
    public:
        using traits = FunctionTraits<Function>;

        HttpParameterBinder(Function&& function)
            : m_Function(std::forward<Function>(function))
        { }

        void PrintParamCount() 
        {
            PS_TRACE(param_count);
        }

        template<typename T>
        void CheckReturnType() 
        {
            PS_TRACE(std::is_same_v<T, typename traits::ResultType>);
        }

        template<typename T, size_t Index>
        void CheckParamType() 
        {
            // "unwrap" the type to its base
            using ValueType = std::remove_const_t<std::remove_reference_t<nth_arg<Index>>>;

            // example:
            // const int& -> int (we remove the reference and the constness)

            PS_TRACE(std::is_same_v<T, ValueType>);
        }

        virtual Shared<HttpResponse> Handle(
                                std::deque<std::string>& params,
                                const Shared<HttpRequest>& request) override
        {
            return Run(params, request);
        }

    private:
        static constexpr size_t param_count = traits::Arity;

        template<size_t Index>
        using nth_arg = typename traits::template Argument<Index>;

        Function m_Function;

        template<typename T>
        void GetValue(T&, std::string&& str) 
        {

        }

        void GetValue(int& value, std::string&& str) 
        {
            value = std::stoi(str);
        }

        void GetValue(double& value, std::string&& str) 
        {
            value = std::stod(str);
        }

        void GetValue(long double& value, std::string&& str) 
        {
            value = std::stold(str);
        }

        void GetValue(float& value, std::string&& str) 
        {
            value = std::stof(str);
        }

        void GetValue(std::string& value, std::string&& str) 
        {
            value = std::move(str);
        }

        void GetValue(long& value, std::string&& str) 
        {
            value = std::stol(str);
        }

        void GetValue(unsigned long& value, std::string&& str) 
        {
            value = std::stoul(str);
        }

        void GetValue(long long& value, std::string&& str) 
        {
            value = std::stoll(str);
        }

        void GetValue(unsigned long long& value, std::string&& str) 
        {
            value = std::stoull(str);
        }

        template<typename... Args>
        std::enable_if_t<sizeof...(Args) + 1 < param_count,
                        Shared<HttpResponse>> 
            Run(
                std::deque<std::string>& params,
                const Shared<HttpRequest>& request,
                Args&&... arguments) 
        {
            Shared<HttpResponse> response;
            using ValueType = 
                    std::remove_const_t<
                            std::remove_reference_t<
                                    nth_arg<sizeof...(Args) + 1>>>;

            ValueType value = ValueType();

            if(!params.empty()) 
            {
                std::string v = std::move(params.front());
                params.pop_front();

                if(!v.empty()) 
                {
                    try 
                    {
                        GetValue(value, std::move(v));
                        // PS_TRACE(value);
                    }
                    catch(const std::exception& e) 
                    {
                        PS_ERROR(e.what());
                        return nullptr;
                    }
                }
            }

            // PS_TRACE("run: arg count less than function arg count");
            response = Run(params, request, std::forward<Args>(arguments)..., std::move(value));
            return response;
        }

        template<typename... Args>
        std::enable_if_t<sizeof...(Args) + 1 == param_count,
                        Shared<HttpResponse>>
            Run(
                std::deque<std::string>& params,
                const Shared<HttpRequest>& request,
                Args&&... arguments) 
        {   
            // PS_TRACE("run: arg count equal to function arg count");
            return m_Function(request, std::move(arguments)...);
        }
    };
}
