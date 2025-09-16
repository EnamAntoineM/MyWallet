#pragma once

#include "shared.hpp"

namespace eraser::storage
{
    template <typename T>
    template <typename U, typename... Us>
    shared<T>::shared(std::in_place_type_t<U>, Us &&...args) : m_value{std::make_shared<U>(std::forward<Us>(args)...)}
    {
    }

    template <typename T>
    T *shared<T>::operator->() const
    {
        return m_value.get();
    }
} // namespace eraser::storage
