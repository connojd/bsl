//
// Copyright (C) 2019 Assured Information Security, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef BAREFLANK_SUPPORT_LIBRARY
#define BAREFLANK_SUPPORT_LIBRARY

#include <iostream>
#include <stdexcept>
#include <utility>

// --------------------------------------------------------------------------
// Helper Macros
// --------------------------------------------------------------------------

#define BAREFLANK_STRINGIFY_DETAILS(a) #a
#define BAREFLANK_STRINGIFY(a) BAREFLANK_STRINGIFY_DETAILS(a)

#if defined(__clang__) || defined(__GNUC__)
#define BAREFLANK_LIKELY(a) __builtin_expect(!!(a), 1)
#define BAREFLANK_UNLIKELY(a) __builtin_expect(!!(a), 0)
#else
#define BAREFLANK_LIKELY(a) (!!(a))
#define BAREFLANK_UNLIKELY(a) (!!(a))
#endif

// --------------------------------------------------------------------------
// Contracts
// --------------------------------------------------------------------------

#ifdef BAREFLANK_CORE_GUIDELINE_COMPLIANT

#define BAREFLANK_CONTRACT_CHECK_THROW(test, file, line)                       \
    if (BAREFLANK_UNLIKELY(!(test))) {                                         \
        throw std::logic_error("contract violation at [" line "]: " file);     \
    }

#define BAREFLANK_CONTRACT_CHECK_TERMINATE(test, file, line)                   \
    if (BAREFLANK_UNLIKELY(!(test))) {                                         \
        std::cerr << "contract failure at [" line "]: " file << '\n';          \
        std::terminate();                                                      \
    }

#if defined(BAREFLANK_THROW_ON_CONTRACT_VIOLATION)

#define bfexpects(test)                                                        \
    BAREFLANK_CONTRACT_CHECK_THROW(                                            \
        (test), __FILE__, BAREFLANK_STRINGIFY(__LINE__))
#define bfensures(test)                                                        \
    BAREFLANK_CONTRACT_CHECK_THROW(                                            \
        (test), __FILE__, BAREFLANK_STRINGIFY(__LINE__))

#define bfexpects_if(cond, test)                                               \
    if (cond) {                                                                \
        bfexpects(test);                                                       \
    }
#define bfensures_if(cond, test)                                               \
    if (cond) {                                                                \
        bfensures(test);                                                       \
    }

#elif defined(BAREFLANK_TERMINATE_ON_CONTRACT_VIOLATION)

#define bfexpects(test)                                                        \
    BAREFLANK_CONTRACT_CHECK_TERMINATE(                                        \
        (test), __FILE__, BAREFLANK_STRINGIFY(__LINE__))
#define bfensures(test)                                                        \
    BAREFLANK_CONTRACT_CHECK_TERMINATE(                                        \
        (test), __FILE__, BAREFLANK_STRINGIFY(__LINE__))

#define bfexpects_if(cond, test)                                               \
    if (cond) {                                                                \
        bfexpects(test);                                                       \
    }
#define bfensures_if(cond, test)                                               \
    if (cond) {                                                                \
        bfensures(test);                                                       \
    }

#elif defined(BAREFLANK_CUSTOM_ON_CONTRACT_VIOLATION)

#else

#define bfexpects(test)
#define bfexpects_if(cond, test)

#define bfensures(test)
#define bfensures_if(cond, test)

#endif

#define bfexpects_terminate(test)                                              \
    BAREFLANK_CONTRACT_CHECK_TERMINATE(                                        \
        (test), __FILE__, BAREFLANK_STRINGIFY(__LINE__))
#define bfensures_terminate(test)                                              \
    BAREFLANK_CONTRACT_CHECK_TERMINATE(                                        \
        (test), __FILE__, BAREFLANK_STRINGIFY(__LINE__))

#define bfexpects_if_terminate(cond, test)                                     \
    if (cond) {                                                                \
        bfexpects_terminate(test);                                             \
    }
#define bfensures_if_terminate(cond, test)                                     \
    if (cond) {                                                                \
        bfensures_terminate(test);                                             \
    }

#if defined(BAREFLANK_THROW_ON_CONTRACT_VIOLATION)
#define BFNOEXCEPT
#elif defined(BAREFLANK_TERMINATE_ON_CONTRACT_VIOLATION)
#define BFNOEXCEPT noexcept
#elif defined(BAREFLANK_CUSTOM_ON_CONTRACT_VIOLATION)
#else
#define BFNOEXCEPT noexcept
#endif

#ifdef BAREFLANK_IGNORE_ENSURES_CONTRACT_VIOLATIONS

#undef bfensures
#undef bfensures_if
#undef bfensures_terminate
#undef bfensures_if_terminate

#define bfensures(test)
#define bfensures_if(cond, test)
#define bfensures_terminate(test)
#define bfensures_if_terminate(cond, test)

#endif

#else

#define bfexpects(test)
#define bfexpects_if(cond, test)
#define bfexpects_terminate(test)
#define bfexpects_if_terminate(cond, test)

#define bfensures(test)
#define bfensures_if(cond, test)
#define bfensures_terminate(test)
#define bfensures_if_terminate(cond, test)

#define BFNOEXCEPT noexcept

#endif

// --------------------------------------------------------------------------
// Type Trait Macros
// --------------------------------------------------------------------------

#define BAREFLANK_IS_POINTER(a)                                                \
    std::enable_if_t<std::is_pointer<a>::value, int> = 0
#define BAREFLANK_IS_NOT_POINTER(a)                                            \
    std::enable_if_t<not std::is_pointer<a>::value, int> = 0

#define BAREFLANK_IS_REFERENCE(a)                                              \
    std::enable_if_t<std::is_reference<a>::value, int> = 0
#define BAREFLANK_IS_NOT_REFERENCE(a)                                          \
    std::enable_if_t<not std::is_reference<a>::value, int> = 0

#define BAREFLANK_IS_DEFAULT_CONSTRUCTABLE(a)                                  \
    std::enable_if_t<std::is_default_constructible<a>::value, int> = 0
#define BAREFLANK_IS_NOT_DEFAULT_CONSTRUCTABLE(a)                              \
    std::enable_if_t<not std::is_default_constructible<a>::value, int> = 0

#define BAREFLANK_IS_NOTHROW_MOVE_CONSTRUCTABLE(a)                             \
    std::enable_if_t<std::is_nothrow_move_constructible<a>::value, int> = 0
#define BAREFLANK_IS_NOT_NOTHROW_MOVE_CONSTRUCTABLE(a)                         \
    std::enable_if_t<not std::is_nothrow_move_constructible<a>::value, int> = 0

#define BAREFLANK_IS_NOTHROW_MOVE_ASSIGNABLE(a)                                \
    std::enable_if_t<std::is_nothrow_move_assignable<a>::value, int> = 0
#define BAREFLANK_IS_NOT_NOTHROW_MOVE_ASSIGNABLE(a)                            \
    std::enable_if_t<not std::is_nothrow_move_assignable<a>::value, int> = 0

// --------------------------------------------------------------------------
// Iterators
// --------------------------------------------------------------------------

namespace bsl
{
    // Random Access Iterator
    //
    // The following defines a random access iterator. For more
    // information, please see the following:
    // https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator
    //
    // The legacy random access iterator has a lot of requirements. The
    // following are most of them:
    // - default constructable
    //   https://en.cppreference.com/w/cpp/named_req/DefaultConstructible
    // - destructable
    //   https://en.cppreference.com/w/cpp/named_req/Destructible
    // - copy constructable/assignable
    //   https://en.cppreference.com/w/cpp/named_req/CopyConstructible
    //   https://en.cppreference.com/w/cpp/named_req/CopyAssignable
    // - move constructable/assignable
    //   https://en.cppreference.com/w/cpp/named_req/MoveConstructible
    //   https://en.cppreference.com/w/cpp/named_req/MoveAssignable
    // - implements *iter, ++iter and iter++
    //   https://en.cppreference.com/w/cpp/named_req/Iterator
    // - equality operators
    //   https://en.cppreference.com/w/cpp/named_req/EqualityComparable
    // - iter != iter, iter->, (void)i++ == (void)++i, *i++
    //   https://en.cppreference.com/w/cpp/named_req/InputIterator
    // - iter-- and --iter, *iter--
    //   https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator
    // - +, +=, -, -=, iter[n], <, <=, >, >=
    //   https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator
    //
    template<typename Array, typename T>
    class random_access_iterator
    {
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference = T &;
        using pointer = T *;
        using iterator_category = std::random_access_iterator_tag;

        random_access_iterator() = default;

        random_access_iterator(const Array *a, difference_type i) BFNOEXCEPT :
            m_a{a},
            m_i{i}
        {}

        constexpr auto operator*() const BFNOEXCEPT -> reference
        {
            bfexpects(m_a != nullptr);
            bfexpects(m_i >= 0 && m_i < m_a->ssize());

            return m_a->get()[static_cast<std::size_t>(m_i)];
        }

        constexpr auto operator-> () const BFNOEXCEPT -> pointer
        {
            bfexpects(m_a != nullptr);
            bfexpects(m_i >= 0 && m_i < m_a->ssize());

            return &m_a->get()[static_cast<std::size_t>(m_i)];
        }

        constexpr auto operator[](std::size_t n) const BFNOEXCEPT -> reference
        {
            bfexpects(m_a != nullptr);
            bfexpects(n < m_a->size());

            return m_a->get()[n];
        }

        constexpr auto
        operator++() noexcept -> random_access_iterator &
        {
            ++m_i;
            return *this;
        }

        constexpr auto
        operator++(int) noexcept -> random_access_iterator
        {
            auto ret = *this;
            ++(*this);
            return ret;
        }

        constexpr auto
        operator--() noexcept -> random_access_iterator &
        {
            --m_i;
            return *this;
        }

        constexpr auto
        operator--(int) noexcept -> random_access_iterator
        {
            auto ret = *this;
            --(*this);
            return ret;
        }

        constexpr auto
        operator+(difference_type n) const noexcept -> random_access_iterator
        {
            auto ret = *this;
            return ret += n;
        }

        constexpr auto
        operator-(difference_type n) const noexcept -> random_access_iterator
        {
            auto ret = *this;
            return ret -= n;
        }

        constexpr auto
        operator+=(difference_type n) noexcept -> random_access_iterator &
        {
            m_i += n;
            return *this;
        }

        constexpr auto
        operator-=(difference_type n) noexcept -> random_access_iterator &
        {
            m_i -= n;
            return *this;
        }

        constexpr auto
        operator-(const random_access_iterator &rhs) const noexcept
            -> difference_type
        {
            return m_i - rhs.m_i;
        }

        friend constexpr auto
        operator==(
            random_access_iterator lhs, random_access_iterator rhs) noexcept
            -> bool
        {
            return lhs.m_a == rhs.m_a && lhs.m_i == rhs.m_i;
        }

        friend constexpr auto
        operator!=(
            random_access_iterator lhs, random_access_iterator rhs) noexcept
            -> bool
        {
            return !(lhs == rhs);
        }

        friend constexpr auto
        operator<(
            random_access_iterator lhs, random_access_iterator rhs) noexcept
            -> bool
        {
            return lhs.m_i < rhs.m_i;
        }

        friend constexpr auto
        operator<=(
            random_access_iterator lhs, random_access_iterator rhs) noexcept
            -> bool
        {
            return lhs.m_i <= rhs.m_i;
        }

        friend constexpr auto
        operator>(
            random_access_iterator lhs, random_access_iterator rhs) noexcept
            -> bool
        {
            return lhs.m_i > rhs.m_i;
        }

        friend constexpr auto
        operator>=(
            random_access_iterator lhs, random_access_iterator rhs) noexcept
            -> bool
        {
            return lhs.m_i >= rhs.m_i;
        }

    private:
        const Array *m_a{};
        difference_type m_i{};
    };
}    // namespace bsl

// --------------------------------------------------------------------------
// Dynamic Array
// --------------------------------------------------------------------------

namespace bsl
{
    // ----------------------------------------------------------------------
    // Deleters
    // ----------------------------------------------------------------------

    template<typename T>
    struct default_deleter
    {
        auto
        operator()(T *ptr, size_t /*unused*/) -> void
        {
            delete[] ptr;
        }
    };

    // ----------------------------------------------------------------------
    // dynarray
    // ----------------------------------------------------------------------

    // Dynamic Array
    //
    // The dynamic array is designed to fill a whole in C++ that currently
    // exists as C++ currently does not have support for a GSL compliant,
    // owning, dynamic array type. std::array provides this support for static
    // arrays, but nothing currently exists for dynamic arrays.
    //
    // THIS DYNAMIC ARRAY IS NOT MEANT TO COMPETE WITH THE FOLLOWING:
    // https://en.cppreference.com/w/cpp/container/dynarray
    //
    // The Dynamic Array is modeled after std::unique_ptr and std::array, and
    // shares the same interface with a couple of differences:
    // - there is no need to use the [] syntax as the dynamic array is assumed
    //   to be an array, which simplifies the interface a bit as there are no
    //   conflicts between [] and non-[]
    // - the dynamic array supports some of the APIs from std::array where
    //   they make sense. This includes iterator support. In this sense, the
    //   APIs are somewhat similar to what was proposed for std::dynarray, but
    //   with the exception that this is intended to be a library
    //   implementation with no support for the stack-based optimizations that
    //   std::dynarray was attempting to pursue. Instead, we decided to model
    //   the bsl::dynarray after the std::unique_ptr as it comes really close
    //   to a complete implementation (missing iterators, size and Core
    //   Guideline Compliance)
    // - the dynamic array is designed to be Core Guideline Compliant when
    //   enabled, with the same ability to define how contract violations are
    //   handled. By default, to achieve this compliance, you must use .at() as
    //   this is always bounds checked. If you would like the bsl::dynarray to
    //   be fully compliant like a gsl::span, define
    //   BAREFLANK_CORE_GUIDELINE_COMPLIANT. Like the GSL, you can control
    //   whether exceptions are thrown or std::terminate is called. This
    //   changes some of the noexcept rules defined by std::unique_ptr. Also
    //   note that not all of the functions can throw on contract violations.
    //   An example of this are move semantics and any functions that are
    //   used by these semantics.
    //
    // The implementation of this code, and it's documentation was inspired
    // by the following:
    // https://en.cppreference.com/w/cpp/memory/unique_ptr
    // https://en.cppreference.com/w/cpp/container/array
    //
    // TODO
    // - operators >, >=, <, and <=
    // - support for deleter pointer and reference types
    //
    template<
        typename T,
        typename Deleter = default_deleter<T>,
        BAREFLANK_IS_NOT_POINTER(Deleter),
        BAREFLANK_IS_NOT_REFERENCE(Deleter)>
    class dynarray : public Deleter
    {
    public:
        using value_type = T;
        using element_type = T;
        using index_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = T &;
        using const_reference = const T &;
        using pointer = T *;
        using const_pointer = const T *;
        using deleter_type = Deleter;
        using const_deleter_type = const Deleter;
        using iterator = random_access_iterator<dynarray, T>;
        using const_iterator = random_access_iterator<dynarray, const T>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    public:
        // Default Constructor
        //
        // Constructs a bsl::dynarray that owns nothing. Value-initializes the
        // stored pointer, stored count and the stored deleter. Requires that
        // Deleter is DefaultConstructible and that construction does not throw
        // an exception.
        //
        // This overload only participate in overload resolution if
        // std::is_default_constructible<Deleter>::value is true and Deleter is
        // not a pointer or reference type.
        //
        // @expects
        // @ensures empty() == true
        //
        template<BAREFLANK_IS_DEFAULT_CONSTRUCTABLE(deleter_type)>
        constexpr dynarray() noexcept
        {
            bfensures_terminate(empty());
        }

        // Pointer / Count Constructor
        //
        // Constructs a bsl::dynarray which owns p, initializing the stored
        // pointer with p, the stored count with count, and value-initializing
        // the stored deleter. Requires that Deleter is DefaultConstructible
        // and that construction does not throw an exception.
        //
        // This overload only participates in overload resolution if
        // std::is_default_constructible<Deleter>::value is true and Deleter
        // is not a pointer or reference type.
        //
        // If BAREFLANK_THROW_ON_CONTRACT_VIOLATION is defined, this function
        // can throw on contract violations. Otherwise, this function is marked
        // as noexcept.
        //
        // @expects ptr != nullptr
        // @expects count != 0
        // @ensures empty() == false
        //
        // @param ptr a pointer to an array
        // @param count the number of elements in the array
        //
        template<BAREFLANK_IS_DEFAULT_CONSTRUCTABLE(deleter_type)>
        explicit dynarray(pointer ptr, index_type count) BFNOEXCEPT :
            deleter_type(deleter_type()),
            m_ptr{ptr},
            m_count{count}
        {
            bfexpects(ptr != nullptr && count != 0);
            bfensures(!empty());
        }

        // Pointer / Count Constructor (const Deleter &)
        //
        // Constructs a bsl::dynarray object which owns p, initializing the
        // stored pointer with p, the stored count with count and initializing
        // a deleter D as a const l-value reference using
        // std::forward<decltype(d)>(d).
        //
        // This overload only participates in overload resolution if
        // Deleter is not a pointer or reference type.
        //
        // If BAREFLANK_THROW_ON_CONTRACT_VIOLATION is defined, this function
        // can throw on contract violations. Otherwise, this function is marked
        // as noexcept.
        //
        // @expects ptr != nullptr
        // @expects count != 0
        // @ensures empty() == false
        //
        // @param ptr a pointer to an array
        // @param count the number of elements in the array
        // @param a deleter to use to destroy the array
        //
        explicit dynarray(pointer ptr, index_type count, const deleter_type &d)
            BFNOEXCEPT :

            deleter_type(std::forward<decltype(d)>(d)),
            m_ptr{ptr},
            m_count{count}
        {
            bfexpects(ptr != nullptr && count != 0);
            bfensures(!empty());
        }

        // Pointer / Count Constructor (Deleter &&)
        //
        // Constructs a bsl::dynarray object which owns p, initializing the
        // stored pointer with p, the stored count with count and initializing
        // a deleter D as an r-value reference using
        // std::forward<decltype(d)>(d).
        //
        // This overload only participates in overload resolution if
        // Deleter is not a pointer or reference type.
        //
        // If BAREFLANK_THROW_ON_CONTRACT_VIOLATION is defined, this function
        // can throw on contract violations. Otherwise, this function is marked
        // as noexcept.
        //
        // @expects ptr != nullptr
        // @expects count != 0
        // @ensures empty() == false
        //
        // @param ptr a pointer to an array
        // @param count the number of elements in the array
        // @param a deleter to use to destroy the array
        //
        explicit dynarray(pointer ptr, index_type count, deleter_type &&d)
            BFNOEXCEPT :

            deleter_type(std::forward<decltype(d)>(d)),
            m_ptr{ptr},
            m_count{count}
        {
            bfexpects(ptr != nullptr && count != 0);
            bfensures(!empty());
        }

        // Move Constructor
        //
        // Constructs a bsl::dynarray by transferring ownership from u to *this
        // and stores the null pointer and 0 count in u. This constructor only
        // participates in overload resolution if
        // std::is_move_nothrow_constructible<Deleter>::value is true.
        //
        // This function always calls std::terminate on contract violations.
        //
        // @expects
        // @ensures u.empty()
        //
        // @param u another dynarray to acquire the array from
        //
        template<BAREFLANK_IS_NOTHROW_MOVE_CONSTRUCTABLE(deleter_type)>
        explicit dynarray(dynarray &&u) noexcept : deleter_type(std::move(u))
        {
            m_ptr = std::exchange(u.m_ptr, m_ptr);
            m_count = std::exchange(u.m_count, m_count);

            bfensures_terminate(u.empty());
        }

        // Destructor
        //
        // If get() == nullptr there are no effects. Otherwise, the array
        // is destroyed via get_deleter()(get(), size()). Requires that
        // get_deleter()(get(), size()) does not throw exceptions.
        //
        // @expects
        // @ensures
        //
        ~dynarray()
        {
            if (get() != nullptr) {
                get_deleter()(get(), size());
            }
        }

        // Move Operator
        //
        // Transfers ownership from r to *this as if by calling
        // reset(r.release()) followed by an assignment of get_deleter() from
        // std::move(r.get_deleter()). This requires that the deleter is
        // nothrow-MoveAssignable. If r == *this, the function has no effect.
        //
        // This function always calls std::terminate on contract violations.
        //
        // @expects
        // @ensures u.empty()
        //
        // @param r another dynarray to acquire the array from
        // @return *this
        //
        template<BAREFLANK_IS_NOTHROW_MOVE_ASSIGNABLE(deleter_type)>
        constexpr auto
        operator=(dynarray &&r) noexcept -> dynarray &
        {
            if (&r == this) {
                return *this;
            }

            reset(r.release());
            get_deleter() = std::move(r.get_deleter());

            bfensures_terminate(r.empty());
            return *this;
        }

        // Release
        //
        // Releases the ownership of the array if any. get() returns
        // nullptr and size() returns 0 after the call
        //
        // This function always calls std::terminate on contract violations.
        //
        // @expects
        // @ensures get() == nullptr
        // @ensures size() == 0
        //
        // @return a std::pair containing a pointer to the array and
        //     the number of elements in the array or nullptr and 0
        //     if empty(), i.e. the value which would be
        //     returned by get() and size() before the call.
        //
        [[nodiscard]] constexpr auto
        release() noexcept -> std::pair<pointer, index_type>
        {
            auto old_ptr = get();
            auto old_count = size();

            m_ptr = pointer();
            m_count = index_type();

            bfensures_terminate(get() == pointer());
            bfensures_terminate(size() == index_type());

            return {old_ptr, old_count};
        }

        // Reset (pointer, count)
        //
        // Given current_ptr, the pointer to the array, and current_size, the
        // number of elements in the array, performs the following actions,
        // in this order:
        // - Saves a copy of the current pointer old_ptr = m_ptr, and a copy
        //   of the current number of elements old_count = m_count.
        // - Overwrites the current pointer with the argument m_ptr = ptr
        //   and the current number of elements with the argument
        //   m_count = count.
        // - If the old pointer was non-empty, deletes the previous array with
        //   if(old_ptr) get_deleter()(old_ptr, old_count).
        //
        // This function always calls std::terminate on contract violations.
        //
        // @expects ptr != nullptr || count == 0
        // @expects ptr == nullptr || count >= 1
        // @ensures if ptr == nullptr, empty()
        // @ensures if ptr != nullptr, !empty()
        //
        // @param ptr pointer to a new array
        // @param count the number of elements in the new array
        //
        constexpr auto
        reset(pointer ptr = pointer(), index_type count = index_type()) noexcept
            -> void
        {
            bfexpects_terminate(ptr != nullptr || count == 0);
            bfexpects_terminate(ptr == nullptr || count >= 1);

            auto old_ptr = m_ptr;
            auto old_count = m_count;

            m_ptr = ptr;
            m_count = count;

            if (old_ptr) {
                get_deleter()(old_ptr, old_count);
            }

            bfensures_if_terminate(ptr == nullptr, empty());
            bfensures_if_terminate(ptr != nullptr, !empty());
        }

        // Reset (std::pair)
        //
        // Equivalent to reset(info.first, info.second)
        //
        constexpr auto
        reset(const std::pair<pointer, index_type> &info) noexcept -> void
        {
            reset(info.first, info.second);
        }

        // Reset (nullptr)
        //
        // Equivalent to reset(pointer(), index_type())
        //
        constexpr auto reset(std::nullptr_t) noexcept -> void
        {
            reset(pointer(), index_type());
        }

        // Swap
        //
        // Swaps the array, the number of elements in the array, and the
        // associated deleters of *this to another dynarray other.
        //
        // @expects
        // @ensures
        //
        // @param other another dynarray to swap the array, the number of
        //     elements in the array, and the associated deleters with
        //
        constexpr auto
        swap(dynarray &other) noexcept -> void
        {
            dynarray tmp(std::move(*this));
            *this = std::move(other);
            other = std::move(tmp);
        }

        // Get
        //
        // Returns a pointer to the array or nullptr if no array is
        // owned.
        //
        // @expects
        // @ensures
        //
        // @return Pointer to the array or nullptr if no array is owned.
        //
        [[nodiscard]] constexpr auto
        get() const -> pointer
        {
            return m_ptr;
        }

        // Get Deleter
        //
        // Returns the deleter object which would be used for destruction of
        // the array.
        //
        // @expects
        // @ensures
        //
        // @return The stored deleter object.
        //
        [[nodiscard]] constexpr auto
        get_deleter() noexcept -> deleter_type &
        {
            return *this;
        }

        // Get Deleter (const)
        //
        // Returns the deleter object which would be used for destruction of
        // the array.
        //
        // @expects
        // @ensures
        //
        // @return The stored deleter object.
        //
        [[nodiscard]] constexpr auto
        get_deleter() const noexcept -> const_deleter_type &
        {
            return *this;
        }

        // Valid
        //
        // Checks whether *this owns an array, i.e. whether get() != nullptr
        //
        // @expects
        // @ensures
        //
        // @return true if *this owns an array, false otherwise.
        //
        explicit operator bool() const noexcept
        {
            return get() != nullptr;
        }

        // Subscript Operator
        //
        // operator[] provides access to the elements in the array.
        // The parameter i shall be less than the number of elements in the
        // array; otherwise, the behavior is undefined by default, or
        // throws an exception when BAREFLANK_CORE_GUIDELINE_COMPLIANT is
        // defined.
        //
        // @expects i < size()
        // @ensures
        //
        // @param i the index of the element to be returned
        // @return Returns the element at index i, i.e. get()[i].
        //
        [[nodiscard]] constexpr auto operator[](index_type i) -> reference
        {
            bfexpects(i < size());
            return get()[i];
        }

        // Subscript Operator (const)
        //
        // operator[] provides access to the elements in the array.
        // The parameter i shall be less than the number of elements in the
        // array; otherwise, the behavior is undefined by default, or
        // throws an exception when BAREFLANK_CORE_GUIDELINE_COMPLIANT is
        // defined.
        //
        // @expects i < size()
        // @ensures
        //
        // @param i the index of the element to be returned
        // @return Returns the element at index i, i.e. get()[i].
        //
        [[nodiscard]] constexpr auto operator[](index_type i) const
            -> const_reference
        {
            bfexpects(i < size());
            return get()[i];
        }

        // At
        //
        // Returns a reference to the element at specified location pos,
        // with bounds checking. If pos is not within the range of the
        // array, an exception of type std::out_of_range is thrown.
        //
        // @expects pos < size()
        // @ensures
        //
        // @param pos position of the element to return
        // @return reference to the requested element.
        //
        [[nodiscard]] constexpr auto
        at(index_type pos) -> reference
        {
            if (pos >= size()) {
                throw std::out_of_range("dynarray: pos >= size()");
            }

            return get()[pos];
        }

        // At (const)
        //
        // Returns a reference to the element at specified location pos,
        // with bounds checking. If pos is not within the range of the
        // array, an exception of type std::out_of_range is thrown.
        //
        // @expects pos < size()
        // @ensures
        //
        // @param pos position of the element to return
        // @return reference to the requested element.
        //
        [[nodiscard]] constexpr auto
        at(index_type pos) const -> const_reference
        {
            if (pos >= size()) {
                throw std::out_of_range("dynarray: pos >= size()");
            }

            return get()[pos];
        }

        // Front
        //
        // Returns a reference to the first element in the array. Calling
        // front on an empty array is undefined unless
        // BAREFLANK_CORE_GUIDELINE_COMPLIANT is defined in which case an
        // exception is thrown.
        //
        // @expects !empty()
        // @ensures
        //
        // @return reference to the first element (i.e., get()[0])
        //
        [[nodiscard]] constexpr auto
        front() -> reference
        {
            bfexpects(!empty());
            return get()[0];
        }

        // Front (const)
        //
        // Returns a reference to the first element in the array. Calling
        // front on an empty array is undefined unless
        // BAREFLANK_CORE_GUIDELINE_COMPLIANT is defined in which case an
        // exception is thrown.
        //
        // @expects !empty()
        // @ensures
        //
        // @return reference to the first element (i.e., get()[0])
        //
        [[nodiscard]] constexpr auto
        front() const -> const_reference
        {
            bfexpects(!empty());
            return get()[0];
        }

        // Back
        //
        // Returns a reference to the last element in the array. Calling
        // back on an empty array is undefined unless
        // BAREFLANK_CORE_GUIDELINE_COMPLIANT is defined in which case an
        // exception is thrown.
        //
        // @expects !empty()
        // @ensures
        //
        // @return reference to the last element (i.e., get()[size() - 1])
        //
        [[nodiscard]] constexpr auto
        back() -> reference
        {
            bfexpects(!empty());
            return get()[size() - 1];
        }

        // Back (const)
        //
        // Returns a reference to the last element in the array. Calling
        // back on an empty array is undefined unless
        // BAREFLANK_CORE_GUIDELINE_COMPLIANT is defined in which case an
        // exception is thrown.
        //
        // @expects !empty()
        // @ensures
        //
        // @return reference to the last element (i.e., get()[size() - 1])
        //
        [[nodiscard]] constexpr auto
        back() const -> const_reference
        {
            bfexpects(!empty());
            return get()[size() - 1];
        }

        // Data
        //
        // Returns a pointer to the underlying array serving as element storage.
        // The pointer is such that range [data(); data() + size()) is always
        // a valid range, even if the array is empty (data() is not
        // dereferenceable in that case).
        //
        // @expects
        // @ensures
        //
        // @return Pointer to the underlying element storage. For non-empty
        //     arrays, the returned pointer compares equal to the address
        //     of the first element.
        //
        [[nodiscard]] constexpr auto
        data() noexcept -> pointer
        {
            return m_ptr;
        }

        // Data (const)
        //
        // Returns a pointer to the underlying array serving as element storage.
        // The pointer is such that range [data(); data() + size()) is always
        // a valid range, even if the array is empty (data() is not
        // dereferenceable in that case).
        //
        // @expects
        // @ensures
        //
        // @return Pointer to the underlying element storage. For non-empty
        //     arrays, the returned pointer compares equal to the address
        //     of the first element.
        //
        [[nodiscard]] constexpr auto
        data() const noexcept -> const_pointer
        {
            return m_ptr;
        }

        // Begin
        //
        // Returns an iterator to the first element of the array. If the
        // array is empty, the returned iterator will be equal to end().
        //
        // @expects
        // @ensures
        //
        // @return Iterator to the first element
        //
        [[nodiscard]] constexpr auto
        begin() noexcept -> iterator
        {
            return {this, 0};
        }

        // Begin
        //
        // Returns an iterator to the first element of the array. If the
        // array is empty, the returned iterator will be equal to end().
        //
        // @expects
        // @ensures
        //
        // @return Iterator to the first element
        //
        [[nodiscard]] constexpr auto
        begin() const noexcept -> const_iterator
        {
            return {this, 0};
        }

        // Begin
        //
        // Returns an iterator to the first element of the array. If the
        // array is empty, the returned iterator will be equal to end().
        //
        // @expects
        // @ensures
        //
        // @return Iterator to the first element
        //
        [[nodiscard]] constexpr auto
        cbegin() const noexcept -> const_iterator
        {
            return {this, 0};
        }

        // End
        //
        // Returns an iterator to the element following the last element of
        // the array. This element acts as a placeholder; attempting to
        // access it results in undefined behavior unless
        // BAREFLANK_CORE_GUIDELINE_COMPLIANT is defined in which case an
        // exception is thrown.
        //
        // @expects
        // @ensures
        //
        // @return Iterator to the element following the last element.
        //
        [[nodiscard]] constexpr auto
        end() noexcept -> iterator
        {
            return {this, ssize()};
        }

        // End
        //
        // Returns an iterator to the element following the last element of
        // the array. This element acts as a placeholder; attempting to
        // access it results in undefined behavior unless
        // BAREFLANK_CORE_GUIDELINE_COMPLIANT is defined in which case an
        // exception is thrown.
        //
        // @expects
        // @ensures
        //
        // @return Iterator to the element following the last element.
        //
        [[nodiscard]] constexpr auto
        end() const noexcept -> const_iterator
        {
            return {this, ssize()};
        }

        // End
        //
        // Returns an iterator to the element following the last element of
        // the array. This element acts as a placeholder; attempting to
        // access it results in undefined behavior unless
        // BAREFLANK_CORE_GUIDELINE_COMPLIANT is defined in which case an
        // exception is thrown.
        //
        // @expects
        // @ensures
        //
        // @return Iterator to the element following the last element.
        //
        [[nodiscard]] constexpr auto
        cend() const noexcept -> const_iterator
        {
            return {this, ssize()};
        }

        // Reverse Begin
        //
        // Returns a reverse iterator to the first element of the reversed
        // array. It corresponds to the last element of the non-reversed array.
        // If the array is empty, the returned iterator is equal to rend().
        //
        // @expects
        // @ensures
        //
        // @return Reverse iterator to the first element.
        //
        [[nodiscard]] constexpr auto
        rbegin() noexcept -> reverse_iterator
        {
            return std::make_reverse_iterator(end());
        }

        // Reverse Begin
        //
        // Returns a reverse iterator to the first element of the reversed
        // array. It corresponds to the last element of the non-reversed array.
        // If the array is empty, the returned iterator is equal to rend().
        //
        // @expects
        // @ensures
        //
        // @return Reverse iterator to the first element.
        //
        [[nodiscard]] constexpr auto
        rbegin() const noexcept -> const_reverse_iterator
        {
            return std::make_reverse_iterator(end());
        }

        // Reverse Begin
        //
        // Returns a reverse iterator to the first element of the reversed
        // array. It corresponds to the last element of the non-reversed array.
        // If the array is empty, the returned iterator is equal to rend().
        //
        // @expects
        // @ensures
        //
        // @return Reverse iterator to the first element.
        //
        [[nodiscard]] constexpr auto
        crbegin() const noexcept -> const_reverse_iterator
        {
            return std::make_reverse_iterator(cend());
        }

        // Reverse End
        //
        // Returns a reverse iterator to the element following the last element
        // of the reversed container. It corresponds to the element preceding
        // the first element of the non-reversed container. This element acts
        // as a placeholder, attempting to access it results in undefined
        // behavior. unless BAREFLANK_CORE_GUIDELINE_COMPLIANT is defined in
        // which case an exception is thrown.
        //
        // @expects
        // @ensures
        //
        // @return Reverse iterator to the element following the last element.
        //
        [[nodiscard]] constexpr auto
        rend() noexcept -> reverse_iterator
        {
            return std::make_reverse_iterator(begin());
        }

        // Reverse End
        //
        // Returns a reverse iterator to the element following the last element
        // of the reversed container. It corresponds to the element preceding
        // the first element of the non-reversed container. This element acts
        // as a placeholder, attempting to access it results in undefined
        // behavior. unless BAREFLANK_CORE_GUIDELINE_COMPLIANT is defined in
        // which case an exception is thrown.
        //
        // @expects
        // @ensures
        //
        // @return Reverse iterator to the element following the last element.
        //
        [[nodiscard]] constexpr auto
        rend() const noexcept -> const_reverse_iterator
        {
            return std::make_reverse_iterator(begin());
        }

        // Reverse End
        //
        // Returns a reverse iterator to the element following the last element
        // of the reversed container. It corresponds to the element preceding
        // the first element of the non-reversed container. This element acts
        // as a placeholder, attempting to access it results in undefined
        // behavior. unless BAREFLANK_CORE_GUIDELINE_COMPLIANT is defined in
        // which case an exception is thrown.
        //
        // @expects
        // @ensures
        //
        // @return Reverse iterator to the element following the last element.
        //
        [[nodiscard]] constexpr auto
        crend() const noexcept -> const_reverse_iterator
        {
            return std::make_reverse_iterator(cbegin());
        }

        // Empty
        //
        // Checks if the array has no elements, i.e. whether size() == 0;
        //
        // @expects
        // @ensures
        //
        // @return true if the array is empty, false otherwise
        //
        [[nodiscard]] constexpr auto
        empty() const noexcept -> bool
        {
            return size() == 0;
        }

        // Size
        //
        // Returns the number of elements in the array
        //
        // @expects
        // @ensures
        //
        // @return The number of elements in the array
        //
        [[nodiscard]] constexpr auto
        size() const noexcept -> index_type
        {
            return m_count;
        }

        // Signed Size
        //
        // Returns the number of elements in the array
        //
        // Notes:
        // - This is a likely a controversial addition. The total size of
        //   dynarray is maxed out on the difference_type as this class
        //   uses iterators. The problem is, when you are working with
        //   size information, you might need access to a signed version.
        //
        // @expects
        // @ensures
        //
        // @return The number of elements in the array
        //
        [[nodiscard]] constexpr auto
        ssize() const noexcept -> difference_type
        {
            return static_cast<difference_type>(m_count);
        }

        // Size in Bytes
        //
        // Returns the size of the array in bytes.
        //
        // @expects
        // @ensures
        //
        // @return The size of the array in bytes, i.e.,
        //     size() * sizeof(element_type)
        //
        [[nodiscard]] constexpr auto
        size_bytes() const noexcept -> index_type
        {
            return size() * sizeof(T);
        }

        // Max Size
        //
        // Returns the maximum number of elements the array is able to
        // hold due to system or library implementation limitations, i.e.
        // size() for the largest array.
        //
        // Notes:
        // - Since this class uses iterators, the max size must be based
        //   on the difference_type as iterators only work with signed types.
        // - This is asking for the maximum number of elements, so we must
        //   divide my the size of T.
        //
        // @expects
        // @ensures
        //
        // @return Maximum number of elements.
        //
        [[nodiscard]] constexpr auto
        max_size() const noexcept -> index_type
        {
            return std::numeric_limits<difference_type>::max() / sizeof(T);
        }

        // Fill (const T &)
        //
        // Assigns the given value value to all elements in the array.
        //
        // @expects
        // @ensures
        //
        // @param the value to assign to the elements
        //
        constexpr auto
        fill(const T &value) -> void
        {
            for (index_type i = 0; i < size(); ++i) {
                get()[i] = value;
            }
        }

    private:
        pointer m_ptr{};         ///< A pointer to the array
        index_type m_count{};    ///< The number of elements in the array

    public:
        //
        // Like the std::unique_ptr and the original proposal for the
        // std::dynarray, this class is not copyable. To make a copy of
        // the dynarray, you must create a second one and copy the contents
        // manually. Unlike the std::dynarray, this class is movable.
        //
        // clang-format off
        dynarray(const dynarray &da) = delete;
        auto operator=(const dynarray &da) -> auto & = delete;
        // clang-format on
    };

    // ----------------------------------------------------------------------

    // Make Dynarray
    //
    // Constructs an array of T with size count. The function is equivalent to:
    // dynarray<T>(new T[count](), count).
    //
    // If BAREFLANK_THROW_ON_CONTRACT_VIOLATION is defined, this function
    // can throw on contract violations.
    //
    // @expects count > 0
    // @ensures
    //
    // @param count the size of the array to construct
    // @return a dynarray containing a newly allocated array with count
    //     number of elements value-initialized.
    //
    template<typename T>
    constexpr auto
    make_dynarray(size_t count) -> dynarray<T>
    {
        bfexpects(count > 0);
        return dynarray<T>(new T[count](), count);
    }

    // Make Dynarray
    //
    // Constructs an array of T with size count. The function is equivalent to:
    // dynarray<T>(new T[count], count).
    //
    // If BAREFLANK_THROW_ON_CONTRACT_VIOLATION is defined, this function
    // can throw on contract violations.
    //
    // @expects count > 0
    // @ensures
    //
    // @param count the size of the array to construct
    // @return a dynarray containing a newly allocated array with count
    //     number of elements default-initialized.
    //
    template<typename T>
    constexpr auto
    make_dynarray_default_init(size_t count) -> dynarray<T>
    {
        bfexpects(count > 0);
        return dynarray<T>(new T[count], count);
    }
}    // namespace bsl

// --------------------------------------------------------------------------
// Operator Overloads
// --------------------------------------------------------------------------

// Equality Operator (==)
//
// Checks if the contents of lhs and rhs are equal, that is, whether each
// element in lhs compares equal with the element in rhs at the same position.
//
// @expects
// @ensures
//
// @param lhs container whose contents to compare
// @param rhs container whose contents to compare
//
// @return true if the contents of the containers are equal, false otherwise
//
template<typename T1, typename D1, typename T2, typename D2>
constexpr bool
operator==(const bsl::dynarray<T1, D1> &lhs, const bsl::dynarray<T2, D2> &rhs)
{
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (size_t i = 0; i < lhs.size(); --i) {
        if (lhs.get()[i] != rhs.get()[i]) {
            return false;
        }
    }

    return true;
}

// Equality Operator (!=)
//
// Checks if the contents of lhs and rhs are equal, that is, whether each
// element in lhs compares equal with the element in rhs at the same position.
//
// @expects
// @ensures
//
// @param lhs container whose contents to compare
// @param rhs container whose contents to compare
//
// @return true if the contents of the containers are not equal, false otherwise
//
template<typename T1, typename D1, typename T2, typename D2>
constexpr bool
operator!=(const bsl::dynarray<T1, D1> &lhs, const bsl::dynarray<T2, D2> &rhs)
{
    return !operator==(lhs, rhs);
}

// Output Stream
//
// Inserts the value of the array's pointer into the output stream os.
// Equivalent to os << static_cast<const void *>(da.get())
//
// @expects
// @ensures
//
// @param os a std::basic_ostream to insert the pointer into
// @param da the array to insert into the stream
//
template<typename CharT, typename Traits, typename T, typename D>
std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &os, const bsl::dynarray<T, D> &da)
{
    return os << static_cast<const void *>(da.get());
}

// --------------------------------------------------------------------------
// File Map
// --------------------------------------------------------------------------

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace bsl
{
    // ----------------------------------------------------------------------
    // Deleters
    // ----------------------------------------------------------------------

    template<typename T>
    struct farray_deleter
    {
        auto
        operator()(T *ptr, size_t size) -> void
        {
            munmap(ptr, size);
        }
    };

    // ----------------------------------------------------------------------
    // farray
    // ----------------------------------------------------------------------

    // File Array
    //
    // The farray is a dynarray that maps in a file using map
    // functions instead of fstream and C style functions. Once the file is
    // mapped, you can use the full services of the dynarray to work with the
    // file as if it were any other array.
    //
    // TODO:
    // - farray should be able to open a file as read/write
    // - ofarray should be added to open a file as write only.
    //
    template<typename T = uint8_t>
    class farray : public dynarray<T, farray_deleter<T>>
    {
        using base_type = dynarray<T, farray_deleter<T>>;

    public:
        using value_type = typename base_type::value_type;
        using element_type = typename base_type::element_type;
        using index_type = typename base_type::index_type;
        using difference_type = typename base_type::difference_type;
        using reference = typename base_type::reference;
        using const_reference = typename base_type::const_reference;
        using pointer = typename base_type::pointer;
        using const_pointer = typename base_type::const_pointer;
        using deleter_type = typename base_type::deleter_type;
        using const_deleter_type = typename base_type::const_deleter_type;
        using iterator = typename base_type::iterator;
        using const_iterator = typename base_type::const_iterator;
        using reverse_iterator = typename base_type::reverse_iterator;
        using const_reverse_iterator =
            typename base_type::const_reverse_iterator;

    public:
        // Default Constructor
        //
        // Constructs an farray that does not map in a file.
        //
        // @expects
        // @ensures empty() == true
        //
        constexpr farray() noexcept
        {
            bfensures_terminate(this->empty());
        }

    protected:
        constexpr auto
        open_file(const std::string &filename, int prot) -> int
        {
            auto fd = open(filename.c_str(), prot);    // NOLINT
            if (fd == -1) {
                throw std::runtime_error("failed to open file");
            }

            return fd;
        }

        constexpr auto
        file_size(int fd) -> index_type
        {
            struct stat sb = {};
            if (fstat(fd, &sb) == -1) {
                throw std::runtime_error("failed to fstat file");
            }

            return sb.st_size;
        }

        constexpr auto
        map_file(int fd, index_type size, int prot, int perm) -> void *
        {
            auto ptr = mmap(nullptr, size, prot, perm, fd, 0);
            if (ptr == MAP_FAILED) {    // NOLINT
                throw std::runtime_error("failed to map file");
            }

            return ptr;
        }
    };

    // ----------------------------------------------------------------------
    // ifarray
    // ----------------------------------------------------------------------

    // In File Array
    //
    // The ifarray is a dynarray that maps in a file (read-only) using map
    // functions instead of fstream and C style functions. Once the file is
    // mapped, you can use the full services of the dynarray to work with the
    // file as if it were any other array.
    //
    template<typename T = uint8_t>
    class ifarray : public farray<T>
    {
        using base_type = farray<T>;

    public:
        using value_type = typename base_type::value_type;
        using element_type = typename base_type::element_type;
        using index_type = typename base_type::index_type;
        using difference_type = typename base_type::difference_type;
        using reference = typename base_type::reference;
        using const_reference = typename base_type::const_reference;
        using pointer = typename base_type::pointer;
        using const_pointer = typename base_type::const_pointer;
        using deleter_type = typename base_type::deleter_type;
        using const_deleter_type = typename base_type::const_deleter_type;
        using iterator = typename base_type::iterator;
        using const_iterator = typename base_type::const_iterator;
        using reverse_iterator = typename base_type::reverse_iterator;
        using const_reverse_iterator =
            typename base_type::const_reverse_iterator;

    public:
        // Default Constructor
        //
        // Constructs an ifarray that does not map in a file.
        //
        // @expects
        // @ensures empty() == true
        //
        constexpr ifarray() noexcept
        {
            bfensures_terminate(this->empty());
        }

        // Filename Constructor
        //
        // Constructs an ifarray by opening the file provided by filename,
        // and ensuring the dynarray contains the contents of the desired file.
        // If possible, this constructor will gain access to the contents of
        // the file by using the OS's mapping facilities instead of using
        // C++ or C style file operations.
        //
        // If the file cannot be opened for whatever reason, this function
        // will throw an exception.
        //
        // @expects filename.empty() == false
        // @ensures
        //
        // @param ptr a pointer to an array
        // @param count the number of elements in the array
        //
        explicit ifarray(const std::string &filename)
        {
            bfexpects(!filename.empty());

            constexpr const auto flag = O_RDONLY;
            constexpr const auto prot = PROT_READ;
            constexpr const auto perm = MAP_SHARED | MAP_POPULATE;    // NOLINT

            auto fd = this->open_file(filename, flag);
            auto size = this->file_size(fd);
            auto ptr = this->map_file(fd, size, prot, perm);

            close(fd);
            this->reset(static_cast<T *>(ptr), size / sizeof(T));
        }
    };
}    // namespace bsl

#endif
