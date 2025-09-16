#pragma once

#include <memory>

namespace eraser::storage
{
    template <typename T>
    class shared
    {
        std::shared_ptr<T> m_value;

      public:
        template <typename U, typename... Us>
        shared(std::in_place_type_t<U>, Us &&...);

      public:
        T *operator->() const;
    };
} // namespace eraser::storage

#include "shared.inl"
