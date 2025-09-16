#pragma once

#include "unique.hpp"

namespace eraser::storage
{
    template <typename T>
    template <typename U, typename... Us>
    unique<T>::unique(std::in_place_type_t<U>, Us &&...args) : m_value{std::make_unique<U>(std::forward<Us>(args)...)}
    {
    }

    template <typename T>
    T *unique<T>::operator->() const
    {
        return m_value.get();
    }
} // namespace eraser::storage
