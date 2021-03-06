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

#ifndef BSL_DETAILS_IFMAP_WINDOWS_HPP
#define BSL_DETAILS_IFMAP_WINDOWS_HPP

#include "../byte.hpp"
#include "../convert.hpp"
#include "../cstdint.hpp"
#include "../debug.hpp"
#include "../discard.hpp"
#include "../safe_integral.hpp"
#include "../span.hpp"
#include "../string_view.hpp"

#include <Windows.h>
#undef max
#undef min

namespace bsl
{
    /// @class bsl::ifmap
    ///
    /// <!-- description -->
    ///   @brief Maps a file as read-only, and returns a pointer to the file
    ///     via data() as well as the size of the mapped file via size().
    ///   @include example_ifmap_overview.hpp
    ///
    class ifmap final
    {
        /// @brief stores a handle to the mapped file.
        HANDLE m_file{};
        /// @brief stores a handle to the mapped file.
        HANDLE m_view{};
        /// @brief stores a view of the file that is mapped.
        span<byte> m_data{};

    public:
        /// @brief alias for: void
        using value_type = void;
        /// @brief alias for: safe_uintmax
        using size_type = safe_uintmax;
        /// @brief alias for: safe_uintmax
        using difference_type = safe_uintmax;
        /// @brief alias for: void *
        using pointer_type = void *;
        /// @brief alias for: void const *
        using const_pointer_type = void const *;

        /// <!-- description -->
        ///   @brief Creates a default ifmap that has not yet been mapped.
        ///   @include ifmap/example_ifmap_default_constructor.hpp
        ///
        ifmap() noexcept = default;

        /// <!-- description -->
        ///   @brief Creates a bsl::ifmap given a the filename and path of
        ///     the file to map as read-only.
        ///   @include ifmap/example_ifmap_constructor.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @param filename the filename and path of the file to map
        ///
        explicit ifmap(string_view const &filename) noexcept
        {
            m_file = CreateFileA(
                filename.data(),
                GENERIC_READ,
                0,
                nullptr,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr);

            m_view = CreateFileMappingA(m_file, nullptr, PAGE_READONLY, 0, 0, nullptr);
            if (nullptr == m_view) {
                bsl::alert() << "failed to open read-only file: "    // --
                             << filename                             // --
                             << bsl::endl;
                return;
            }

            DWORD high{};
            DWORD size{GetFileSize(m_file, &high)};
            if (size == INVALID_FILE_SIZE) {
                bsl::alert() << "failed to get the size of the read-only file: "    // --
                             << filename                                            // --
                             << bsl::endl;
                CloseHandle(m_file);
                return;
            }

            pointer_type const ptr{MapViewOfFile(m_view, FILE_MAP_READ, 0, 0, 0)};
            if (nullptr == ptr) {
                bsl::alert() << "failed to map read-only file: "    // --
                             << filename                            // --
                             << bsl::endl;
                CloseHandle(m_file);
                return;
            }

            m_data = as_writable_bytes(ptr, (to_umax(high) << 32U) | to_umax(size));
        }

        /// <!-- description -->
        ///   @brief Destructor unmaps a previously mapped file.
        ///
        ~ifmap() noexcept
        {
            UnmapViewOfFile(m_data.data());
            CloseHandle(m_view);
            CloseHandle(m_file);
        }

        /// <!-- description -->
        ///   @brief copy constructor
        ///
        /// <!-- inputs/outputs -->
        ///   @param o the object being copied
        ///
        constexpr ifmap(ifmap const &o) noexcept = default;

        /// <!-- description -->
        ///   @brief move constructor
        ///
        /// <!-- inputs/outputs -->
        ///   @param o the object being moved
        ///
        constexpr ifmap(ifmap &&o) noexcept = default;

        /// <!-- description -->
        ///   @brief copy assignment
        ///
        /// <!-- inputs/outputs -->
        ///   @param o the object being copied
        ///   @return a reference to *this
        ///
        ifmap &operator=(ifmap const &o) &noexcept = default;

        /// <!-- description -->
        ///   @brief move assignment
        ///
        /// <!-- inputs/outputs -->
        ///   @param o the object being moved
        ///   @return a reference to *this
        ///
        ifmap &operator=(ifmap &&o) &noexcept = default;

        /// <!-- description -->
        ///   @brief Returns a pointer to the read-only mapped file.
        ///   @include ifmap/example_ifmap_data.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns a pointer to the read-only mapped file.
        ///
        [[nodiscard]] constexpr const_pointer_type
        data() const noexcept
        {
            return m_data.data();
        }

        /// <!-- description -->
        ///   @brief Returns true if the file failed to be mapped, false
        ///     otherwise.
        ///   @include ifmap/example_ifmap_empty.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns true if the file failed to be mapped, false
        ///     otherwise.
        ///
        [[nodiscard]] constexpr bool
        empty() const noexcept
        {
            return m_data.empty();
        }

        /// <!-- description -->
        ///   @brief Returns !empty()
        ///   @include ifmap/example_ifmap_operator_bool.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns !empty()
        ///
        [[nodiscard]] constexpr explicit operator bool() const noexcept
        {
            return !this->empty();
        }

        /// <!-- description -->
        ///   @brief Returns the number of bytes in the file being
        ///     mapped.
        ///   @include ifmap/example_ifmap_size.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns the number of bytes in the file being
        ///     mapped.
        ///
        [[nodiscard]] constexpr size_type const &
        size() const noexcept
        {
            return m_data.size();
        }

        /// <!-- description -->
        ///   @brief Returns the max number of bytes the BSL supports.
        ///   @include ifmap/example_ifmap_max_size.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns the max number of bytes the BSL supports.
        ///
        [[nodiscard]] static constexpr size_type
        max_size() noexcept
        {
            return to_umax(size_type::max());
        }

        /// <!-- description -->
        ///   @brief Returns the number of bytes in the file being
        ///     mapped.
        ///   @include ifmap/example_ifmap_size_bytes.hpp
        ///
        /// <!-- inputs/outputs -->
        ///   @return Returns the number of bytes in the file being
        ///     mapped.
        ///
        [[nodiscard]] constexpr size_type const &
        size_bytes() const noexcept
        {
            return m_data.size();
        }
    };
}

#endif
