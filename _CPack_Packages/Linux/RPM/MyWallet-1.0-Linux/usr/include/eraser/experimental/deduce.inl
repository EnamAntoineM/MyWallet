#pragma once

#include "deduce.hpp"
#include "../utils.hpp"

#include <utility>
#include <functional>

namespace eraser::experimental
{
    namespace impl
    {
        template <typename Interface, std::size_t I>
        struct storage
        {
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-template-friend"
#endif
            friend auto get(storage<Interface, I>);
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

            template <typename V>
            struct set
            {
                friend auto get(storage<Interface, I>)
                {
                    return std::type_identity<V>{};
                }
            };
        };

        template <typename Interface, auto I = 0, typename Tag = decltype([] {})>
        consteval auto size()
        {
            if constexpr (requires { get(storage<Interface, I>{}); })
            {
                return size<Interface, I + 1>();
            }
            else
            {
                return I;
            }
        }

        template <typename Interface, typename = decltype([] {})>
        consteval auto interface()
        {
            return []<auto... Is>(std::index_sequence<Is...>)
            {
                return utils::identity<eraser::interface<typename decltype(get(storage<Interface, Is>{}))::type...>>{};
            }(std::make_index_sequence<size<Interface>()>());
        }
    } // namespace impl

    template <typename Interface>
    struct deduce
    {
        using interface = decltype(impl::interface<Interface>())::type;
    };

    template <typename Result, typename Func, typename Interface, typename... Ts>
    auto call(Func &&visitor, const Interface *self, Ts &&...args)
    {
        static constexpr auto index = impl::size<Interface>();
        static constexpr auto func  = []<typename Self>(Self &&self, void *callback, Ts... args)
        {
            auto &func = *reinterpret_cast<Func *>(callback);
            return std::invoke(func, std::forward<Self>(self), std::forward<Ts>(args)...);
        };

        using signature = Result(void *, Ts...);
        using setter    = typename impl::storage<Interface, index>::template set<method<index, func, signature>>;

        std::ignore = setter{};

        const auto *base  = (const eraser::impl::base *)self; // NOLINT
        auto *const entry = base->vtable()[index];

        return eraser::impl::invoke<signature>(entry, base->value(), std::addressof(visitor),
                                               std::forward<Ts>(args)...);
    }
} // namespace eraser::experimental

namespace eraser
{
    template <typename Interface, template <typename> typename Storage>
    class erased<experimental::deduce<Interface>, Storage>
        : public Interface, public erased<typename experimental::deduce<Interface>::interface, Storage>
    {
        using erased<typename experimental::deduce<Interface>::interface, Storage>::invoke;

      public:
        using erased<typename experimental::deduce<Interface>::interface, Storage>::erased;
    };
} // namespace eraser
