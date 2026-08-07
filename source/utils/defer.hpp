#pragma once

#include <concepts>
#include <functional>
#include <type_traits>

namespace utils
{
    class Defer
    {
    public:
        template <typename Func>
            requires(!std::same_as<std::remove_cvref_t<Func>, Defer> && std::is_nothrow_invocable_r_v<void, Func>)
        constexpr explicit Defer(Func&& func) : function_(std::forward<Func>(func))
        {
        }

        constexpr ~Defer() noexcept
        {
            if (function_)
            {
                function_();
            }
        }

        Defer(const Defer&)            = delete;
        Defer& operator=(const Defer&) = delete;

        Defer(Defer&&) noexcept            = default;
        Defer& operator=(Defer&&) noexcept = default;

    private:
        std::move_only_function<void() noexcept> function_;
    };
}  // namespace utils
