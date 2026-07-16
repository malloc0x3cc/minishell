*This project has been created as part of the 42 curriculum by gahubert, madelwau*

Minishell
=========
Description
-----------
**Minishell** is the first group project of the 42 curriculum. As suggested by it's name, the goal of this project is to write a minimalist implementation of a command-line interpreter inspired by Bash. The primary goal of this project is to explore the underlying mechanisms of process creation, lifecycle management, file descriptor manipulations, and signal handling within a Unix environment. 

This implementation focuses exclusively on the mandatory requirements of the project, establishing a reliable, leak-free, and robust foundation for basic shell operations.

### Core Features (Mandatory Part)
- **Interactive Prompt:** Displays a clean visual prompt while waiting for new command inputs.
- **Command History:** Full tracking and navigation of previously executed commands using the `readline` library history tools.
- **System Execution:** Dynamically searches, locates, and launches binaries using absolute paths, relative paths, or by scanning the system's `PATH` environment variable.
- **Environment Variable Expansion:** Seamlessly expands environment variables (e.g., `$USER`) and handles the specific `$?` variable to fetch the exit status of the latest foreground execution pipeline.
- **Quote Handling:** 
  - Single quotes (`'`) literalize every character contained inside them.
  - Double quotes (`"`) literalize characters but retain the expansion properties of the `$` token.
- **Redirections:**
  - Input redirection (`<`)
  - Output redirection (`>`) in overwrite mode
  - Append output redirection (`>>`)
  - Here-doc (`<<`) with a custom delimiter string (does not update command history).
- **Pipelines (`|`):** Connects the standard output of a command to the standard input of the next command sequentially using Unix pipes.
- **Signal Handling:** Replicating Bash's response layout to key interactive interrupts:
  - `Ctrl-C`: Displays a fresh prompt on a new line.
  - `Ctrl-D`: Cleanly exits the shell session.
  - `Ctrl-\`: Performs no action.
- **Built-in Utilities:** Custom native implementations of standard routines:
  - `echo` (supporting the `-n` option)
  - `cd` (accepting relative or absolute target paths)
  - `pwd` (with no options)
  - `export` (with no options)
  - `unset` (with no options)
  - `env` (with no arguments or options)
  - `exit` (with no options)

Instructions
-----------
### Compilation
The project includes a `Makefile` configured to compile the source code using `cc` along with strict compiler warnings (`-Wall -Wextra -Werror`). 

To compile the executable, run:
```bash
make
```
- `make` or `make all`: Compiles the core minishell executable.
- `make clean`: Removes intermediate object files (.o).
- `make fclean`: Clears object files along with the final minishell binary file.
- `make re`: Triggers a full clean re-compilation of the entire project scope.

### Launching the Shell
Once compilation completes successfully, launch the shell interface directly from your system terminal:
```bash
./minishell
```

Ressources
----------
Ai Usage
--------
