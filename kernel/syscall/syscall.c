#include <stdint.h>

#include <interrupt/interrupt.h>
#include <syscall/syscall.h>
#include <terminal/tty.h>

void syscall_init(void)
{
    idt_handlers[0x80] = syscall_handler;
}

void syscall_handler()
{
    kprintf("Welcome to syscalls\n");
}
