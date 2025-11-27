/*
 * Copyright (c) 2016 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
