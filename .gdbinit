# assembly settings
set disassembly-flavor intel
set disassemble-next-line on
show disassemble-next-line

# layout 
layout split

# -------------------------------------------------------- #
## macros 
# print top 8 words from the stack
define xsp 
    x/8xw $sp 
end 

# print top n words from the stack
define sp 
    x/$arg0xw $sp
end

# print eflags in binary
define pe 
    p/t $eflags
end

# reconnect to qemu
define re
    target remote localhost:1234
end

# -------------------------------------------------------- #

# remote debugging
target remote localhost:1234

# location of object files
dir kernel

# set breakpoints
break kmain
