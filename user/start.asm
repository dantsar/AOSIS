extern main

section .text

_start:
    jmp main

    jmp $   ; loop in place
