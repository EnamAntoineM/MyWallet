#pragma once

#include "traits/method.hpp"

#include <tuple>

namespace eraser
{
    template <traits::method... Methods>
    struct interface
    {
        using methods              = std::tuple<Methods...>;
        static constexpr auto size = sizeof...(Methods);

      private:
        template <auto Name, auto I = 0>
        static consteval auto find();

      public:
        template <auto Name>
        static constexpr auto index = find<Name>();

        template <auto Index>
        using at = std::tuple_element_t<Index, methods>;

        template <auto Name>
        using method = at<index<Name>>;
    };
} // namespace eraser

#include "interface.inl"
