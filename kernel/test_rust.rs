#![no_main]
#![no_std]

use core::panic::PanicInfo;

#[panic_handler]
fn panicing(_info: &PanicInfo) -> ! {
    loop {}
}

#[no_mangle]
extern "C" fn add_nums(n1 : u32, n2 : u32) -> u32 {
    return n1 + n2;
}
