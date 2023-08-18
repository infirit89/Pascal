#pragma once 

#include <type_traits>
#include <tuple>

namespace Pascal 
{
    template<typename>
    struct FunctionTraits;


    template <typename ReturnType, typename... Arguments>
    struct FunctionTraits<ReturnType(*)(Arguments...)> {
        using ResultType = ReturnType;

        template <std::size_t Index>
        using Argument = typename std::tuple_element<
            Index,
            std::tuple<Arguments...>
        >::type;

        static constexpr std::size_t Arity = sizeof...(Arguments);
    };

}