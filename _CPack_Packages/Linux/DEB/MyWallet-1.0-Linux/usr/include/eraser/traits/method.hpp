#pragma once

#include "../method.hpp"

#include <type_traits>

namespace eraser::traits
{
    namespace impl
    {
        template <typename T>
        struct is_method : std::false_type
        {
        };

        template <auto Func, auto Name, typename Signature>
        struct is_method<method<Func, Name, Signature>> : std::true_type
        {
        };
    } // namespace impl

    template <typename T>
    concept method = impl::is_method<T>::value;
} // namespace eraser::traits
