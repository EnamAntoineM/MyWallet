#pragma once

namespace eraser
{
    template <auto Name, auto Func, typename Signature>
    struct method;

    template <auto Name, auto Func, typename R, typename... Ts>
    struct method<Name, Func, R(Ts...)>
    {
        static constexpr auto name = Name;
        static constexpr auto func = Func;

      public:
        using signature = R(Ts...);
    };
} // namespace eraser
