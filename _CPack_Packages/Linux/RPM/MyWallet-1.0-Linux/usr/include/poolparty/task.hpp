#pragma once

#include <future>
#include <functional>

namespace poolparty
{
    namespace impl
    {
        template <typename T>
        struct function_guide_impl;

        template <typename R, typename... Ts>
        struct function_guide_impl<std::function<R(Ts...)>>
        {
            using type = R(Ts...);
        };

        template <typename T>
        using function_guide = function_guide_impl<decltype(std::function{std::declval<T>()})>::type;
    } // namespace impl

    //? Naive packaged_task implementation, required as MSVCs implementation is broken.

    template <typename T>
    class packaged_task;

    template <typename R, typename... Ts>
    class packaged_task<R(Ts...)>
    {
        std::promise<R> m_promise;
        std::move_only_function<R(Ts...)> m_callback;

      public:
        template <typename... Us>
        packaged_task(Us &&...args);

      public:
        std::future<R> get_future();

      public:
        template <typename... Us>
            requires std::is_void_v<R>
        void operator()(Us &&...args);

        template <typename... Us>
            requires(not std::is_void_v<R>)
        void operator()(Us &&...args);
    };

    template <typename T>
    packaged_task(T) -> packaged_task<impl::function_guide<T>>;
} // namespace poolparty

#include "task.inl"
