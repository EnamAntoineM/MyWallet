#pragma once

#include "id.hpp"

#include <atomic>

namespace eraser
{
    namespace impl
    {
        inline auto id_counter()
        {
            static std::atomic_size_t counter{};
            return ++counter;
        }
    } // namespace impl

    template <typename T>
    std::size_t id_of()
    {
        static const auto id = impl::id_counter();
        return id;
    }
} // namespace eraser
