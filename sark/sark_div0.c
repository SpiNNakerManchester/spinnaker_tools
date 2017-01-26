#include <sark.h>

// Functions to handle division by 0 (simply RTE)
void __aeabi_idiv0() {
    rt_error(RTE_DIV0);
}

void __aeabi_ldiv0() {
    rt_error(RTE_DIV0);
}
