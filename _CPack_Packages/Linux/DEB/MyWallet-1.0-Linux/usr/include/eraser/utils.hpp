#pragma once

#include <tuple>
#include <type_traits>

namespace eraser::utils
{
    namespace impl
    {
        template <typename T, typename... Ts>
        struct prepend_arguments;

        template <typename R, typename... Us, typename... Ts>
        struct prepend_arguments<R(Us...), Ts...>
        {
            using type = R(Ts..., Us...);
        };
    } // namespace impl

    template <auto I>
    using constant = std::integral_constant<decltype(I), I>;

    template <typename T>
    using identity = std::type_identity<T>;

    template <typename... Ts>
    using identities = std::type_identity<std::tuple<Ts...>>;

    template <typename T, typename... Ts>
    using prepend_arguments = impl::prepend_arguments<T, Ts...>::type;
} // namespace eraser::utils
