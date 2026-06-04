*This project has been created as part of the 42 curriculum by madelwau.*

Libft
=====
Description
-----------
Libft is a foundational project at 42. The goal is to create a personal C library re-implementing standard C functions (from libc) and adding useful custom functions. This library serves as a Swiss-Army knife for all future C projects in the curriculum.

My version of Libft goes beyond the mandatory and bonus requirements by integrating additionnal functions as well as two other major 42 projects:
- [ft_printf](https://github.com/malloc0x3cc/ft_printf): A custom implementation of the printf function.
- [get_next_line (GNL)](https://github.com/malloc0x3cc/get-next-line): A function that reads a line from a file descriptor.

### Features
The library is organized into several modules:
- Libc Functions: Re-implementations of standard functions like strlen, memcpy, atoi, etc.
- Linked Lists: A complete set of functions to manage dynamic lists.
- GNL Integration: Efficiently read text from files or standard input.
- Printf Integration: Formatted output handling multiple conversions (%s, %d, %x, etc.).
- Additional Functions: Personal additions, all listed in `inc/my.h`.

Library Structure:
```text
.
├── inc/                    # Header files
├── src/
│   ├── gnl/                # get_next_line source code
│   ├── is/                 # Boolean checks (isalpha, isdigit, etc.)
│   ├── lst/                # Linked list manipulation (bonus part)
│   ├── mem/                # Memory management (memset, calloc, etc.)
│   ├── mlx/                # Graphics-related functions for MiniLibX (put_pixel)
│   ├── printf/             # ft_printf source code
│   ├── put/                # File descriptor output (putchar, putstr)
│   ├── str/                # String manipulation (split, join, substr)
│   └── to/                 # Conversions (atoi, itoa, atof, etc.)
├── Makefile                # Multi-module compilation script
└── README.md
```

Instructions
-------------
### Compilation
The library is compiled into a libft.a file using the provided Makefile.

To compile the whole library (including mandatory, bonus, ft_printf, get_next_line and additionnal functions):
```bash
make
```
To clean object files:
```bash
make clean
```
To remove all generated files (objects and the .a library):
```bash
make fclean
```
To re-compile everything:
```bash
make re
```

### Usage
To use this library in your own C project, include the main header and link the library during compilation:

1.  Include the header in your code:
    ```c
    #include "libft.h"
    ```
2.  Compile your project with the library:
    ```bash
    cc main.c -L. -lft -o my_program
    ```

Resources
---------
### Documentation
- [Man 7 - Libc](https://man7.org/linux/man-pages/dir_section_3.html): Official documentation for standard C functions.
- [Learn-C.org](https://www.learn-c.org/): For linked lists and memory management basics.

AI Usage Disclosure
-------------------
In accordance with the 42 AI policy:
- **Brainstorming**: AI was used to discuss the optimal directory structure (separating `src/str`, `src/mem`, etc.) for better maintainability.
- **Documentation**: This README was structured with AI assistance to ensure all subject requirements were met.
