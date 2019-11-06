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
	//this pointer points at the beginning of memory space
    int* volatile p =(int*)&_kernel_end +1;
	//size counter
	int size = 0;
	//pointer for testing whether we are at the end
	int volatile test;
	
    while (true) {
        test = *p;
        *p = test + 1;
        if (*p == test)
            return (size/4); 
        *p = test;
		size++;
        p++;       
    }   
}
