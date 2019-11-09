// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <debug/mm.h>
#include <main.h>

/** Probe available base physical memory.
 *
 * Do not check for non-continuous memory blocks or for memory available via
 * TLB only.
 *
 * @return Amount of memory available in bytes.
 */
size_t debug_get_base_memory_size(void) {
    char* volatile ptr =(char*)&_kernel_end +1;
    int size = 0;
    int volatile tester;
    while (true) {
        tester = *ptr;
        *ptr = tester + 1;
        if (*ptr == tester)
		{
			ptr = ptr - 1024;
			size = size - 1024;
			while (true)
			{
				tester = *ptr;
				*ptr = tester + 1;
				if (*ptr == tester)
				{
					return size;
				}
				*ptr = tester;
				ptr++;
				size++;
			}
		}
        *ptr = tester;
        ptr = ptr + 1024;
        size += 1024;
    }
    
}