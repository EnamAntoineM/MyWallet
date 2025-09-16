#pragma once

#include "erased.hpp"
#include "utils.hpp"
#include "id.hpp"

#include <array>
#include <functional>

namespace eraser
{
    namespace impl
    {
        struct concept_
        {
            concept_(base &base, void *value, std::size_t type, void *const *vtable)
            {
                base.m_value  = value;
                base.m_type   = type;
                base.m_vtable = vtable;
            }

          public:
            virtual ~concept_() = default;
        };

        inline void *base::value() const
        {
            return m_value;
        }

        inline std::size_t base::type() const
        {
            return m_type;
        }

        inline void *const *base::vtable() const
        {
            return m_vtable;
        }

        template <typename Interface, typename T>
        auto make_vtable()
        {
            std::array<void *, Interface::size> rtn;

            static auto make = []<typename Method, typename R, typename... Ts>(utils::identities<Method, R(Ts...)>)
            {
                return +[](void *value, Ts... args) -> R
                {
                    return Method::func(*static_cast<T *>(value), std::forward<Ts>(args)...);
                };
            };

            static auto unpack = []<auto I>(utils::constant<I>)
            {
                using current   = Interface::template at<I>;
                using signature = current::signature;

                return reinterpret_cast<void *>(make(utils::identities<current, signature>{}));
            };

            [&]<auto... Is>(std::index_sequence<Is...>)
            {
                ((rtn[Is] = unpack(utils::constant<Is>{})), ...);
            }(std::make_index_sequence<Interface::size>());

            return rtn;
        }

        template <typename Interface, typename T>
        class model : public concept_
        {
            static const inline auto vtable = impl::make_vtable<Interface, T>();

          private:
            T m_value;

          public:
            template <typename... Us>
            model(base &base, Us &&...args)
                : concept_{base, std::addressof(m_value), id_of<T>(), vtable.data()}, m_value{std::forward<Us>(args)...}
            {
            }

          public:
            ~model() override = default;
        };

        template <typename Signature, typename... Ts>
        auto invoke(void *func, void *value, Ts &&...args)
        {
            using func_t = utils::prepend_arguments<Signature, void *> *;
            return std::invoke(reinterpret_cast<func_t>(func), value, std::forward<Ts>(args)...);
        }
    } // namespace impl

    template <typename Interface, template <typename> typename Storage>
#ifndef __APPLE__
    template <traits::except<erased<Interface, Storage>> T>
    erased<Interface, Storage>::erased(T &&value)
#else
    template <typename T>
    erased<Interface, Storage>::erased(T &&value)
        requires traits::except<T, erased>
#endif
        : m_value{std::in_place_type_t<impl::model<Interface, T>>{}, static_cast<impl::base &>(*this),
                  std::forward<T>(value)}
    {
    }

    template <typename Interface, template <typename> typename Storage>
    template <typename T, typename... Ts>
    erased<Interface, Storage>::erased(std::in_place_type_t<T>, Ts &&...args)
        : m_value{std::in_place_type_t<impl::model<Interface, T>>{}, static_cast<impl::base &>(*this),
                  std::forward<Ts>(args)...}
    {
    }

    template <typename Interface, template <typename> typename Storage>
    template <auto Name, typename... Ts>
    auto erased<Interface, Storage>::invoke(Ts &&...args) const
    {
        using method    = Interface::template method<Name>;
        using signature = method::signature;

        static_assert(std::invocable<signature, Ts...>);

        auto *const entry = vtable()[Interface::template index<Name>];
        return impl::invoke<signature>(entry, value(), std::forward<Ts>(args)...);
    }

    template <typename Interface, template <typename> typename Storage>
    template <typename T>
    std::optional<T *> erased<Interface, Storage>::as() const
    {
        if (type() != id_of<T>())
        {
            return std::nullopt;
        }

        return static_cast<T *>(value());
    }

    template <typename Interface, typename T, template <typename> typename Storage, typename... Ts>
    auto make_erased(Ts &&...args)
    {
        return erased<Interface, Storage>{std::in_place_type_t<T>{}, std::forward<Ts>(args)...};
    }
} // namespace eraser
