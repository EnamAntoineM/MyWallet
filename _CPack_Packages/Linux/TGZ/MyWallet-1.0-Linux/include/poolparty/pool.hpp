#pragma once

#include <queue>
#include <cstddef>
#include <functional>

#include <concepts>
#include <type_traits>

#include <thread>
#include <stop_token>
#include <condition_variable>

namespace poolparty
{
    namespace impl
    {
        template <typename T>
        concept task_like = requires() {
            requires not std::copyable<T>;
            requires std::is_default_constructible_v<T>;
        };
    } // namespace impl

    template <typename T>
    struct traits;

    template <template <typename...> class Queue = std::queue, impl::task_like Task = std::move_only_function<void()>,
              typename... Ts>
    class pool
    {
        using queue  = Queue<Task, Ts...>;
        using traits = poolparty::traits<queue>;

      private:
        mutable std::mutex m_mutex;
        std::condition_variable m_cond;

      private:
        bool m_pause{false};

      private:
        queue m_queue;
        std::vector<std::jthread> m_threads;

      public:
        pool(std::size_t = std::thread::hardware_concurrency());

      public:
        ~pool();

      private:
        void worker(std::stop_token);

      public:
        void pause();
        void resume();

      public:
        template <typename... As>
        void emplace(As &&...);

      public:
        template <auto... TaskParams, typename Func, typename... As>
        [[nodiscard]] auto submit(Func &&, As &&...);

        template <auto... TaskParams, typename Func, typename... As>
        void forget(Func &&, As &&...);

      public:
        std::stop_source add_thread();

      public:
        void cleanup();

      public:
        [[nodiscard]] bool paused() const;

      public:
        [[nodiscard]] std::size_t size() const;
        [[nodiscard]] std::size_t tasks() const;
    };
} // namespace poolparty

#include "pool.inl"
