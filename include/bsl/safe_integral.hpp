/// @copyright
/// Copyright (C) 2019 Assured Information Security, Inc.
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
///
/// @file safe_integral.hpp
///

#ifndef BSL_SAFE_INTEGRAL_HPP
#define BSL_SAFE_INTEGRAL_HPP

#include "cstdint.hpp"
#include "enable_if.hpp"
#include "is_constant_evaluated.hpp"
#include "is_integral.hpp"
#include "is_same.hpp"
#include "is_signed.hpp"
#include "is_unsigned.hpp"
#include "numeric_limits.hpp"

namespace bsl
{
    /// <!-- description -->
    ///   @brief Used to tell the user during compile-time that and
    ///     error has occurred during an add, sub or mul operation
    ///     from a bsl::safe_integral.
    ///
    /// <!-- inputs/outputs -->
    ///   @return Always returns true
    ///
    [[maybe_unused]] inline bool
    integral_overflow_underflow_wrap_error() noexcept
    {
        return true;
    }

    /// <!-- description -->
    ///   @brief Returns __builtin_add_overflow(x, y, res)
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the type of values to add
    ///   @param lhs the left hand side of the operation
    ///   @param rhs the right hand side of the operation
    ///   @param res the result of the operation
    ///   @return Returns __builtin_add_overflow(x, y, res)
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    builtin_add_overflow(T const lhs, T const rhs, T *const res) noexcept
    {
        if constexpr (BSL_PERFORCE) {
            *res = lhs + rhs;
            return true;
        }
        else {
            if (is_constant_evaluated()) {
                if (__builtin_add_overflow(lhs, rhs, res)) {    // NOLINT
                    integral_overflow_underflow_wrap_error();
                    return true;
                }

                return false;
            }

            return __builtin_add_overflow(lhs, rhs, res);    // NOLINT
        }
    }

    /// <!-- description -->
    ///   @brief Returns __builtin_sub_overflow(x, y, res)
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the type of values to subtract
    ///   @param lhs the left hand side of the operation
    ///   @param rhs the right hand side of the operation
    ///   @param res the result of the operation
    ///   @return Returns __builtin_sub_overflow(x, y, res)
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    builtin_sub_overflow(T const lhs, T const rhs, T *const res) noexcept
    {
        if constexpr (BSL_PERFORCE) {
            *res = lhs - rhs;
            return true;
        }
        else {
            if (is_constant_evaluated()) {
                if (__builtin_sub_overflow(lhs, rhs, res)) {    // NOLINT
                    integral_overflow_underflow_wrap_error();
                    return true;
                }

                return false;
            }

            return __builtin_sub_overflow(lhs, rhs, res);    // NOLINT
        }
    }

    /// <!-- description -->
    ///   @brief Returns __builtin_mul_overflow(x, y, res)
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the type of values to multiply
    ///   @param lhs the left hand side of the operation
    ///   @param rhs the right hand side of the operation
    ///   @param res the result of the operation
    ///   @return Returns __builtin_mul_overflow(x, y, res)
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    builtin_mul_overflow(T const lhs, T const rhs, T *const res) noexcept
    {
        if constexpr (BSL_PERFORCE) {
            *res = lhs * rhs;
            return true;
        }
        else {
            if (is_constant_evaluated()) {
                if (__builtin_mul_overflow(lhs, rhs, res)) {    // NOLINT
                    integral_overflow_underflow_wrap_error();
                    return true;
                }

                return false;
            }

            return __builtin_mul_overflow(lhs, rhs, res);    // NOLINT
        }
    }

    /// @class bsl::safe_integral
    ///
    /// <!-- description -->
    ///   @brief Provides a safe implementation of an integral type that
    ///     adheres to AUTOSAR's requirement that an integral shall not
    ///     overflow, wrap, divide by zero, etc.
    ///   @include example_safe_integral_overview.hpp
    ///
    /// <!-- template parameters -->
    ///   @tparam T the integral type to encapsulate.
    ///
    template<typename T>
    class safe_integral final    // NOLINT
    {
        static_assert(bsl::is_integral<T>::value, "only integral types are supported");

        /// @brief stores the value of the integral
        T m_val;
        /// @brief stores whether or not the integral has resulted in an error.
        bool m_error;

    public:
        /// @brief alias for: T
        using value_type = T;
        /// @brief alias for: T &
        using reference_type = T &;
        /// @brief alias for: T const
        using const_reference_type = T const;

        /// <!-- description -->
        ///   @brief Default constructor that creates a safe_integral with
        ///     get() == 0. Note that like other view types
        ///     in the BSL, the bsl::safe_integral is a POD type. This
        ///     means that when declaring a global, default constructed
        ///     bsl::safe_integral, DO NOT include the {} for
        ///     initialization. Instead, remove the {} and the global
        ///     bsl::safe_integral will be included in the BSS section of
        ///     the executable, and initialized to 0 for you. All other
        ///     instantiations of a bsl::safe_integral (or any POD
        ///     type), should be initialized using {} to ensure the POD is
        ///     properly initialized. Using the above method for global
        ///     initialization ensures that global constructors are not
        ///     executed at runtime, which is required by AUTOSAR.
        ///   @include safe_integral/example_safe_integral_default_constructor.hpp
        ///
        constexpr safe_integral() noexcept = default;

        /// <!-- description -->
        ///   @brief Creates a bsl::safe_integral given a BSL fixed width
        ///     type.
        ///   @include safe_integral/example_safe_integral_constructor_t.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param val the value to set the bsl::safe_integral to
        ///
        template<typename U, enable_if_t<is_same<T, U>::value, bool> = true>
        explicit constexpr safe_integral(U const val) noexcept    // --
            : m_val{val}, m_error{}
        {}

        /// <!-- description -->
        ///   @brief Creates a bsl::safe_integral given a BSL fixed width
        ///     type.
        ///   @include safe_integral/example_safe_integral_constructor_t_error.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param val the value to set the bsl::safe_integral to
        ///   @param err used to create a safe_integer that has already
        ///     resulted in an error.
        ///
        template<typename U, enable_if_t<is_same<T, U>::value, bool> = true>
        constexpr safe_integral(U const val, bool const err) noexcept    // --
            : m_val{val}, m_error{err}
        {}

        /// <!-- description -->
        ///   @brief Creates a bsl::safe_integral given a BSL fixed width
        ///     type. Note that this will clear the integral's error flag,
        ///     starting with a fresh value.
        ///   @include safe_integral/example_safe_integral_assignment_t.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param val the value to set the bsl::safe_integral to
        ///   @return Returns *this
        ///
        template<typename U, enable_if_t<is_same<T, U>::value, bool> = true>
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator=(U const val) &noexcept
        {
            m_val = val;
            m_error = false;

            return *this;
        }

        /// <!-- description -->
        ///   @brief Returns the value stored by the bsl::safe_integral. If an
        ///     error has occurred, this function will always return 0.
        ///   @include safe_integral/example_safe_integral_get.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns the value stored by the bsl::safe_integral. If an
        ///     error has occurred, this function will always return 0.
        ///
        [[nodiscard]] constexpr value_type
        get() const noexcept
        {
            if (m_error) {
                return static_cast<value_type>(0);
            }

            return m_val;
        }

        /// <!-- description -->
        ///   @brief Returns true if the safe_integral has never experienced
        ///     a wrap, overflow, underflow, divide by 0, etc.
        ///   @include safe_integral/example_safe_integral_operator_bool.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the safe_integral has never experienced
        ///     a wrap, overflow, underflow, divide by 0, etc.
        ///
        [[nodiscard]] constexpr explicit operator bool() const noexcept
        {
            return !m_error;
        }

        /// <!-- description -->
        ///   @brief Returns true if the safe_integral has experienced
        ///     a wrap, overflow, underflow, divide by 0, etc.
        ///   @include safe_integral/example_safe_integral_failure.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the safe_integral has experienced
        ///     a wrap, overflow, underflow, divide by 0, etc.
        ///
        [[nodiscard]] constexpr bool
        failure() const noexcept
        {
            return m_error;
        }

        /// <!-- description -->
        ///   @brief Used to indicate that the integral has experienced an
        ///     error.
        ///   @include safe_integral/example_safe_integral_set_failure.hpp
        ///
        constexpr void
        set_failure() noexcept
        {
            m_error = true;
        }

        /// <!-- description -->
        ///   @brief Returns the max value the bsl::safe_integral can store.
        ///   @include safe_integral/example_safe_integral_max.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns the max value the bsl::safe_integral can store.
        ///
        [[nodiscard]] static constexpr value_type
        max() noexcept
        {
            return numeric_limits<value_type>::max();
        }

        /// <!-- description -->
        ///   @brief Returns the max value between *this and other. If an
        ///     error has previously been encountered, this function returns
        ///     0 with an error.
        ///   @include safe_integral/example_safe_integral_max.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param other the other value to compare with *this
        ///   @return Returns the max value between *this and other. If an
        ///     error has previously been encountered, this function returns
        ///     0 with an error.
        ///
        [[nodiscard]] constexpr safe_integral<value_type>
        max(safe_integral<value_type> const &other) const noexcept
        {
            if (this->failure() || other.failure()) {
                return zero(true);
            }

            return safe_integral<value_type>{(m_val < other.m_val) ? other.m_val : m_val};
        }

        /// <!-- description -->
        ///   @brief Returns the max value between *this and other. If an
        ///     error has previously been encountered, this function returns
        ///     0 with an error.
        ///   @include safe_integral/example_safe_integral_max.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param other the other value to compare with *this
        ///   @return Returns the max value between *this and other. If an
        ///     error has previously been encountered, this function returns
        ///     0 with an error.
        ///
        template<typename U, enable_if_t<is_same<value_type, U>::value, bool> = true>
        [[nodiscard]] constexpr safe_integral<value_type>
        max(U const other) const noexcept
        {
            if (this->failure()) {
                return zero(true);
            }

            return safe_integral<value_type>{(m_val < other) ? other : m_val};
        }

        /// <!-- description -->
        ///   @brief Returns the min value the bsl::safe_integral can store.
        ///   @include safe_integral/example_safe_integral_min.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns the min value the bsl::safe_integral can store.
        ///
        [[nodiscard]] static constexpr value_type
        min() noexcept
        {
            return numeric_limits<value_type>::min();
        }

        /// <!-- description -->
        ///   @brief Returns the min value between *this and other. If an
        ///     error has previously been encountered, this function returns
        ///     0 with an error.
        ///   @include safe_integral/example_safe_integral_min.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param other the other value to compare with *this
        ///   @return Returns the min value between *this and other. If an
        ///     error has previously been encountered, this function returns
        ///     0 with an error.
        ///
        [[nodiscard]] constexpr safe_integral<value_type>
        min(safe_integral<value_type> const &other) const noexcept
        {
            if (this->failure() || other.failure()) {
                return zero(true);
            }

            return safe_integral<value_type>{(m_val < other.m_val) ? m_val : other.m_val};
        }

        /// <!-- description -->
        ///   @brief Returns the min value between *this and other. If an
        ///     error has previously been encountered, this function returns
        ///     0 with an error.
        ///   @include safe_integral/example_safe_integral_min.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param other the other value to compare with *this
        ///   @return Returns the min value between *this and other. If an
        ///     error has previously been encountered, this function returns
        ///     0 with an error.
        ///
        template<typename U, enable_if_t<is_same<value_type, U>::value, bool> = true>
        [[nodiscard]] constexpr safe_integral<value_type>
        min(U const other) const noexcept
        {
            if (this->failure()) {
                return zero(true);
            }

            return safe_integral<value_type>{(m_val < other) ? m_val : other};
        }

        /// <!-- description -->
        ///   @brief Returns 0
        ///   @include safe_integral/example_safe_integral_min.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param err used to indicate a failure()
        ///   @return Returns 0
        ///
        [[nodiscard]] static constexpr safe_integral<value_type>
        one(bool const err = false) noexcept
        {
            return safe_integral<value_type>{static_cast<value_type>(1), err};
        }

        /// <!-- description -->
        ///   @brief Returns 0
        ///   @include safe_integral/example_safe_integral_min.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param err used to indicate a failure()
        ///   @return Returns 0
        ///
        [[nodiscard]] static constexpr safe_integral<value_type>
        zero(bool const err = false) noexcept
        {
            return safe_integral<value_type>{static_cast<value_type>(0), err};
        }

        /// <!-- description -->
        ///   @brief Returns true if the safe_integral is signed
        ///   @include safe_integral/example_safe_integral_is_signed_type.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the safe_integral is signed
        ///
        [[nodiscard]] static constexpr bool
        is_signed_type() noexcept
        {
            return is_signed<value_type>::value;
        }

        /// <!-- description -->
        ///   @brief Returns true if the safe_integral is unsigned
        ///   @include safe_integral/example_safe_integral_is_unsigned_type.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the safe_integral is unsigned
        ///
        [[nodiscard]] static constexpr bool
        is_unsigned_type() noexcept
        {
            return is_unsigned<value_type>::value;
        }

        /// <!-- description -->
        ///   @brief Returns true if the safe_integral is positive. Will
        ///     always return false if an error has been encountered.
        ///   @include safe_integral/example_safe_integral_is_pos.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the safe_integral is positive
        ///
        [[nodiscard]] constexpr bool
        is_pos() const noexcept
        {
            return zero() < *this;
        }

        /// <!-- description -->
        ///   @brief Returns true if the safe_integral is negative. Will
        ///     always return false if an error has been encountered.
        ///   @include safe_integral/example_safe_integral_is_neg.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the safe_integral is negative
        ///
        [[nodiscard]] constexpr bool
        is_neg() const noexcept
        {
            if constexpr (is_unsigned_type()) {
                return false;
            }

            return zero() > *this;
        }

        /// <!-- description -->
        ///   @brief Returns true if the safe_integral is 0. Will
        ///     always return true if an error has been encountered.
        ///   @include safe_integral/example_safe_integral_is_zero.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the safe_integral is 0
        ///
        [[nodiscard]] constexpr bool
        is_zero() const noexcept
        {
            if (m_error) {
                return true;
            }

            return zero().get() == m_val;
        }

        /// <!-- description -->
        ///   @brief Returns true if the safe_integral equals the max value.
        ///     Will always return false if an error has been encountered.
        ///   @include safe_integral/example_safe_integral_is_max.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the safe_integral equals the max value
        ///
        [[nodiscard]] constexpr bool
        is_max() const noexcept
        {
            return max() == *this;
        }

        /// <!-- description -->
        ///   @brief Returns true if the safe_integral equals the min value.
        ///     Will always return false if an error has been encountered.
        ///   @include safe_integral/example_safe_integral_is_min.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the safe_integral equals the min value
        ///
        [[nodiscard]] constexpr bool
        is_min() const noexcept
        {
            return min() == *this;
        }

        /// <!-- description -->
        ///   @brief Returns *this += rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_assign_add.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param rhs the value to add to *this
        ///   @return Returns *this += rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator+=(safe_integral<value_type> const &rhs) &noexcept
        {
            bool const err{builtin_add_overflow(m_val, rhs.m_val, &m_val)};

            m_error = m_error || (err || rhs.failure());
            return *this;
        }

        /// <!-- description -->
        ///   @brief Returns *this += rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_assign_add.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @tparam U must be the same as T
        ///   @param rhs the value to add to *this
        ///   @return Returns *this += rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        template<typename U, enable_if_t<is_same<T, U>::value, bool> = true>
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator+=(U const rhs) &noexcept
        {
            bool const err{builtin_add_overflow(m_val, rhs, &m_val)};

            m_error = m_error || err;
            return *this;
        }

        /// <!-- description -->
        ///   @brief Returns *this -= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_assign_sub.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param rhs the value to sub from *this
        ///   @return Returns *this -= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator-=(safe_integral<value_type> const &rhs) &noexcept
        {
            bool const err{builtin_sub_overflow(m_val, rhs.m_val, &m_val)};

            m_error = m_error || (err || rhs.failure());
            return *this;
        }

        /// <!-- description -->
        ///   @brief Returns *this -= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_assign_sub.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @tparam U must be the same as T
        ///   @param rhs the value to sub from *this
        ///   @return Returns *this -= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        template<typename U, enable_if_t<is_same<T, U>::value, bool> = true>
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator-=(U const rhs) &noexcept
        {
            bool const err{builtin_sub_overflow(m_val, rhs, &m_val)};

            m_error = m_error || err;
            return *this;
        }

        /// <!-- description -->
        ///   @brief Returns *this *= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_assign_mul.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param rhs the value to multiply *this by
        ///   @return Returns *this *= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator*=(safe_integral<value_type> const &rhs) &noexcept
        {
            bool const err{builtin_mul_overflow(m_val, rhs.m_val, &m_val)};

            m_error = m_error || (err || rhs.failure());
            return *this;
        }

        /// <!-- description -->
        ///   @brief Returns *this *= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_assign_mul.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @tparam U must be the same as T
        ///   @param rhs the value to multiply *this by
        ///   @return Returns *this *= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        template<typename U, enable_if_t<is_same<T, U>::value, bool> = true>
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator*=(U const rhs) &noexcept
        {
            bool const err{builtin_mul_overflow(m_val, rhs, &m_val)};

            m_error = m_error || err;
            return *this;
        }

        /// <!-- description -->
        ///   @brief Returns *this /= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_assign_div.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param rhs the value to divide *this by
        ///   @return Returns *this /= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator/=(safe_integral<value_type> const &rhs) &noexcept
        {
            if (this->failure() || rhs.failure()) {
                m_error = true;
                return *this;
            }

            if (zero() == rhs) {
                m_error = integral_overflow_underflow_wrap_error();
                return *this;
            }

            if constexpr (is_signed_type()) {
                if ((min() == m_val) && (-one() == rhs)) {
                    m_error = integral_overflow_underflow_wrap_error();
                    return *this;
                }
            }

            m_val /= rhs.m_val;
            return *this;
        }

        /// <!-- description -->
        ///   @brief Returns *this /= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_assign_div.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @tparam U must be the same as T
        ///   @param rhs the value to divide *this by
        ///   @return Returns *this /= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        template<typename U, enable_if_t<is_same<T, U>::value, bool> = true>
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator/=(U const rhs) &noexcept
        {
            return *this /= safe_integral<value_type>{rhs};
        }

        /// <!-- description -->
        ///   @brief Returns *this %= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_assign_mod.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param rhs the value to modulo *this by
        ///   @return Returns *this %= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator%=(safe_integral<value_type> const &rhs) &noexcept
        {
            if (this->failure() || rhs.failure()) {
                m_error = true;
                return *this;
            }

            if (zero() == rhs) {
                m_error = integral_overflow_underflow_wrap_error();
                return *this;
            }

            if constexpr (is_signed_type()) {
                if ((min() == m_val) && (-one() == rhs.m_val)) {
                    m_error = integral_overflow_underflow_wrap_error();
                    return *this;
                }
            }

            m_val %= rhs.m_val;
            return *this;
        }

        /// <!-- description -->
        ///   @brief Returns *this %= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_assign_mod.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @tparam U must be the same as T
        ///   @param rhs the value to modulo *this by
        ///   @return Returns *this %= rhs. If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        template<typename U, enable_if_t<is_same<T, U>::value, bool> = true>
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator%=(U const rhs) &noexcept
        {
            return *this %= safe_integral<value_type>{rhs};
        }

        /// <!-- description -->
        ///   @brief Returns ++(*this). If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_inc.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns ++(*this). If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator++() noexcept
        {
            bool const err{builtin_add_overflow(m_val, one().get(), &m_val)};

            m_error = m_error || err;
            return *this;
        }

        /// <!-- description -->
        ///   @brief Returns --(*this). If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///   @include safe_integral/example_safe_integral_dec.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns --(*this). If this operation results in
        ///     an error (e.g., overflow, wrapping, etc.), the result of
        ///     this operation is undefined, and get() will always return 0.
        ///
        [[maybe_unused]] constexpr safe_integral<value_type> &
        operator--() noexcept
        {
            bool const err{builtin_sub_overflow(m_val, one().get(), &m_val)};

            m_error = m_error || err;
            return *this;
        }
    };

    // -------------------------------------------------------------------------
    // safe_integral rational operators
    // -------------------------------------------------------------------------

    /// <!-- description -->
    ///   @brief Returns lhs.get() == rhs.get(). Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() == rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator==(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        return (lhs.get() == rhs.get()) && (!lhs.failure()) && (!rhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() == rhs. Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() == rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator==(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return (lhs.get() == rhs) && (!lhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs == rhs.get(). Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs == rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator==(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return (lhs == rhs.get()) && (!rhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() != rhs.get(). Will always return true,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_not_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() != rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator!=(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        return !(lhs == rhs);
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() != rhs. Will always return true,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_not_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() != rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator!=(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return !(lhs == rhs);
    }

    /// <!-- description -->
    ///   @brief Returns lhs != rhs.get(). Will always return true,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_not_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs != rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator!=(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return !(lhs == rhs);
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() < rhs.get(). Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_lt.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() < rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator<(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        return (lhs.get() < rhs.get()) && (!lhs.failure()) && (!rhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() < rhs. Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_lt.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() < rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator<(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return (lhs.get() < rhs) && (!lhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs < rhs.get(). Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_lt.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs < rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator<(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return (lhs < rhs.get()) && (!rhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() <= rhs.get().Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_lt_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() <= rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator<=(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        return (lhs.get() <= rhs.get()) && (!lhs.failure()) && (!rhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() <= rhs. Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_lt_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() <= rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator<=(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return (lhs.get() <= rhs) && (!lhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs <= rhs.get(). Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_lt_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs <= rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator<=(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return (lhs <= rhs.get()) && (!rhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() > rhs.get(). Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_gt.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() > rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator>(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        return (lhs.get() > rhs.get()) && (!lhs.failure()) && (!rhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() > rhs. Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_gt.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() > rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator>(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return (lhs.get() > rhs) && (!lhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs > rhs.get(). Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_gt.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs > rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator>(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return (lhs > rhs.get()) && (!rhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() >= rhs.get(). Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_gt_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() >= rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator>=(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        return (lhs.get() >= rhs.get()) && (!lhs.failure()) && (!rhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs.get() >= rhs. Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_gt_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs.get() >= rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator>=(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return (lhs.get() >= rhs) && (!lhs.failure());
    }

    /// <!-- description -->
    ///   @brief Returns lhs >= rhs.get(). Will always return false,
    ///     even when comparing to 0 if the safe_integral parameters have
    ///     encountered an error.
    ///   @include safe_integral/example_safe_integral_gt_equals.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs >= rhs.get()
    ///
    template<typename T>
    [[nodiscard]] constexpr bool
    operator>=(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return (lhs >= rhs.get()) && (!rhs.failure());
    }

    // -------------------------------------------------------------------------
    // safe_integral arithmetic operators
    // -------------------------------------------------------------------------

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} += rhs
    ///   @include safe_integral/example_safe_integral_add.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} += rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator+(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        safe_integral<T> tmp{lhs};
        return tmp += rhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs + safe_integral<T>{rhs}
    ///   @include safe_integral/example_safe_integral_add.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs + safe_integral<T>{rhs}
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator+(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return lhs + safe_integral<T>{rhs};
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} + rhs
    ///   @include safe_integral/example_safe_integral_add.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} + rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator+(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return safe_integral<T>{lhs} + rhs;
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} -= rhs
    ///   @include safe_integral/example_safe_integral_sub.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} -= rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator-(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        safe_integral<T> tmp{lhs};
        return tmp -= rhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs - safe_integral<T>{rhs}
    ///   @include safe_integral/example_safe_integral_sub.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs - safe_integral<T>{rhs}
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator-(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return lhs - safe_integral<T>{rhs};
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} - rhs
    ///   @include safe_integral/example_safe_integral_sub.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} - rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator-(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return safe_integral<T>{lhs} - rhs;
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} *= rhs
    ///   @include safe_integral/example_safe_integral_mul.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} *= rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator*(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        safe_integral<T> tmp{lhs};
        return tmp *= rhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs * safe_integral<T>{rhs}
    ///   @include safe_integral/example_safe_integral_mul.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs * safe_integral<T>{rhs}
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator*(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return lhs * safe_integral<T>{rhs};
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} * rhs
    ///   @include safe_integral/example_safe_integral_mul.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} * rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator*(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return safe_integral<T>{lhs} * rhs;
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} /= rhs
    ///   @include safe_integral/example_safe_integral_div.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} /= rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator/(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        safe_integral<T> tmp{lhs};
        return tmp /= rhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs / safe_integral<T>{rhs}
    ///   @include safe_integral/example_safe_integral_div.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs / safe_integral<T>{rhs}
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator/(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return lhs / safe_integral<T>{rhs};
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} / rhs
    ///   @include safe_integral/example_safe_integral_div.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} / rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator/(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return safe_integral<T>{lhs} / rhs;
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} %= rhs
    ///   @include safe_integral/example_safe_integral_mod.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} %= rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator%(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        safe_integral<T> tmp{lhs};
        return tmp %= rhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs % safe_integral<T>{rhs}
    ///   @include safe_integral/example_safe_integral_mod.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs % safe_integral<T>{rhs}
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator%(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return lhs % safe_integral<T>{rhs};
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} % rhs
    ///   @include safe_integral/example_safe_integral_mod.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} % rhs
    ///
    template<typename T>
    [[nodiscard]] constexpr safe_integral<T>
    operator%(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return safe_integral<T>{lhs} % rhs;
    }

    // -------------------------------------------------------------------------
    // shirt operators
    // -------------------------------------------------------------------------

    /// <!-- description -->
    ///   @brief Returns lhs = (lhs.get() << bits)
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_assign_lshift.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param bits the number of bits to shift the integral by
    ///   @return Returns lhs = (lhs.get() << bits)
    ///
    template<
        typename T,
        typename U,
        enable_if_t<is_unsigned<T>::value, bool> = true,
        enable_if_t<is_unsigned<U>::value, bool> = true>
    [[maybe_unused]] constexpr safe_integral<T> &
    operator<<=(safe_integral<T> &lhs, U const bits) noexcept
    {
        T tmp{lhs.get()};
        tmp <<= bits;

        lhs = safe_integral<T>{tmp, lhs.failure()};
        return lhs;
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{(lhs.get() << bits)}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_lshift.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param bits the number of bits to shift the integral by
    ///   @return Returns safe_integral<T>{(lhs.get() << bits)}
    ///
    template<
        typename T,
        typename U,
        enable_if_t<is_unsigned<T>::value, bool> = true,
        enable_if_t<is_unsigned<U>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator<<(safe_integral<T> const &lhs, U const bits) noexcept
    {
        T tmp{lhs.get()};
        tmp <<= bits;

        return safe_integral<T>{tmp, lhs.failure()};
    }

    /// <!-- description -->
    ///   @brief Returns lhs = (lhs.get() >> bits)
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_assign_rshift.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param bits the number of bits to shift the integral by
    ///   @return Returns lhs = (lhs.get() >> bits)
    ///
    template<
        typename T,
        typename U,
        enable_if_t<is_unsigned<T>::value, bool> = true,
        enable_if_t<is_unsigned<U>::value, bool> = true>
    [[maybe_unused]] constexpr safe_integral<T> &
    operator>>=(safe_integral<T> &lhs, U const bits) noexcept
    {
        T tmp{lhs.get()};
        tmp >>= bits;

        lhs = safe_integral<T>{tmp, lhs.failure()};
        return lhs;
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{(lhs.get() >> bits)}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_rshift.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param bits the number of bits to shift the integral by
    ///   @return Returns safe_integral<T>{(lhs.get() >> bits)}
    ///
    template<
        typename T,
        typename U,
        enable_if_t<is_unsigned<T>::value, bool> = true,
        enable_if_t<is_unsigned<U>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator>>(safe_integral<T> const &lhs, U const bits) noexcept
    {
        T tmp{lhs.get()};
        tmp >>= bits;

        return safe_integral<T>{tmp, lhs.failure()};
    }

    // -------------------------------------------------------------------------
    // bitwise operators
    // -------------------------------------------------------------------------

    /// <!-- description -->
    ///   @brief Returns lhs = safe_integral<T>{lhs.get() & rhs.get()}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_assign_and.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs = safe_integral<T>{lhs.get() & rhs.get()}
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[maybe_unused]] constexpr safe_integral<T> &
    operator&=(safe_integral<T> &lhs, safe_integral<T> const &rhs) noexcept
    {
        T tmp{lhs.get()};
        tmp &= rhs.get();

        lhs = safe_integral<T>{tmp, lhs.failure() || rhs.failure()};
        return lhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs = safe_integral<T>{lhs.get() & rhs.get()}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_assign_and.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs = safe_integral<T>{lhs.get() & rhs.get()}
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[maybe_unused]] constexpr safe_integral<T> &
    operator&=(safe_integral<T> &lhs, T const rhs) noexcept
    {
        T tmp{lhs.get()};
        tmp &= rhs;

        lhs = safe_integral<T>{tmp, lhs.failure()};
        return lhs;
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} &= rhs
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_and.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} &= rhs
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator&(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        safe_integral<T> tmp{lhs};
        return tmp &= rhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs & safe_integral<T>{rhs}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_and.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs & safe_integral<T>{rhs}
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator&(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return lhs & safe_integral<T>{rhs};
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} & rhs
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_and.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} & rhs
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator&(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return safe_integral<T>{lhs} & rhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs = safe_integral<T>{lhs.get() | rhs.get()}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_assign_or.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs = safe_integral<T>{lhs.get() | rhs.get()}
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[maybe_unused]] constexpr safe_integral<T> &
    operator|=(safe_integral<T> &lhs, safe_integral<T> const &rhs) noexcept
    {
        T tmp{lhs.get()};
        tmp |= rhs.get();

        lhs = safe_integral<T>{tmp, lhs.failure() || rhs.failure()};
        return lhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs = safe_integral<T>{lhs.get() | rhs.get()}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_assign_or.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs = safe_integral<T>{lhs.get() | rhs.get()}
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[maybe_unused]] constexpr safe_integral<T> &
    operator|=(safe_integral<T> &lhs, T const rhs) noexcept
    {
        T tmp{lhs.get()};
        tmp |= rhs;

        lhs = safe_integral<T>{tmp, lhs.failure()};
        return lhs;
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} |= rhs
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_or.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs = safe_integral<T>{lhs} |= rhs
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator|(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        safe_integral<T> tmp{lhs};
        return tmp |= rhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs | safe_integral<T>{rhs}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_or.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs = lhs | safe_integral<T>{rhs}
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator|(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return lhs | safe_integral<T>{rhs};
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} | rhs
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_or.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs = safe_integral<T>{lhs} | rhs
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator|(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return safe_integral<T>{lhs} | rhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs = safe_integral<T>{lhs.get() ^ rhs.get()}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_assign_xor.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs = safe_integral<T>{lhs.get() ^ rhs.get()}
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[maybe_unused]] constexpr safe_integral<T> &
    operator^=(safe_integral<T> &lhs, safe_integral<T> const &rhs) noexcept
    {
        T tmp{lhs.get()};
        tmp ^= rhs.get();

        lhs = safe_integral<T>{tmp, lhs.failure() || rhs.failure()};
        return lhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs = safe_integral<T>{lhs.get() ^ rhs.get()}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_assign_xor.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs = safe_integral<T>{lhs.get() ^ rhs.get()}
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[maybe_unused]] constexpr safe_integral<T> &
    operator^=(safe_integral<T> &lhs, T const rhs) noexcept
    {
        T tmp{lhs.get()};
        tmp ^= rhs;

        lhs = safe_integral<T>{tmp, lhs.failure()};
        return lhs;
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} ^= rhs
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_xor.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} ^= rhs
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator^(safe_integral<T> const &lhs, safe_integral<T> const &rhs) noexcept
    {
        safe_integral<T> tmp{lhs};
        return tmp ^= rhs;
    }

    /// <!-- description -->
    ///   @brief Returns lhs ^ safe_integral<T>{rhs}
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_xor.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns lhs ^ safe_integral<T>{rhs}
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator^(safe_integral<T> const &lhs, T const rhs) noexcept
    {
        return lhs ^ safe_integral<T>{rhs};
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>{lhs} ^ rhs
    ///     Only unsigned types are supported due to AUTOSAR.
    ///   @include safe_integral/example_safe_integral_xor.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param lhs the left hand side of the operator
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>{lhs} ^ rhs
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator^(T const lhs, safe_integral<T> const &rhs) noexcept
    {
        return safe_integral<T>{lhs} ^ rhs;
    }

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>::max() ^ rhs. Only unsigned types
    ///     are supported.
    ///   @include safe_integral/example_safe_integral_complement.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>::max() ^ rhs. Only unsigned types
    ///     are supported.
    ///
    template<typename T, enable_if_t<is_unsigned<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator~(safe_integral<T> const &rhs) noexcept
    {
        return safe_integral<T>::max() ^ rhs;
    }

    // -------------------------------------------------------------------------
    // unary operators
    // -------------------------------------------------------------------------

    /// <!-- description -->
    ///   @brief Returns safe_integral<T>::zero() - rhs; Only Signed types
    ///     are supported, and if the value equals the types minimum value
    ///     this operation results in an error due to overflow.
    ///   @include safe_integral/example_safe_integral_unary.hpp
    ///   @related bsl::safe_integral
    ///
    /// <!-- inputs/outputs -->
    ///   @tparam T the integral type to encapsulate.
    ///   @param rhs the right hand side of the operator
    ///   @return Returns safe_integral<T>::zero() - rhs; Only Signed types
    ///     are supported, and if the value equals the types minimum value
    ///     this operation results in an error due to overflow.
    ///
    template<typename T, enable_if_t<is_signed<T>::value, bool> = true>
    [[nodiscard]] constexpr safe_integral<T>
    operator-(safe_integral<T> const &rhs) noexcept
    {
        return safe_integral<T>::zero() - rhs;
    }

    // -------------------------------------------------------------------------
    // supported safe_integral types
    // -------------------------------------------------------------------------

    /// @brief provides the bsl::safe_integral version of bsl::int8
    using safe_int8 = safe_integral<bsl::int8>;
    /// @brief provides the bsl::safe_integral version of bsl::int16
    using safe_int16 = safe_integral<bsl::int16>;
    /// @brief provides the bsl::safe_integral version of bsl::int32
    using safe_int32 = safe_integral<bsl::int32>;
    /// @brief provides the bsl::safe_integral version of bsl::int64
    using safe_int64 = safe_integral<bsl::int64>;
    /// @brief provides the bsl::safe_integral version of bsl::int_fast8
    using safe_int_fast8 = safe_integral<bsl::int_fast8>;
    /// @brief provides the bsl::safe_integral version of bsl::int_fast16
    using safe_int_fast16 = safe_integral<bsl::int_fast16>;
    /// @brief provides the bsl::safe_integral version of bsl::int_fast32
    using safe_int_fast32 = safe_integral<bsl::int_fast32>;
    /// @brief provides the bsl::safe_integral version of bsl::int_fast64
    using safe_int_fast64 = safe_integral<bsl::int_fast64>;
    /// @brief provides the bsl::safe_integral version of bsl::int_least8
    using safe_int_least8 = safe_integral<bsl::int_least8>;
    /// @brief provides the bsl::safe_integral version of bsl::int_least16
    using safe_int_least16 = safe_integral<bsl::int_least16>;
    /// @brief provides the bsl::safe_integral version of bsl::int_least32
    using safe_int_least32 = safe_integral<bsl::int_least32>;
    /// @brief provides the bsl::safe_integral version of bsl::int_least64
    using safe_int_least64 = safe_integral<bsl::int_least64>;
    /// @brief provides the bsl::safe_integral version of bsl::intmax
    using safe_intmax = safe_integral<bsl::intmax>;
    /// @brief provides the bsl::safe_integral version of bsl::intptr
    using safe_intptr = safe_integral<bsl::intptr>;

    /// @brief provides the bsl::safe_integral version of bsl::uint8
    using safe_uint8 = safe_integral<bsl::uint8>;
    /// @brief provides the bsl::safe_integral version of bsl::uint16
    using safe_uint16 = safe_integral<bsl::uint16>;
    /// @brief provides the bsl::safe_integral version of bsl::uint32
    using safe_uint32 = safe_integral<bsl::uint32>;
    /// @brief provides the bsl::safe_integral version of bsl::uint64
    using safe_uint64 = safe_integral<bsl::uint64>;
    /// @brief provides the bsl::safe_integral version of bsl::uint_fast8
    using safe_uint_fast8 = safe_integral<bsl::uint_fast8>;
    /// @brief provides the bsl::safe_integral version of bsl::uint_fast16
    using safe_uint_fast16 = safe_integral<bsl::uint_fast16>;
    /// @brief provides the bsl::safe_integral version of bsl::uint_fast32
    using safe_uint_fast32 = safe_integral<bsl::uint_fast32>;
    /// @brief provides the bsl::safe_integral version of bsl::uint_fast64
    using safe_uint_fast64 = safe_integral<bsl::uint_fast64>;
    /// @brief provides the bsl::safe_integral version of bsl::uint_least8
    using safe_uint_least8 = safe_integral<bsl::uint_least8>;
    /// @brief provides the bsl::safe_integral version of bsl::uint_least16
    using safe_uint_least16 = safe_integral<bsl::uint_least16>;
    /// @brief provides the bsl::safe_integral version of bsl::uint_least32
    using safe_uint_least32 = safe_integral<bsl::uint_least32>;
    /// @brief provides the bsl::safe_integral version of bsl::uint_least64
    using safe_uint_least64 = safe_integral<bsl::uint_least64>;
    /// @brief provides the bsl::safe_integral version of bsl::uintmax
    using safe_uintmax = safe_integral<bsl::uintmax>;
    /// @brief provides the bsl::safe_integral version of bsl::uintptr
    using safe_uintptr = safe_integral<bsl::uintptr>;
}

#endif
