#include <cstdint>
void foo() {
    int i = 42;
    int* p = &i;
    int r1 = *p;
    *p = 1;
    int r2 = i;
    uintptr_t addr = reinterpret_cast<uintptr_t>(p);
    int* q = reinterpret_cast<int*>(addr);
}