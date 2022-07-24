#include <kernel.h>
#include <memory/pmm.h>
#include <memory/multiboot.h>
#include <terminal/tty.h>

#include <stdint.h>
#include <stddef.h>

extern uint32_t kernel_start_address;
extern uint32_t kernel_end_address;

size_t kernel_size;
pmm_t pmm = {
	.mem_ranges = 0,
	.total_mem = 0
};

void pmm_init(multiboot_info_t *mbt)
{
	// check the sixth bit of the flags to see 
	//if grub provided a valid memory map
	if (!((mbt->flags >> 6) & 0x01)) {
		panic("GRUB provided invalid memory map");
	}

	// debugging info
	kprintf("MMAP_ADDR: 0x%x\n", mbt->mmap_addr);
	kprintf("MMAP_LENGTH: %d\n", mbt->mmap_length);
	kprintf("mmm_t size: %d\n", sizeof(multiboot_memory_map_t));

	uint8_t *last_mem_addr;
	multiboot_memory_map_t *entry;
	for (size_t i = 0; i < mbt->mmap_length;
		 i += sizeof(multiboot_memory_map_t)) {

		entry = (multiboot_memory_map_t *) (mbt->mmap_addr + i);

		kprintf("---\n");

		if (entry->type != MULTIBOOT_MEMORY_AVAILABLE) {
			continue;
		}

		// more debugging info
		kprintf("\tsize: %d\n"
				"\taddr: 0x%x %x\n"
				"\tlen: 0x%x %x\n",
				entry->size,
				entry->addr_high, entry->addr_low,
				entry->len_high, entry->len_low);
		kprintf("\ttype %d\n", entry->type);
		
		/* ---- */

		if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
			uint64_t temp = ((uint64_t) entry->len_high << 32) | ((uint64_t) entry->len_low);
			pmm.total_mem += temp - (temp % 4096);
			pmm.mem_ranges++;

			// last memory address
			last_mem_addr = (uint8_t *) (entry->addr_low + entry->len_low);
		}

	}

    kernel_size = &kernel_end_address - &kernel_start_address;

	// debugging info
	kprintf("available memory: 0x%x %x\n", ((uint32_t) (pmm.total_mem >> 32)), (uint32_t)pmm.total_mem);
	kprintf("Kstart: 0x%x\nKend: 0x%x\n", &kernel_start_address, &kernel_end_address);
	kprintf("Kernel Size: %d bytes\n", kernel_size);

	/* --- */

	kprintf("last mem addr: 0x%x\n", last_mem_addr);

	size_t bitmap_size = (pmm.total_mem / 4096) / 8;
	pmm.bmap.index = last_mem_addr - bitmap_size;


	kprintf("done!!\n");
	
	// for(;;);
}


/* 

move the page frame bit map to the beginning of the memory
	but check that it fits contiguously
  maybe I should make it restricted/inaccessible, rather restricted

*/





/* 
    create a contiuous range for the memory map
    segment the range into 4k pages
    create a bit map of all the pages

    create an alloc and free api

    reserve the memory used by the kernel
*/

/* 
	reserve bit map in the top of the memory
		// edge case what if the memory range is not enough to populate bitmap
			e.g. the last memory range is 1 byte

	right before the bitmap, 
	reserve the memory ranges

	finish initalizeing the bitmap
		clear everything

	reserve bits for the kernel
*/
