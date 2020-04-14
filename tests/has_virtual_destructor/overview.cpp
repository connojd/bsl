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

#include <bsl/has_virtual_destructor.hpp>
#include <bsl/ut.hpp>

namespace
{
    class myclass final
    {};

    class myclass_virtual    // NOLINT
    {
        virtual ~myclass_virtual() = default;
    };

    class myclass_subclass final : public myclass_virtual
    {};
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

    static_assert(has_virtual_destructor<myclass_virtual>::value);
    static_assert(has_virtual_destructor<myclass_subclass>::value);

    static_assert(!has_virtual_destructor<void>::value);
    static_assert(!has_virtual_destructor<bool>::value);
    static_assert(!has_virtual_destructor<bool &>::value);
    static_assert(!has_virtual_destructor<bool &&>::value);
    static_assert(!has_virtual_destructor<bool const>::value);
    static_assert(!has_virtual_destructor<bool const &>::value);
    static_assert(!has_virtual_destructor<bool const &&>::value);
    static_assert(!has_virtual_destructor<bool(bool)>::value);
    static_assert(!has_virtual_destructor<bool (*)(bool)>::value);
    static_assert(!has_virtual_destructor<myclass>::value);

    return bsl::ut_success();
}