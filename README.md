# Simple Unix Utilities in C

This repository contains a collection of simple Unix utility programs implemented in C. These programs mimic the basic functionality of common command-line tools found in Linux/Unix systems.

## Utilities

The following utilities are included in this repository:

- **`my_cp`**: Copies a file from a source location to a destination location, similar to the `cp` command.
- **`my_echo`**: Prints its command-line arguments to standard output, separated by spaces, followed by a newline.
- **`my_pwd`**: Prints the current working directory to standard output, mimicking the `pwd` command.
- **`my_mv`**: Moves a file from a source location to a destination location. If the rename operation fails (e.g., across different filesystems), it falls back to copying and deleting the original file.
- **`myFemtoShell`**: A simple shell that supports `echo` and `exit` commands.
- **`myPicoShell`**: An extended shell that supports `echo`, `exit`, and additional basic command execution.
- **`myNanoShell`**: A more advanced shell that supports command execution, input/output redirection, and command chaining.

## Compilation

To compile these utilities, ensure you have `gcc` installed. You can install it using:
```bash
sudo apt install gcc   # For Debian-based systems
sudo yum install gcc   # For Red Hat-based systems
```
Alternatively, using a `Makefile` can simplify compilation for frequent use.

To compile the utilities, use the following commands with GCC:

```bash
gcc my_cp.c -o my_cp
gcc my_echo.c -o my_echo
gcc my_pwd.c -o my_pwd
gcc mv_command.c -o my_mv
gcc femto_shell.c -o myFemtoShell
gcc pico_shell.c -o myPicoShell
gcc nano_shell.c -o myNanoShell
```

## Usage Examples

### `my_cp` - Copy a file
```bash
./my_cp source.txt destination.txt
```
**Expected output:**
```
File copied successfully
```

**Error case:**
```bash
./my_cp non_existing.txt destination.txt
```
**Output:**
```
Error opening source file 'non_existing.txt'
```

---

### `my_echo` - Print text
```bash
./my_echo Hello, world!
```
**Expected output:**
```
Hello, world!
```

**Edge case:**
```bash
./my_echo
```
**Output:**
```

```

---

### `my_pwd` - Print working directory
```bash
./my_pwd
```
**Expected output (example):**
```
/home/user/projects
```

---

### `my_mv` - Move a file
```bash
./my_mv source.txt new_location.txt
```
**Expected output:**
```
File moved successfully.
```

**Cross-filesystem move:**
```bash
./my_mv source.txt /mnt/external_drive/destination.txt
```
**Output:**
```
Cross device operation, copying and deleting...
File moved successfully (copied & deleted).
```

**Error case:**
```bash
./my_mv non_existing.txt destination.txt
```
**Output:**
```
Error opening source file 'non_existing.txt'
```

---

### `myFemtoShell` - A simple shell
```bash
./myFemtoShell
```
**Example Session:**
```
Welcome to MiniShell! Type 'exit' to quit.
MiniShell > echo Hello my shell
Hello my shell
MiniShell > echo What can you do?
What can you do?
MiniShell > ls
Invalid command
MiniShell > exit
Good Bye :)
```

---

### `myPicoShell` - An extended shell

**Error Handling:**
- If an invalid command is entered, an error message is displayed.
- Returns `1` for failed commands.
```bash
./myPicoShell
```
**Example Session:**
```
Welcome to PicoShell! Type 'exit' to quit.
PicoShell > echo Hello PicoShell
Hello PicoShell
PicoShell > ls
bin  boot  dev  etc  home  lib  usr  var
PicoShell > date
Wed Mar 19 12:34:56 UTC 2025
PicoShell > exit
Good Bye :)
```
**Features:**
- Supports `echo` and `exit` like `myFemtoShell`.
- Can execute basic external commands (`ls`, `date`, etc.).
- Uses `execvp` for command execution.

---

### `myNanoShell` - A more advanced shell

**Error Handling:**
- Displays an error message for unknown commands.
- Returns `1` for failed executions.
- Handles invalid syntax in redirection and piping.
```bash
./myNanoShell
```
**Example Session:**
```
Welcome to NanoShell! Type 'exit' to quit.
NanoShell > echo Hello NanoShell
Hello NanoShell
NanoShell > ls
bin  boot  dev  etc  home  lib  usr  var
NanoShell > date > current_time.txt
NanoShell > cat current_time.txt
Wed Mar 19 12:34:56 UTC 2025
NanoShell > ls | grep txt
current_time.txt
NanoShell > exit
Good Bye :)
```
**Features:**
- Supports command execution like `myPicoShell`.
- Implements input/output redirection (`>`, `<`).
- Supports command chaining with piping (`|`).
- Provides simple error handling: returns `1` if the command fails.

**Limitations:**
- No support for background processes (`&`).
- Lacks advanced shell scripting capabilities.
- No built-in environment variable support.

## Future Improvements

- Add support for directories in `my_cp` and `my_mv`.
- Implement `-r` (recursive) option for `my_cp`.
- Improve error handling with more descriptive messages using `perror()`.
- Use standard `PATH_MAX` from `<limits.h>` in `my_pwd`.
- Add support for additional options like `-i` (interactive) and `-v` (verbose).
- Extend `myNanoShell` with:
  - Background process handling (`&`).
  - Environment variable support (`export VAR=value`).
  - Enhanced scripting capabilities.
  - Command history using arrow keys.
  - Support for command aliasing.
  - Improved handling of complex command chaining and redirections.

---

This repository serves as a foundational exercise in system programming and file manipulation using C. Feel free to contribute or suggest improvements!

