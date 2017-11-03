#pragma GCC diagnostic ignored "-Wwrite-strings"

extern "C"
{
  #include <sark.h>
}
#include <cstdlib>
#include <new>

inline void* operator new (size_t size) {
    void *ptr = sark_alloc(1, size);
    if (ptr == nullptr) {
	throw std::bad_alloc();
    }
    return ptr;
}

inline void operator delete (void *ptr) noexcept {
    if (ptr != nullptr) {
	sark_free(ptr);
    }
}

inline void* operator new [](size_t size) {
    void *ptr = sark_alloc(1, size);
    if (ptr == nullptr) {
	throw std::bad_alloc();
    }
    return ptr;
}

inline void operator delete [](void *ptr) noexcept {
    operator delete (ptr); // Same as regular delete
}

inline void* operator new (size_t size, std::nothrow_t) noexcept {
    return sark_alloc(1, size);
}

inline void operator delete (void *ptr, std::nothrow_t) noexcept {
    operator delete (ptr); // Same as regular delete
}

inline void* operator new [](size_t size, std::nothrow_t nothrow) noexcept {
    return sark_alloc(1, size);
}

inline void operator delete [](void *ptr, std::nothrow_t nothrow) noexcept {
    operator delete (ptr); // Same as regular delete
}

extern void cpp_main(void);
