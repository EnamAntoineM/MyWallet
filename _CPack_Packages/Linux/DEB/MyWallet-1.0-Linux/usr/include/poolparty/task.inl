#pragma once

#include "task.hpp"

namespace poolparty
{
    template <typename R, typename... Ts>
    template <typename... Us>
    packaged_task<R(Ts...)>::packaged_task(Us &&...args) : m_callback(std::forward<Us>(args)...)
    {
    }

    template <typename R, typename... Ts>
    std::future<R> packaged_task<R(Ts...)>::get_future()
    {
        return m_promise.get_future();
    }

    template <typename R, typename... Ts>
    template <typename... Us>
        requires std::is_void_v<R>
    void packaged_task<R(Ts...)>::operator()(Us &&...args)
    {
        std::invoke(m_callback, std::forward<Us>(args)...);
        m_promise.set_value();
    }

    template <typename R, typename... Ts>
    template <typename... Us>
        requires(not std::is_void_v<R>)
    void packaged_task<R(Ts...)>::operator()(Us &&...args)
    {
        m_promise.set_value(std::invoke(m_callback, std::forward<Us>(args)...));
    }
} // namespace poolparty
