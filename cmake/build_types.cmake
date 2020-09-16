#
# Copyright (C) 2020 Assured Information Security, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

include(${CMAKE_CURRENT_LIST_DIR}/flags.cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Release)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_BUILD_TYPE DEBUG)
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL RELEASE AND
   NOT CMAKE_BUILD_TYPE STREQUAL DEBUG AND
   NOT CMAKE_BUILD_TYPE STREQUAL CLANG_TIDY AND
   NOT CMAKE_BUILD_TYPE STREQUAL ASAN AND
   NOT CMAKE_BUILD_TYPE STREQUAL UBSAN AND
   NOT CMAKE_BUILD_TYPE STREQUAL CODECOV)
    message(FATAL_ERROR "Unknown CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS_RELEASE "${BSL_FLAGS_RELEASE}")
string(REPLACE ";" " " CMAKE_LINKER_FLAGS_RELEASE "${BSL_FLAGS_RELEASE}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS_DEBUG "${BSL_FLAGS_DEBUG}")
string(REPLACE ";" " " CMAKE_LINKER_FLAGS_DEBUG "${BSL_FLAGS_DEBUG}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS_CLANG_TIDY "${BSL_FLAGS_CLANG_TIDY}")
string(REPLACE ";" " " CMAKE_LINKER_FLAGS_CLANG_TIDY "${BSL_FLAGS_CLANG_TIDY}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS_ASAN "${BSL_FLAGS_ASAN}")
string(REPLACE ";" " " CMAKE_LINKER_FLAGS_ASAN "${BSL_FLAGS_ASAN}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS_UBSAN "${BSL_FLAGS_UBSAN}")
string(REPLACE ";" " " CMAKE_LINKER_FLAGS_UBSAN "${BSL_FLAGS_UBSAN}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS_CODECOV "${BSL_FLAGS_CODECOV}")
string(REPLACE ";" " " CMAKE_LINKER_FLAGS_CODECOV "${BSL_FLAGS_CODECOV}")
