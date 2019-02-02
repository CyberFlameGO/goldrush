#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// This is a rather depressing state of affairs. We don't have any math functions
// available at this point. Instead we'll need to try to make a guess by using
// a log base 10 function that returns integer results and then divide by reasonable
// approximations for whatever base we need.
//
// TODO: Import PDLIBM because I don't want to write the libm whenever I get to that
// point.
static int _int_log10(int n)
{
	int result = 0;
	do {
		result++;
	} while (n /= 10);
	return result;
}

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
			format++;
			int i = va_arg(parameters, int);
			// note: potential_size is a guess as to the buffer size - always
			// find the string size afterwards
			int potential_size = _int_log10(i) + 1;
			if (maxrem < potential_size) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			char str[potential_size];
			itoa(i, str, 10);
			if (!print(str, strlen(str)))
				return -1;
			written += potential_size;
		} else if (*format == 'x') {
			format++;
			int i = va_arg(parameters, int);
			int potential_size = (_int_log10(i) * 5 / 4);
			if (maxrem < potential_size) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			char str[potential_size];
			itoa(i, str, 16);
			if (!print(str, strlen(str)))
				return -1;
			written += potential_size;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}