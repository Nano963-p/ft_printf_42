# ft_printf

A custom implementation of the C standard library `printf` function, built as part of the [42 School](https://42.fr) curriculum.

## Overview

`ft_printf` replicates the core behavior of `printf`, parsing a format string and dispatching to dedicated output helpers for each supported conversion specifier. It returns the total number of characters written to standard output, or `-1` on error — matching the interface of the real `printf`.

## Supported Conversions

| Specifier | Description |
|-----------|-------------|
| `%c` | Single character |
| `%s` | String (prints `(null)` if pointer is NULL) |
| `%d` / `%i` | Signed decimal integer |
| `%u` | Unsigned decimal integer |
| `%x` | Unsigned hexadecimal (lowercase) |
| `%X` | Unsigned hexadecimal (uppercase) |
| `%p` | Pointer address (e.g. `0x7ffd...`, or `(nil)` for NULL) |
| `%%` | Literal `%` character |

## File Structure

```
ftprint/
├── ft_printf.h       # Header — includes and function prototypes
├── ft_printf.c       # Core parser and dispatcher
├── ft_putchar.c      # Writes a single character
├── ft_putstr.c       # Writes a string
├── ft_putnbr.c       # Writes a signed integer
├── ft_putunbr.c      # Writes an unsigned integer
├── ft_puthex.c       # Writes an unsigned int as hex (upper or lower)
├── ft_putptr.c       # Writes a pointer address with 0x prefix
└── Makefile
```

## Building

```bash
make        # Compiles object files and archives into libftprintf.a
make clean  # Removes object files
make fclean # Removes object files and libftprintf.a
make re     # Full rebuild (fclean + all)
```

The Makefile compiles with `cc -Wall -Wextra -Werror` and produces a static library: **`libftprintf.a`**.

## Usage

Link `libftprintf.a` into your project and include the header:

```c
#include "ft_printf.h"

int main(void)
{
    int len;

    len = ft_printf("Hello, %s! You are %d years old.\n", "mel-hyna", 21);
    ft_printf("Characters written: %d\n", len);
    ft_printf("Pointer: %p\n", (void *)&len);
    ft_printf("Hex: %x / %X\n", 255, 255);
    return (0);
}
```

Compile:

```bash
cc -Wall -Wextra -Werror main.c -L. -lftprintf -o my_program
```

## Implementation Notes

- The format string is walked byte-by-byte. On encountering `%`, the next character is passed to `verif_type()`, which calls the appropriate helper.
- An unrecognized specifier (e.g. `%q`) is printed literally as `%q`.
- A trailing lone `%` at the end of the format string is silently ignored (loop exits before processing it).
- `ft_putnbr` uses a `long` internally to safely handle `INT_MIN` without overflow.
- `ft_puthex` and `ft_putptr` allocate a temporary buffer on the heap for the hex digits and free it after writing.
- `write(1, "", 0)` is used as an early fd-validity check — if it returns `-1`, `ft_printf` returns `-1` immediately.

## Author

**mel-hyna** — 42 School student