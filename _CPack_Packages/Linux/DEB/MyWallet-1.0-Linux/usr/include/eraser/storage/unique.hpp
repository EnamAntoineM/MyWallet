#pragma once

#include <memory>

namespace eraser::storage
{
    template <typename T>
    class unique
    {
        std::unique_ptr<T> m_value;

      public:
        template <typename U, typename... Us>
        unique(std::in_place_type_t<U>, Us &&...);

      public:
        T *operator->() const;
    };
} // namespace eraser::storage

#include "unique.inl"
