#pragma GCC diagnostic ignored "-Wwrite-strings"

extern "C"
{
  #include <sark.h>
}
#include <cstdlib>
#include <new>

inline void* operator new(size_t size) noexcept {
    void *p = sark_alloc(1, size);
    while (p == nullptr) {
	rt_error(RTE_SWERR);
    }
    return p;
}

inline void operator delete(void *p) noexcept {
    sark_free(p);
}

inline void* operator new[](size_t size) noexcept {
    return operator new(size); // Same as regular new
}

inline void operator delete[](void *p) noexcept {
    operator delete(p); // Same as regular delete
}

inline void* operator new(size_t size, std::nothrow_t) noexcept {
    return sark_alloc(1, size);
}

inline void operator delete(void *p,  std::nothrow_t) noexcept {
    operator delete(p); // Same as regular delete
}

inline void* operator new[](size_t size, std::nothrow_t) noexcept {
    return sark_alloc(1, size);
}

inline void operator delete[](void *p,  std::nothrow_t) noexcept {
    operator delete(p); // Same as regular delete
}
