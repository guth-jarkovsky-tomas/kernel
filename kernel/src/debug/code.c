// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <debug/code.h>
#include <lib/print.h>

/** Dump function code at given address.
 *
 * Generally, the output should look like disassembly without
 * mnemonics part.
 *
 * @param name Function name to print in header.
 * @param address Function address.
 * @instruction_count How many instructions to print.
 */
void debug_dump_function(const char* name, uintptr_t address, size_t instruction_count) {
	//get pointer to address
	int* a = (int*)address;
	//print header
    printk("%x <%s>:", address, name);
    for (size_t i = 0; i < instruction_count; i++) {
		//I created a new print output for printing out number in correct formatting
        printk("%x:        %q", a, *a);
        a++;
    }
}

