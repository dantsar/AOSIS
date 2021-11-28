; test interrupt handling for a division by zero
global div_zero
div_zero:
    mov edx, 0
    mov eax, 250
    mov ecx, 0
    div ecx
    ret