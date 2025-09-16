#pragma once

#include "../interface.hpp"

#include <type_traits>

namespace eraser::traits
{
    namespace impl
    {
        template <typename T>
        struct is_interface : std::false_type
        {
        };

        template <method... Methods>
        struct is_interface<interface<Methods...>> : std::true_type
        {
        };

    } // namespace impl

    template <typename T>
    concept interface = impl::is_interface<T>::value;
} // namespace eraser::traits
