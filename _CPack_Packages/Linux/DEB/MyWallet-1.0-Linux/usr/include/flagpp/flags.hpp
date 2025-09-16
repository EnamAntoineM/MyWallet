#pragma once

#include <concepts>
#include <type_traits>

namespace flagpp
{
    namespace impl
    {
        template <typename T>
            requires std::is_scoped_enum_v<T>
        struct wrapper;

        template <typename Enum, typename T>
        struct allowed_impl : std::false_type
        {
        };

        template <typename Enum>
        struct allowed_impl<Enum, Enum> : std::true_type
        {
        };

        template <typename Enum, typename T>
            requires std::same_as<Enum, typename T::enum_type>
        struct allowed_impl<Enum, T> : std::true_type
        {
        };

        template <typename Enum, typename O>
        concept allowed = requires() { requires allowed_impl<Enum, O>::value; };
    } // namespace impl

    template <typename T>
        requires std::is_scoped_enum_v<T>
    static constexpr bool enabled = false;
} // namespace flagpp

template <typename Enum, typename Other>
    requires flagpp::enabled<Enum> and flagpp::impl::allowed<Enum, Other>
constexpr auto operator&(Enum left, Other right)
{
    return flagpp::impl::wrapper{left} & right;
}

template <typename Enum, typename Other>
    requires flagpp::enabled<Enum> and flagpp::impl::allowed<Enum, Other>
constexpr auto operator|(Enum left, Other right)
{
    return flagpp::impl::wrapper{left} | right;
}

template <typename Enum, typename Other>
    requires flagpp::enabled<Enum> and flagpp::impl::allowed<Enum, Other>
constexpr auto operator^(Enum left, Other right)
{
    return flagpp::impl::wrapper{left} ^ right;
}

template <typename Enum, typename Other>
    requires flagpp::enabled<Enum> and flagpp::impl::allowed<Enum, Other>
constexpr auto operator<<(Enum left, Other right)
{
    return flagpp::impl::wrapper{left} << right;
}

template <typename Enum, typename Other>
    requires flagpp::enabled<Enum> and flagpp::impl::allowed<Enum, Other>
constexpr auto operator>>(Enum left, Other right)
{
    return flagpp::impl::wrapper{left} >> right;
}

template <typename Enum, typename Other>
    requires flagpp::enabled<Enum> and flagpp::impl::allowed<Enum, Other>
constexpr auto &operator&=(Enum &left, Other right)
{
    return left = flagpp::impl::wrapper{left} & right;
}

template <typename Enum, typename Other>
    requires flagpp::enabled<Enum> and flagpp::impl::allowed<Enum, Other>
constexpr auto &operator|=(Enum &left, Other right)
{
    return left = flagpp::impl::wrapper{left} | right;
}

template <typename Enum, typename Other>
    requires flagpp::enabled<Enum> and flagpp::impl::allowed<Enum, Other>
constexpr auto &operator^=(Enum &left, Other right)
{
    return left = flagpp::impl::wrapper{left} ^ right;
}

template <typename Enum>
    requires flagpp::enabled<Enum>
constexpr auto operator~(Enum value)
{
    return ~flagpp::impl::wrapper{value};
}

#include "flags.inl"
