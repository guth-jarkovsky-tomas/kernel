// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <adt/list.h>
#include <debug.h>
#include <drivers/printer.h>
#include <lib/print.h>
#include <lib/stdarg.h>
#include <types.h>

//struct for lists
typedef struct {
	link_t link;
} paw_t;

void print_signed_int(int num);
void print_int(int num);
void print_unint(unsigned int num);
void print_hex(unsigned int num);
void print_hex_eight(uintptr_t p);
void print_hex_big(unsigned int num);
char hex_digit(int v);
void print_p(uintptr_t p);
void printp(const char* format, ...);

//prints negative ints
void print_signed_int(int num) {
	if (num < 0) {
		printer_putchar('-');
	}
	print_int(num);
}

//prints positive ints
void print_int(int num) {
	if (num == 0) {
		return;
	}
	int j = num % 10;
	print_int(num / 10);
	if (j < 0) {
		j = -j;
	}
	char c = j + '0';
	printer_putchar(c);
}

//prints unsigned positive ints
void print_unint(unsigned int num) {
	if (num == 0) {
		return;
	}
	int j = num % 10;
	print_unint(num / 10);
	if (j < 0) {
		j = -j;
	}
	char c = j + '0';
	printer_putchar(c);
}

//prints ints in hex 
void print_hex(unsigned int num) {
	if (num == 0) {
		return;
	}
	int j = num % 16;
	print_hex(num / 16);
	if (j > 9) {
		j = j + 39;
	}
	char c = j + '0';
	printer_putchar(c);
}

//print ints in hex, when there should be 8 characters for each 
void print_hex_eight(uintptr_t p) {
    int i;
	for (i = (sizeof(p) << 3) - 4; i >= 0; i -= 4) {
		printer_putchar(hex_digit((p >> i) & 0xf));
	}
}

//prints int int hex for capital letters
void print_hex_big(unsigned int num) {
	if (num == 0) {
		return;
	}

	int j = num % 16;
	print_hex_big(num / 16);
	if (j > 9) {
		j = j + 7;
	}
	char c = j + '0';
	printer_putchar(c);
}

//helping function for writing hex numbers
char hex_digit(int v) {
	if (v >= 0 && v < 10)
		return '0' + v;
	else
		return 'a' + v - 10;
}

//print hex address
void print_p(uintptr_t p)
{
	printer_putchar('0');
	printer_putchar('x');
	int i;
	for (i = (sizeof(p) << 3) - 4; i >= 0; i -= 4) {
		printer_putchar(hex_digit((p >> i) & 0xf));
	}
}

/**
* additional print for printing lists with %p
*/
void printp(const char* format, ...) {
	va_list argp;
	va_start(argp, format);
	while (*format != '\0') {
		if (*format == '%') {
			format++;
			if (*format == '%') {
				printer_putchar('%');
			}

			else if (*format == 'p') {
				uintptr_t p = va_arg(argp, uintptr_t);
				print_p(p);
			}
		}
		else {
			printer_putchar(*format);
		}
		format++;
	}
	va_end(argp);
}


/** Prints given formatted string to console.
*
* @param format printf-style formatting string.
*/
void printk(const char* format, ...) {
	va_list argp;
	va_start(argp, format);
	while (*format != '\0') {
		if (*format == '%') {
			format++;
			if (*format == '%') {
				printer_putchar('%');

			}
			else if (*format == 'c') {
				char c = va_arg(argp, int);
				printer_putchar(c);
			}
			else if (*format == 'q') {
				uintptr_t p = va_arg(argp, uintptr_t);
				print_hex_eight(p);
			}
			else if (*format == 'u') {
				unsigned int u = va_arg(argp, unsigned int);
				if (u==0)
				{
					printer_putchar('0');
				}
				print_unint(u);
			}

			else if (*format == 'd') {
				int int_to_print = va_arg(argp, int);
				if (int_to_print == 0) {
					printer_putchar('0');
				}
				print_signed_int(int_to_print);
			}

			else if (*format == 'X') {
				unsigned int hex_to_print = va_arg(argp, int);
				print_hex_big(hex_to_print);
			}

			else if (*format == 'x') {
				unsigned int hex_to_print = va_arg(argp, int);
				print_hex(hex_to_print);
			}

			else if (*format == 's') {
				char* s = va_arg(argp, char*);
				while (*s != '\0') {
					printer_putchar(*s);
					s++;
				}
			}

			else if (*format == 'p') {
				format++;
				if (*format == 'L') {
					printer_putchar('[');
					list_t* p = va_arg(argp, list_t*);
					printp("%p", p);
					int i = 0;
					list_foreach(*p, paw_t, link, it) {
						i++;
					}
					if (i == 0) {
						printer_putchar('e');
						printer_putchar('m');
						printer_putchar('p');
						printer_putchar('t');
						printer_putchar('y');
						printer_putchar(']');
					}
					else
					{
						print_int(i);
						printer_putchar(':');
						printer_putchar(' ');
						int j = 0;
						list_foreach(*p, paw_t, link, it) {
							j++;
							if (j == i)
							{
								printer_putchar(']');
								printp("%p", it);
							}
							else
							{
								printer_putchar('-');
								printp("%p", it);
							}
						}
					}

				}
				else
				{
					printer_putchar(*format);
					uintptr_t p = va_arg(argp, uintptr_t);
					print_p(p);
				}

			}

		}
		else {
			printer_putchar(*format);
		}
		format++;
	}
	va_end(argp);
	printer_putchar('\n');
}

/** Prints given string to console, terminating it with newline.
*
* @param s String to print.
*/
void puts(const char* s) {
	while (*s != '\0') {
		printer_putchar(*s);
		s++;
	}
	printer_putchar('\n');
}


