#pragma once

#include "interface.hpp"

namespace eraser
{
    template <traits::method... Methods>
    template <auto Name, auto I>
    consteval auto interface<Methods...>::find()
    {
        if constexpr (I < size)
        {
            using current       = std::tuple_element_t<I, methods>;
            constexpr auto name = current::name;

            if constexpr (std::equality_comparable_with<decltype(Name), decltype(name)> && Name == name)
            {
                return I;
            }
            else
            {
                return find<Name, I + 1>();
            }
        }
    }
} // namespace eraser
