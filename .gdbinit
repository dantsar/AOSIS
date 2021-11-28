# assembly settings
set disassembly-flavor intel
set disassemble-next-line on
show disassemble-next-line

# layout 
layout split

# macros 
define xsp 
    x/8xw $sp 
end 

# remote debugging
target remote localhost:1234

# location of object files
dir kernel

# set breakpoints
break kmain
