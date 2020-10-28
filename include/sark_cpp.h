/*
 * Copyright (c) 2017-2019 The University of Manchester
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//! \file
//! \brief Wrapper for using sark.h from C++
#pragma GCC diagnostic ignored "-Wwrite-strings"

extern "C"
{
#include <sark.h>
}
#include <cstdlib>
#include <new>

inline void* operator new (size_t size) noexcept {
    void *p = sark_alloc(1, size);
    while (p == nullptr) {
	rt_error(RTE_SWERR);
    }
    return p;
}

inline void operator delete (void *p) noexcept {
    sark_free(p);
}

inline void* operator new [](size_t size) noexcept {
    return operator new (size); // Same as regular new
}

inline void operator delete [](void *p) noexcept {
    operator delete (p); // Same as regular delete
}

inline void* operator new (size_t size, std::nothrow_t) noexcept {
    return sark_alloc(1, size);
}

inline void operator delete (void *p,  std::nothrow_t) noexcept {
    operator delete (p); // Same as regular delete
}

inline void* operator new [](size_t size, std::nothrow_t) noexcept {
    return sark_alloc(1, size);
}

inline void operator delete [](void *p,  std::nothrow_t) noexcept {
    operator delete (p); // Same as regular delete
}
