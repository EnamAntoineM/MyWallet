#pragma once

#include "../erased.hpp"

namespace eraser::experimental
{
    template <typename Interface>
    struct deduce;

    template <typename Result = void, typename Func, typename Interface, typename... Ts>
    auto call(Func &&, const Interface *, Ts &&...);
} // namespace eraser::experimental

namespace eraser
{
    template <typename Interface, template <typename> typename Storage>
    class erased<experimental::deduce<Interface>, Storage>;
} // namespace eraser

#include "deduce.inl"
