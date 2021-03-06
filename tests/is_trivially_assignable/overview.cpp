/// @copyright
/// Copyright (C) 2020 Assured Information Security, Inc.
///
/// @copyright
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// @copyright
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// @copyright
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#include <bsl/is_trivially_assignable.hpp>
#include <bsl/ut.hpp>

namespace
{
    class myclass final
    {};

    class myclass_copy_only final
    {
    public:
        constexpr myclass_copy_only() noexcept = default;
        ~myclass_copy_only() noexcept = default;
        constexpr myclass_copy_only(myclass_copy_only const &) noexcept = default;
        constexpr myclass_copy_only &operator=(myclass_copy_only const &) &noexcept = default;
        constexpr myclass_copy_only(myclass_copy_only &&) noexcept = delete;
        constexpr myclass_copy_only &operator=(myclass_copy_only &&) &noexcept = delete;
    };

    class myclass_move_only final
    {
    public:
        constexpr myclass_move_only() noexcept = default;
        ~myclass_move_only() noexcept = default;
        constexpr myclass_move_only(myclass_move_only const &) noexcept = delete;
        constexpr myclass_move_only &operator=(myclass_move_only const &) &noexcept = delete;
        constexpr myclass_move_only(myclass_move_only &&) noexcept = default;
        constexpr myclass_move_only &operator=(myclass_move_only &&) &noexcept = default;
    };

    class myclass_no_assign final
    {
    public:
        constexpr myclass_no_assign() noexcept = default;
        ~myclass_no_assign() noexcept = default;
        constexpr myclass_no_assign(myclass_no_assign const &) noexcept = delete;
        constexpr myclass_no_assign &operator=(myclass_no_assign const &) &noexcept = delete;
        constexpr myclass_no_assign(myclass_no_assign &&) noexcept = delete;
        constexpr myclass_no_assign &operator=(myclass_no_assign &&) &noexcept = delete;
    };

    class myclass_except final
    {
    public:
        constexpr myclass_except() noexcept(false) = default;
        ~myclass_except() noexcept(false) = default;
        constexpr myclass_except(myclass_except const &) noexcept(false) = default;
        constexpr myclass_except &operator=(myclass_except const &) &noexcept(false) = default;
        constexpr myclass_except(myclass_except &&) noexcept(false) = delete;
        constexpr myclass_except &operator=(myclass_except &&) &noexcept(false) = delete;
    };
}

/// <!-- description -->
///   @brief Main function for this unit test. If a call to ut_check() fails
///     the application will fast fail. If all calls to ut_check() pass, this
///     function will successfully return with bsl::exit_success.
///
/// <!-- inputs/outputs -->
///   @return Always returns bsl::exit_success.
///
bsl::exit_code
main() noexcept
{
    using namespace bsl;

    // NOTE: --
    //
    // This leverages a builtin so we do not exhaustively test this as we
    // assume the compiler already has the proper tests. These test are to
    // ensure we are using the right builtin
    //

    static_assert(!is_trivially_assignable<bool, bool>::value);
    static_assert(!is_trivially_assignable<bool, bool const>::value);
    static_assert(!is_trivially_assignable<bool, bool &>::value);
    static_assert(!is_trivially_assignable<bool, bool &&>::value);
    static_assert(!is_trivially_assignable<bool, bool const &>::value);
    static_assert(!is_trivially_assignable<bool, bool const &&>::value);
    static_assert(!is_trivially_assignable<bool, bool *>::value);
    static_assert(!is_trivially_assignable<bool, bool const *>::value);
    static_assert(!is_trivially_assignable<bool, bool *const>::value);
    static_assert(!is_trivially_assignable<bool, bool const *const>::value);

    static_assert(!is_trivially_assignable<bool const, bool>::value);
    static_assert(!is_trivially_assignable<bool const, bool const>::value);
    static_assert(!is_trivially_assignable<bool const, bool &>::value);
    static_assert(!is_trivially_assignable<bool const, bool &&>::value);
    static_assert(!is_trivially_assignable<bool const, bool const &>::value);
    static_assert(!is_trivially_assignable<bool const, bool const &&>::value);
    static_assert(!is_trivially_assignable<bool const, bool *>::value);
    static_assert(!is_trivially_assignable<bool const, bool const *>::value);
    static_assert(!is_trivially_assignable<bool const, bool *const>::value);
    static_assert(!is_trivially_assignable<bool const, bool const *const>::value);

    static_assert(is_trivially_assignable<bool &, bool>::value);
    static_assert(is_trivially_assignable<bool &, bool const>::value);
    static_assert(is_trivially_assignable<bool &, bool &>::value);
    static_assert(is_trivially_assignable<bool &, bool &&>::value);
    static_assert(is_trivially_assignable<bool &, bool const &>::value);
    static_assert(is_trivially_assignable<bool &, bool const &&>::value);
    static_assert(is_trivially_assignable<bool &, bool *>::value);
    static_assert(is_trivially_assignable<bool &, bool const *>::value);
    static_assert(is_trivially_assignable<bool &, bool *const>::value);
    static_assert(is_trivially_assignable<bool &, bool const *const>::value);

    static_assert(!is_trivially_assignable<bool &&, bool>::value);
    static_assert(!is_trivially_assignable<bool &&, bool const>::value);
    static_assert(!is_trivially_assignable<bool &&, bool &>::value);
    static_assert(!is_trivially_assignable<bool &&, bool &&>::value);
    static_assert(!is_trivially_assignable<bool &&, bool const &>::value);
    static_assert(!is_trivially_assignable<bool &&, bool const &&>::value);
    static_assert(!is_trivially_assignable<bool &&, bool *>::value);
    static_assert(!is_trivially_assignable<bool &&, bool const *>::value);
    static_assert(!is_trivially_assignable<bool &&, bool *const>::value);
    static_assert(!is_trivially_assignable<bool &&, bool const *const>::value);

    static_assert(!is_trivially_assignable<bool const &, bool>::value);
    static_assert(!is_trivially_assignable<bool const &, bool const>::value);
    static_assert(!is_trivially_assignable<bool const &, bool &>::value);
    static_assert(!is_trivially_assignable<bool const &, bool &&>::value);
    static_assert(!is_trivially_assignable<bool const &, bool const &>::value);
    static_assert(!is_trivially_assignable<bool const &, bool const &&>::value);
    static_assert(!is_trivially_assignable<bool const &, bool *>::value);
    static_assert(!is_trivially_assignable<bool const &, bool const *>::value);
    static_assert(!is_trivially_assignable<bool const &, bool *const>::value);
    static_assert(!is_trivially_assignable<bool const &, bool const *const>::value);

    static_assert(!is_trivially_assignable<bool const &&, bool>::value);
    static_assert(!is_trivially_assignable<bool const &&, bool const>::value);
    static_assert(!is_trivially_assignable<bool const &&, bool &>::value);
    static_assert(!is_trivially_assignable<bool const &&, bool &&>::value);
    static_assert(!is_trivially_assignable<bool const &&, bool const &>::value);
    static_assert(!is_trivially_assignable<bool const &&, bool const &&>::value);
    static_assert(!is_trivially_assignable<bool const &&, bool *>::value);
    static_assert(!is_trivially_assignable<bool const &&, bool const *>::value);
    static_assert(!is_trivially_assignable<bool const &&, bool *const>::value);
    static_assert(!is_trivially_assignable<bool const &&, bool const *const>::value);

    static_assert(!is_trivially_assignable<bool *, bool>::value);
    static_assert(!is_trivially_assignable<bool *, bool const>::value);
    static_assert(!is_trivially_assignable<bool *, bool &>::value);
    static_assert(!is_trivially_assignable<bool *, bool &&>::value);
    static_assert(!is_trivially_assignable<bool *, bool const &>::value);
    static_assert(!is_trivially_assignable<bool *, bool const &&>::value);
    static_assert(!is_trivially_assignable<bool *, bool *>::value);
    static_assert(!is_trivially_assignable<bool *, bool const *>::value);
    static_assert(!is_trivially_assignable<bool *, bool *const>::value);
    static_assert(!is_trivially_assignable<bool *, bool const *const>::value);

    static_assert(!is_trivially_assignable<bool const *, bool>::value);
    static_assert(!is_trivially_assignable<bool const *, bool const>::value);
    static_assert(!is_trivially_assignable<bool const *, bool &>::value);
    static_assert(!is_trivially_assignable<bool const *, bool &&>::value);
    static_assert(!is_trivially_assignable<bool const *, bool const &>::value);
    static_assert(!is_trivially_assignable<bool const *, bool const &&>::value);
    static_assert(!is_trivially_assignable<bool const *, bool *>::value);
    static_assert(!is_trivially_assignable<bool const *, bool const *>::value);
    static_assert(!is_trivially_assignable<bool const *, bool *const>::value);
    static_assert(!is_trivially_assignable<bool const *, bool const *const>::value);

    static_assert(!is_trivially_assignable<bool *const, bool>::value);
    static_assert(!is_trivially_assignable<bool *const, bool const>::value);
    static_assert(!is_trivially_assignable<bool *const, bool &>::value);
    static_assert(!is_trivially_assignable<bool *const, bool &&>::value);
    static_assert(!is_trivially_assignable<bool *const, bool const &>::value);
    static_assert(!is_trivially_assignable<bool *const, bool const &&>::value);
    static_assert(!is_trivially_assignable<bool *const, bool *>::value);
    static_assert(!is_trivially_assignable<bool *const, bool const *>::value);
    static_assert(!is_trivially_assignable<bool *const, bool *const>::value);
    static_assert(!is_trivially_assignable<bool *const, bool const *const>::value);

    static_assert(!is_trivially_assignable<bool const *const, bool>::value);
    static_assert(!is_trivially_assignable<bool const *const, bool const>::value);
    static_assert(!is_trivially_assignable<bool const *const, bool &>::value);
    static_assert(!is_trivially_assignable<bool const *const, bool &&>::value);
    static_assert(!is_trivially_assignable<bool const *const, bool const &>::value);
    static_assert(!is_trivially_assignable<bool const *const, bool const &&>::value);
    static_assert(!is_trivially_assignable<bool const *const, bool *>::value);
    static_assert(!is_trivially_assignable<bool const *const, bool const *>::value);
    static_assert(!is_trivially_assignable<bool const *const, bool *const>::value);
    static_assert(!is_trivially_assignable<bool const *const, bool const *const>::value);

    static_assert(is_trivially_assignable<myclass, myclass>::value);
    static_assert(is_trivially_assignable<myclass, myclass const>::value);
    static_assert(is_trivially_assignable<myclass, myclass &>::value);
    static_assert(is_trivially_assignable<myclass, myclass &&>::value);
    static_assert(is_trivially_assignable<myclass, myclass const &>::value);
    static_assert(is_trivially_assignable<myclass, myclass const &&>::value);
    static_assert(!is_trivially_assignable<myclass, myclass *>::value);
    static_assert(!is_trivially_assignable<myclass, myclass const *>::value);
    static_assert(!is_trivially_assignable<myclass, myclass *const>::value);
    static_assert(!is_trivially_assignable<myclass, myclass const *const>::value);

    static_assert(!is_trivially_assignable<myclass const, myclass>::value);
    static_assert(!is_trivially_assignable<myclass const, myclass const>::value);
    static_assert(!is_trivially_assignable<myclass const, myclass &>::value);
    static_assert(!is_trivially_assignable<myclass const, myclass &&>::value);
    static_assert(!is_trivially_assignable<myclass const, myclass const &>::value);
    static_assert(!is_trivially_assignable<myclass const, myclass const &&>::value);
    static_assert(!is_trivially_assignable<myclass const, myclass *>::value);
    static_assert(!is_trivially_assignable<myclass const, myclass const *>::value);
    static_assert(!is_trivially_assignable<myclass const, myclass *const>::value);
    static_assert(!is_trivially_assignable<myclass const, myclass const *const>::value);

    static_assert(is_trivially_assignable<myclass &, myclass>::value);
    static_assert(is_trivially_assignable<myclass &, myclass const>::value);
    static_assert(is_trivially_assignable<myclass &, myclass &>::value);
    static_assert(is_trivially_assignable<myclass &, myclass &&>::value);
    static_assert(is_trivially_assignable<myclass &, myclass const &>::value);
    static_assert(is_trivially_assignable<myclass &, myclass const &&>::value);
    static_assert(!is_trivially_assignable<myclass &, myclass *>::value);
    static_assert(!is_trivially_assignable<myclass &, myclass const *>::value);
    static_assert(!is_trivially_assignable<myclass &, myclass *const>::value);
    static_assert(!is_trivially_assignable<myclass &, myclass const *const>::value);

    static_assert(is_trivially_assignable<myclass &&, myclass>::value);
    static_assert(is_trivially_assignable<myclass &&, myclass const>::value);
    static_assert(is_trivially_assignable<myclass &&, myclass &>::value);
    static_assert(is_trivially_assignable<myclass &&, myclass &&>::value);
    static_assert(is_trivially_assignable<myclass &&, myclass const &>::value);
    static_assert(is_trivially_assignable<myclass &&, myclass const &&>::value);
    static_assert(!is_trivially_assignable<myclass &&, myclass *>::value);
    static_assert(!is_trivially_assignable<myclass &&, myclass const *>::value);
    static_assert(!is_trivially_assignable<myclass &&, myclass *const>::value);
    static_assert(!is_trivially_assignable<myclass &&, myclass const *const>::value);

    static_assert(!is_trivially_assignable<myclass const &, myclass>::value);
    static_assert(!is_trivially_assignable<myclass const &, myclass const>::value);
    static_assert(!is_trivially_assignable<myclass const &, myclass &>::value);
    static_assert(!is_trivially_assignable<myclass const &, myclass &&>::value);
    static_assert(!is_trivially_assignable<myclass const &, myclass const &>::value);
    static_assert(!is_trivially_assignable<myclass const &, myclass const &&>::value);
    static_assert(!is_trivially_assignable<myclass const &, myclass *>::value);
    static_assert(!is_trivially_assignable<myclass const &, myclass const *>::value);
    static_assert(!is_trivially_assignable<myclass const &, myclass *const>::value);
    static_assert(!is_trivially_assignable<myclass const &, myclass const *const>::value);

    static_assert(!is_trivially_assignable<myclass const &&, myclass>::value);
    static_assert(!is_trivially_assignable<myclass const &&, myclass const>::value);
    static_assert(!is_trivially_assignable<myclass const &&, myclass &>::value);
    static_assert(!is_trivially_assignable<myclass const &&, myclass &&>::value);
    static_assert(!is_trivially_assignable<myclass const &&, myclass const &>::value);
    static_assert(!is_trivially_assignable<myclass const &&, myclass const &&>::value);
    static_assert(!is_trivially_assignable<myclass const &&, myclass *>::value);
    static_assert(!is_trivially_assignable<myclass const &&, myclass const *>::value);
    static_assert(!is_trivially_assignable<myclass const &&, myclass *const>::value);
    static_assert(!is_trivially_assignable<myclass const &&, myclass const *const>::value);

    static_assert(!is_trivially_assignable<myclass *, myclass>::value);
    static_assert(!is_trivially_assignable<myclass *, myclass const>::value);
    static_assert(!is_trivially_assignable<myclass *, myclass &>::value);
    static_assert(!is_trivially_assignable<myclass *, myclass &&>::value);
    static_assert(!is_trivially_assignable<myclass *, myclass const &>::value);
    static_assert(!is_trivially_assignable<myclass *, myclass const &&>::value);
    static_assert(!is_trivially_assignable<myclass *, myclass *>::value);
    static_assert(!is_trivially_assignable<myclass *, myclass const *>::value);
    static_assert(!is_trivially_assignable<myclass *, myclass *const>::value);
    static_assert(!is_trivially_assignable<myclass *, myclass const *const>::value);

    static_assert(!is_trivially_assignable<myclass const *, myclass>::value);
    static_assert(!is_trivially_assignable<myclass const *, myclass const>::value);
    static_assert(!is_trivially_assignable<myclass const *, myclass &>::value);
    static_assert(!is_trivially_assignable<myclass const *, myclass &&>::value);
    static_assert(!is_trivially_assignable<myclass const *, myclass const &>::value);
    static_assert(!is_trivially_assignable<myclass const *, myclass const &&>::value);
    static_assert(!is_trivially_assignable<myclass const *, myclass *>::value);
    static_assert(!is_trivially_assignable<myclass const *, myclass const *>::value);
    static_assert(!is_trivially_assignable<myclass const *, myclass *const>::value);
    static_assert(!is_trivially_assignable<myclass const *, myclass const *const>::value);

    static_assert(!is_trivially_assignable<myclass *const, myclass>::value);
    static_assert(!is_trivially_assignable<myclass *const, myclass const>::value);
    static_assert(!is_trivially_assignable<myclass *const, myclass &>::value);
    static_assert(!is_trivially_assignable<myclass *const, myclass &&>::value);
    static_assert(!is_trivially_assignable<myclass *const, myclass const &>::value);
    static_assert(!is_trivially_assignable<myclass *const, myclass const &&>::value);
    static_assert(!is_trivially_assignable<myclass *const, myclass *>::value);
    static_assert(!is_trivially_assignable<myclass *const, myclass const *>::value);
    static_assert(!is_trivially_assignable<myclass *const, myclass *const>::value);
    static_assert(!is_trivially_assignable<myclass *const, myclass const *const>::value);

    static_assert(!is_trivially_assignable<myclass const *const, myclass>::value);
    static_assert(!is_trivially_assignable<myclass const *const, myclass const>::value);
    static_assert(!is_trivially_assignable<myclass const *const, myclass &>::value);
    static_assert(!is_trivially_assignable<myclass const *const, myclass &&>::value);
    static_assert(!is_trivially_assignable<myclass const *const, myclass const &>::value);
    static_assert(!is_trivially_assignable<myclass const *const, myclass const &&>::value);
    static_assert(!is_trivially_assignable<myclass const *const, myclass *>::value);
    static_assert(!is_trivially_assignable<myclass const *const, myclass const *>::value);
    static_assert(!is_trivially_assignable<myclass const *const, myclass *const>::value);
    static_assert(!is_trivially_assignable<myclass const *const, myclass const *const>::value);

    static_assert(!is_trivially_assignable<myclass_copy_only, myclass_copy_only>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only, myclass_copy_only const>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only, myclass_copy_only &>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only, myclass_copy_only &&>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only, myclass_copy_only const &>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only, myclass_copy_only const &&>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only, myclass_copy_only *>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only, myclass_copy_only const *>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only, myclass_copy_only *const>::value);
    static_assert(
        !is_trivially_assignable<myclass_copy_only, myclass_copy_only const *const>::value);

    static_assert(!is_trivially_assignable<myclass_copy_only &, myclass_copy_only>::value);
    static_assert(is_trivially_assignable<myclass_copy_only &, myclass_copy_only const>::value);
    static_assert(is_trivially_assignable<myclass_copy_only &, myclass_copy_only &>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &, myclass_copy_only &&>::value);
    static_assert(is_trivially_assignable<myclass_copy_only &, myclass_copy_only const &>::value);
    static_assert(is_trivially_assignable<myclass_copy_only &, myclass_copy_only const &&>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &, myclass_copy_only *>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &, myclass_copy_only const *>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &, myclass_copy_only *const>::value);
    static_assert(
        !is_trivially_assignable<myclass_copy_only &, myclass_copy_only const *const>::value);

    static_assert(!is_trivially_assignable<myclass_copy_only &&, myclass_copy_only>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &&, myclass_copy_only const>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &&, myclass_copy_only &>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &&, myclass_copy_only &&>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &&, myclass_copy_only const &>::value);
    static_assert(
        !is_trivially_assignable<myclass_copy_only &&, myclass_copy_only const &&>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &&, myclass_copy_only *>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &&, myclass_copy_only const *>::value);
    static_assert(!is_trivially_assignable<myclass_copy_only &&, myclass_copy_only *const>::value);
    static_assert(
        !is_trivially_assignable<myclass_copy_only &&, myclass_copy_only const *const>::value);

    static_assert(!is_trivially_assignable<myclass_move_only, myclass_move_only>::value);
    static_assert(!is_trivially_assignable<myclass_move_only, myclass_move_only const>::value);
    static_assert(!is_trivially_assignable<myclass_move_only, myclass_move_only &>::value);
    static_assert(!is_trivially_assignable<myclass_move_only, myclass_move_only &&>::value);
    static_assert(!is_trivially_assignable<myclass_move_only, myclass_move_only const &>::value);
    static_assert(!is_trivially_assignable<myclass_move_only, myclass_move_only const &&>::value);
    static_assert(!is_trivially_assignable<myclass_move_only, myclass_move_only *>::value);
    static_assert(!is_trivially_assignable<myclass_move_only, myclass_move_only const *>::value);
    static_assert(!is_trivially_assignable<myclass_move_only, myclass_move_only *const>::value);
    static_assert(
        !is_trivially_assignable<myclass_move_only, myclass_move_only const *const>::value);

    static_assert(is_trivially_assignable<myclass_move_only &, myclass_move_only>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &, myclass_move_only const>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &, myclass_move_only &>::value);
    static_assert(is_trivially_assignable<myclass_move_only &, myclass_move_only &&>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &, myclass_move_only const &>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &, myclass_move_only const &&>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &, myclass_move_only *>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &, myclass_move_only const *>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &, myclass_move_only *const>::value);
    static_assert(
        !is_trivially_assignable<myclass_move_only &, myclass_move_only const *const>::value);

    static_assert(!is_trivially_assignable<myclass_move_only &&, myclass_move_only>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &&, myclass_move_only const>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &&, myclass_move_only &>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &&, myclass_move_only &&>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &&, myclass_move_only const &>::value);
    static_assert(
        !is_trivially_assignable<myclass_move_only &&, myclass_move_only const &&>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &&, myclass_move_only *>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &&, myclass_move_only const *>::value);
    static_assert(!is_trivially_assignable<myclass_move_only &&, myclass_move_only *const>::value);
    static_assert(
        !is_trivially_assignable<myclass_move_only &&, myclass_move_only const *const>::value);

    static_assert(!is_trivially_assignable<myclass_no_assign, myclass_no_assign>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign, myclass_no_assign const>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign, myclass_no_assign &>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign, myclass_no_assign &&>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign, myclass_no_assign const &>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign, myclass_no_assign const &&>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign, myclass_no_assign *>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign, myclass_no_assign const *>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign, myclass_no_assign *const>::value);
    static_assert(
        !is_trivially_assignable<myclass_no_assign, myclass_no_assign const *const>::value);

    static_assert(!is_trivially_assignable<myclass_no_assign &, myclass_no_assign>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &, myclass_no_assign const>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &, myclass_no_assign &>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &, myclass_no_assign &&>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &, myclass_no_assign const &>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &, myclass_no_assign const &&>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &, myclass_no_assign *>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &, myclass_no_assign const *>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &, myclass_no_assign *const>::value);
    static_assert(
        !is_trivially_assignable<myclass_no_assign &, myclass_no_assign const *const>::value);

    static_assert(!is_trivially_assignable<myclass_no_assign &&, myclass_no_assign>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &&, myclass_no_assign const>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &&, myclass_no_assign &>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &&, myclass_no_assign &&>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &&, myclass_no_assign const &>::value);
    static_assert(
        !is_trivially_assignable<myclass_no_assign &&, myclass_no_assign const &&>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &&, myclass_no_assign *>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &&, myclass_no_assign const *>::value);
    static_assert(!is_trivially_assignable<myclass_no_assign &&, myclass_no_assign *const>::value);
    static_assert(
        !is_trivially_assignable<myclass_no_assign &&, myclass_no_assign const *const>::value);

    static_assert(!is_trivially_assignable<myclass_except, myclass_except>::value);
    static_assert(!is_trivially_assignable<myclass_except, myclass_except const>::value);
    static_assert(!is_trivially_assignable<myclass_except, myclass_except &>::value);
    static_assert(!is_trivially_assignable<myclass_except, myclass_except &&>::value);
    static_assert(!is_trivially_assignable<myclass_except, myclass_except const &>::value);
    static_assert(!is_trivially_assignable<myclass_except, myclass_except const &&>::value);
    static_assert(!is_trivially_assignable<myclass_except, myclass_except *>::value);
    static_assert(!is_trivially_assignable<myclass_except, myclass_except const *>::value);
    static_assert(!is_trivially_assignable<myclass_except, myclass_except *const>::value);
    static_assert(!is_trivially_assignable<myclass_except, myclass_except const *const>::value);

    static_assert(!is_trivially_assignable<myclass_except &, myclass_except>::value);
    static_assert(is_trivially_assignable<myclass_except &, myclass_except const>::value);
    static_assert(is_trivially_assignable<myclass_except &, myclass_except &>::value);
    static_assert(!is_trivially_assignable<myclass_except &, myclass_except &&>::value);
    static_assert(is_trivially_assignable<myclass_except &, myclass_except const &>::value);
    static_assert(is_trivially_assignable<myclass_except &, myclass_except const &&>::value);
    static_assert(!is_trivially_assignable<myclass_except &, myclass_except *>::value);
    static_assert(!is_trivially_assignable<myclass_except &, myclass_except const *>::value);
    static_assert(!is_trivially_assignable<myclass_except &, myclass_except *const>::value);
    static_assert(!is_trivially_assignable<myclass_except &, myclass_except const *const>::value);

    static_assert(!is_trivially_assignable<myclass_except &&, myclass_except>::value);
    static_assert(!is_trivially_assignable<myclass_except &&, myclass_except const>::value);
    static_assert(!is_trivially_assignable<myclass_except &&, myclass_except &>::value);
    static_assert(!is_trivially_assignable<myclass_except &&, myclass_except &&>::value);
    static_assert(!is_trivially_assignable<myclass_except &&, myclass_except const &>::value);
    static_assert(!is_trivially_assignable<myclass_except &&, myclass_except const &&>::value);
    static_assert(!is_trivially_assignable<myclass_except &&, myclass_except *>::value);
    static_assert(!is_trivially_assignable<myclass_except &&, myclass_except const *>::value);
    static_assert(!is_trivially_assignable<myclass_except &&, myclass_except *const>::value);
    static_assert(
        !is_trivially_assignable<myclass_except &&, myclass_no_assign const *const>::value);

    return bsl::ut_success();
}
