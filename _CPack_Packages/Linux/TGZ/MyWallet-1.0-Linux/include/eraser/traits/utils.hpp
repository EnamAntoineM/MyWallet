#pragma once

#include <concepts>
#include <type_traits>

namespace eraser::traits
{
    template <typename T, typename U>
    concept except = not std::same_as<std::remove_cvref_t<T>, U>;
} // namespace eraser::traits
