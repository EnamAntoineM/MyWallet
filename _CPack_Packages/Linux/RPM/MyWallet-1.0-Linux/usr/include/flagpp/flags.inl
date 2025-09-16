#pragma once

#include "flags.hpp"
#include <utility>

namespace flagpp::impl
{
    template <typename T>
        requires std::is_scoped_enum_v<T>
    struct wrapper
    {
        using enum_type       = T;
        using underlying_type = std::underlying_type_t<T>;

      private:
        T m_value;

      public:
        constexpr wrapper(T value) : m_value(value) {}

      private:
        constexpr wrapper(underlying_type value) : m_value(static_cast<enum_type>(value)) {}

      public:
        constexpr T value() const
        {
            return m_value;
        }

        constexpr operator T() const
        {
            return value();
        }

      public:
        constexpr auto underlying() const
        {
            return std::to_underlying(m_value);
        }

        constexpr operator underlying_type() const
        {
            return underlying();
        }

      public:
        constexpr wrapper operator&(wrapper other) const
        {
            return underlying() & other.underlying();
        }

        constexpr wrapper operator|(wrapper other) const
        {
            return underlying() | other.underlying();
        }

        constexpr wrapper operator^(wrapper other) const
        {
            return underlying() ^ other.underlying();
        }

        constexpr wrapper operator<<(wrapper other) const
        {
            return underlying() << other.underlying();
        }

        constexpr wrapper operator>>(wrapper other) const
        {
            return underlying() >> other.underlying();
        }

      public:
        constexpr wrapper operator~() const
        {
            return ~underlying();
        }

      public:
        constexpr wrapper &operator&=(wrapper other)
        {
            m_value = (*this) & other;
            return *this;
        }

        constexpr wrapper &operator|=(wrapper other)
        {
            m_value = (*this) | other;
            return *this;
        }

        constexpr wrapper &operator^=(wrapper other)
        {
            m_value = (*this) | other;
            return *this;
        }
    };
} // namespace flagpp::impl
