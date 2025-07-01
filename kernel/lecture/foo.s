_Z3foov:
    push    rbp
    mov     rbp, rsp

    ; int i = 42;
    mov     dword ptr [rbp - 4], 42

    ; int* p = &i;
    lea     rax, [rbp - 4]
    mov     qword ptr [rbp - 16], rax

    ; int r1 = *p;
    mov     rax, qword ptr [rbp - 16]
    mov     ecx, dword ptr [rax]
    mov     dword ptr [rbp - 20], ecx

    ; *p = 1;
    mov     rax, qword ptr [rbp - 16]
    mov     dword ptr [rax], 1

    ; int r2 = i;
    mov     ecx, dword ptr [rbp - 4]
    mov     dword ptr [rbx - 24], ecx

    ;  uintptr_t addr = reinterpret_cast<uintptr_t>(p);
    mov     rax, qword ptr [rbp - 16]
    mov     qword ptr [rbp - 32], rax

    ; int* q = reinterpret_cast<int*>(addr);
    mov     rax, qword ptr [rbp - 32]
    mov     qword ptr [rbp - 40], rax

    pop     rbp
    ret