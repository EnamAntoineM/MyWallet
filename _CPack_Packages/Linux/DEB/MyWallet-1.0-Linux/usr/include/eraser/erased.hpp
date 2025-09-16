#pragma once

#include "traits/interface.hpp"
#include "traits/utils.hpp"

#include "storage/unique.hpp"

#include <utility>
#include <cstddef>

#include <optional>

namespace eraser
{
    namespace impl
    {
        struct concept_;

        class base
        {
            friend struct concept_;

          private:
            void *m_value;
            std::size_t m_type;

          private:
            void *const *m_vtable;

          public:
            [[nodiscard]] void *value() const;
            [[nodiscard]] std::size_t type() const;
            [[nodiscard]] void *const *vtable() const;
        };
    } // namespace impl

    template <typename Interface, template <typename> typename Storage = storage::unique>
    class erased : impl::base
    {
        static_assert(traits::interface<Interface>);

      private:
        Storage<impl::concept_> m_value;

      public:
#ifndef __APPLE__
        template <traits::except<erased<Interface, Storage>> T>
        explicit erased(T &&);
#else
        template <typename T>
        explicit erased(T &&)
            requires traits::except<T, erased>;
#endif

        template <typename T, typename... Ts>
        explicit erased(std::in_place_type_t<T>, Ts &&...);

      public:
        template <auto Name, typename... Ts>
        auto invoke(Ts &&...) const;

      public:
        template <typename T>
        std::optional<T *> as() const;
    };

    template <typename Interface, typename T, template <typename> typename Storage = storage::unique, typename... Ts>
    auto make_erased(Ts &&...);
} // namespace eraser

#include "erased.inl"
