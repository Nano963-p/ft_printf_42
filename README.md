# 🖨️ ft_printf — 42 School Project

> **Author:** mel-hyna (`mel-hyna@student.42.fr`)
> **School:** 42 Network
> **Language:** C
> **Compiled with:** `cc -Wall -Wextra -Werror`
> **Key concept:** Variadic functions, format string parsing, type dispatching

---

## 📖 Overview

**ft_printf** is the third major project at 42 School. The goal is to re-implement the C standard library's `printf()` function from scratch — without using it — by parsing a format string and dispatching each conversion specifier (`%d`, `%s`, `%x`, etc.) to a dedicated handler.

What makes this project valuable is that it teaches you:
- How **variadic functions** work in C (`va_list`, `va_start`, `va_arg`, `va_end`)
- How to parse a format string character by character
- How to convert numbers to different bases (decimal, hexadecimal)
- How to correctly track and return a **total character count**

The output is a static library `libftprintf.a` that you will link into every future 42 project alongside libft.

---

## 📁 Project Structure

```
ft_printf_42-main/
├── ft_printf.h       ← Header: includes, all function prototypes
├── Makefile          ← Build system → produces libftprintf.a
├── ft_printf.c       ← Entry point: format string parser + specifier dispatcher
├── ft_putchar.c      ← Handles %c
├── ft_putstr.c       ← Handles %s
├── ft_putnbr.c       ← Handles %d and %i (signed int)
├── ft_putunbr.c      ← Handles %u (unsigned int)
├── ft_puthex.c       ← Handles %x (lowercase) and %X (uppercase hex)
└── ft_putptr.c       ← Handles %p (pointer address)
```

Total: **8 files** — 1 header, 1 Makefile, 6 source files

---

## 🔨 How to Build

### Compile the library

```bash
make
```

This compiles all `.c` files into `.o` object files and archives them into `libftprintf.a`.

### Link into your own project

```bash
cc -Wall -Wextra -Werror your_program.c libftprintf.a -o your_program
```

Or if you also use libft:

```bash
cc your_program.c libftprintf.a libft.a -o your_program
```

### Makefile targets

| Target   | Description                                     |
|----------|-------------------------------------------------|
| `all`    | Build `libftprintf.a` (default)                 |
| `clean`  | Remove all `.o` object files                    |
| `fclean` | Remove `.o` files **and** `libftprintf.a`       |
| `re`     | Full rebuild (`fclean` + `all`)                 |

### Compiler flags

| Flag      | Meaning                              |
|-----------|--------------------------------------|
| `-Wall`   | Enable all common warnings           |
| `-Wextra` | Enable extra warnings                |
| `-Werror` | Treat every warning as a fatal error |

---

## 🔍 Function Reference

### Main Function

```c
int ft_printf(const char *format, ...);
```

| Parameter  | Description                                     |
|------------|-------------------------------------------------|
| `format`   | Format string, e.g. `"Hello %s, you are %d!"` |
| `...`      | Variadic arguments matching each `%` specifier |

**Returns:** The total number of characters written to stdout, or `-1` on error.

This matches the behaviour of the real `printf()`.

---

### Supported Conversion Specifiers

| Specifier | Handler function             | Description                                                      |
|-----------|------------------------------|------------------------------------------------------------------|
| `%c`      | `ft_putchar(char c)`         | Prints a single character                                        |
| `%s`      | `ft_putstr(char *s)`         | Prints a string; prints `(null)` if pointer is `NULL`           |
| `%d`      | `ft_putnbr(int d)`           | Prints a signed decimal integer                                  |
| `%i`      | `ft_putnbr(int d)`           | Same as `%d` — both route to `ft_putnbr`                        |
| `%u`      | `ft_putunbr(unsigned int n)` | Prints an unsigned decimal integer                               |
| `%x`      | `ft_puthex(n, 0)`            | Prints unsigned int as lowercase hexadecimal (`a–f`)            |
| `%X`      | `ft_puthex(n, 1)`            | Prints unsigned int as uppercase hexadecimal (`A–F`)            |
| `%p`      | `ft_putptr(void *p)`         | Prints a pointer address as `0x...`; prints `(nil)` if `NULL`  |
| `%%`      | *(inline)*                   | Prints a literal `%` character                                   |

---

### Handler Functions

#### `ft_putchar` — `%c`

```c
int ft_putchar(char c);
```

Writes a single byte to stdout via `write(1, &c, 1)`. Returns `1` (always writes exactly one character).

---

#### `ft_putstr` — `%s`

```c
int ft_putstr(char *s);
```

Iterates over the string writing one byte at a time. Returns the number of characters written.

**NULL handling:** If `s` is `NULL`, prints the literal string `(null)` (6 characters) — matching the behaviour of the real `printf`.

---

#### `ft_putnbr` — `%d` / `%i`

```c
int ft_putnbr(int d);
```

Handles signed integers recursively. Uses a `long` internally to safely handle `INT_MIN` (-2147483648), whose absolute value overflows a regular `int`. Prints a `-` prefix for negative numbers, then recurses on the positive value. Returns the total digit count (including the `-` sign if negative) via a static `count_d()` helper.

---

#### `ft_putunbr` — `%u`

```c
int ft_putunbr(unsigned int n);
```

Prints an unsigned integer recursively (no sign handling needed). Returns the digit count via its own `count_d()` helper which correctly returns `1` for the value `0`.

---

#### `ft_puthex` — `%x` / `%X`

```c
int ft_puthex(unsigned int n, int uppercase);
```

| Parameter   | Description                                           |
|-------------|-------------------------------------------------------|
| `n`         | The unsigned integer to print in base 16             |
| `uppercase` | `0` = lowercase (`a–f`), `1` = uppercase (`A–F`)    |

Internally calls the static `put_16()` helper which builds the hex string into a heap-allocated buffer using the digit string `"0123456789abcdef"`. If `uppercase` is set, letters `a–f` are converted by subtracting 32 (ASCII offset to uppercase). The buffer is freed after writing. Returns the number of hex digits written.

---

#### `ft_putptr` — `%p`

```c
int ft_putptr(void *p);
```

Casts the pointer to `unsigned long long` to handle both 32-bit and 64-bit architectures safely. Builds the hex representation with its own `put_16()` (which operates on `unsigned long long`, unlike the one in `ft_puthex`), prepends `"0x"`, then writes the address. Returns the total character count including the `"0x"` prefix.

**NULL handling:** If `p` is `NULL`, prints `(nil)` (5 characters) and returns `5`.

---

## 🧠 How It Works — Internal Architecture

### Format string parsing (`ft_printf.c`)

```c
int ft_printf(const char *format, ...)
```

The main loop walks through `format` one character at a time:

```
for each character in format:
    if '%' and next char is not '\0':
        → advance i, call verif_type(format[i], args)
        → add return value to total
    else if not '%':
        → write the character directly
        → increment total
```

A trailing `%` with no following character is silently ignored (the loop exits before dispatching).

### Specifier dispatcher (`verif_type`)

```c
static int verif_type(char a, va_list args)
```

A chain of `if` comparisons routes each specifier to its handler. `va_arg(args, type)` extracts the next argument from the variadic list with the correct type for each specifier:

```
'%'      → write(1, "%", 1)
'd'/'i'  → ft_putnbr(va_arg(args, int))
'c'      → ft_putchar((char)va_arg(args, int))
's'      → ft_putstr(va_arg(args, char *))
'p'      → ft_putptr(va_arg(args, void *))
'u'      → ft_putunbr(va_arg(args, unsigned int))
'x'      → ft_puthex(va_arg(args, unsigned int), 0)
'X'      → ft_puthex(va_arg(args, unsigned int), 1)
unknown  → write '%' + the unknown character (2 chars)
```

Every handler returns the number of characters it wrote. These return values bubble up through `verif_type` into `ft_printf`'s `total` counter, which is ultimately returned to the caller.

### Call flow diagram

```
ft_printf("Value: %d, hex: %x\n", 42, 255)
    │
    ├── 'V','a','l','u','e',':',' '  → write directly, total += 7
    │
    ├── '%d'  → verif_type('d', args)
    │               └── ft_putnbr(42)  → writes "42", returns 2
    │           total += 2
    │
    ├── ',',' ','h','e','x',':',' '  → write directly, total += 7
    │
    ├── '%x'  → verif_type('x', args)
    │               └── ft_puthex(255, 0)  → writes "ff", returns 2
    │           total += 2
    │
    └── '\n'  → write directly, total += 1
    
    return 19
```

---

## 📋 Edge Cases Handled

| Scenario                          | Behaviour                                                        |
|-----------------------------------|------------------------------------------------------------------|
| `NULL` string (`%s`)             | Prints `(null)` — 6 characters                                   |
| `NULL` pointer (`%p`)            | Prints `(nil)` — 5 characters                                    |
| `INT_MIN` (`%d`)                 | Handled safely via `long` cast before negation                   |
| `0` with `%u`                    | Prints `0` (digit count returns 1, not 0)                        |
| `0` with `%x` / `%X`            | Prints `0`                                                       |
| `%%`                             | Prints a single literal `%`                                      |
| Unknown specifier (e.g. `%q`)   | Prints `%q` as-is (2 characters)                                 |
| Trailing `%` at end of format    | Ignored — the loop terminates before dispatching                 |
| `NULL` format string             | Returns `-1` immediately                                         |
| Write error (stdout closed)      | `write(1, "", 0)` probe at start returns `-1` early              |

---

## 🧪 Testing

### Quick manual test

```c
#include "ft_printf.h"
#include <stdio.h>

int main(void)
{
    int ret;

    ret = ft_printf("char:    [%c]\n", 'A');
    ft_printf("putchar returned: %d\n\n", ret);

    ret = ft_printf("string:  [%s]\n", "hello");
    ft_printf("putstr returned: %d\n\n", ret);

    ret = ft_printf("null s:  [%s]\n", (char *)NULL);
    ft_printf("null str returned: %d\n\n", ret);

    ret = ft_printf("int:     [%d]\n", -2147483648);
    ft_printf("putnbr returned: %d\n\n", ret);

    ret = ft_printf("uint:    [%u]\n", 4294967295u);
    ft_printf("putunbr returned: %d\n\n", ret);

    ret = ft_printf("hex lc:  [%x]\n", 255);
    ret = ft_printf("hex uc:  [%X]\n", 255);
    ft_printf("puthex returned: %d\n\n", ret);

    ret = ft_printf("ptr:     [%p]\n", (void *)0x7fff5fbff8a0);
    ret = ft_printf("null p:  [%p]\n", NULL);
    ft_printf("putptr returned: %d\n\n", ret);

    ret = ft_printf("percent: [%%]\n");
    ft_printf("percent returned: %d\n", ret);

    return (0);
}
```

Compile and run:

```bash
make
cc -Wall -Wextra -Werror test.c libftprintf.a -o test && ./test
```

### Compare against real `printf`

```c
#include <stdio.h>
#include "ft_printf.h"

int main(void)
{
    int r1, r2;

    r1 = printf("real:  [%d] [%s] [%x]\n", -42, "hi", 255);
    r2 = ft_printf("mine:  [%d] [%s] [%x]\n", -42, "hi", 255);
    printf("return values: real=%d  mine=%d\n", r1, r2);
    return (0);
}
```

### Check for memory leaks

```bash
valgrind --leak-check=full ./test
```

`ft_puthex` and `ft_putptr` both `malloc` a temporary hex buffer — these must be freed before return. Valgrind will catch any miss.

### Popular third-party testers

- [printfTester](https://github.com/Tripouille/printfTester)
- [ft_printf_tester](https://github.com/paulo-santana/ft_printf_tester)
- [francinette](https://github.com/xicodomingues/francinette)

---

## ✅ 42 Norm Compliance

This project follows the **42 Norm** (Norminette):

- Maximum **25 lines** per function
- No more than **5 variables** declared per function
- No `for` loops (only `while`)
- No assignments inside conditions
- Proper `#ifndef` header guards in `.h` files
- Complex operations (digit counting, hex conversion) are split into dedicated static helper functions

---

## 📌 Important Notes for 42 Students

- **Always check the return value.** `ft_printf` returns the number of characters written, just like the real `printf`. Your evaluator will test this.
- **`va_arg` type must match exactly.** Passing the wrong type to `va_arg` is undefined behaviour. Note that `%c` uses `va_arg(args, int)` — `char` is promoted to `int` in variadic calls, so you must extract an `int` and cast it.
- **`ft_puthex` and `ft_putptr` allocate memory.** Both call `put_16()` which uses `malloc`. Always `free` the result — Valgrind is used in evaluations.
- **`ft_putnbr` uses `long` to handle `INT_MIN`.** Negating `INT_MIN` as an `int` is undefined behaviour; the cast to `long` before negation is intentional and necessary.
- **`%p` needs `unsigned long long`.** Pointer addresses are 64-bit on modern systems. Using `unsigned int` or `unsigned long` would truncate the address on 64-bit platforms.
- **Unknown specifiers print both `%` and the character.** This matches the behaviour of the real `printf`.
- **The bonus part** (format flags: `-`, `0`, `.`, width, precision) is **not implemented** in this version.
- Do **not** use `printf`, `sprintf`, or any `<stdio.h>` function inside your implementation.

---

*Created by mel-hyna — 42 School, 2025*
