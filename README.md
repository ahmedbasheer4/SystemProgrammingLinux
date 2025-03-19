
# Simple Unix Utilities in C

This repository contains a collection of simple Unix utility programs implemented in C. These programs mimic the basic functionality of common command-line tools found in Linux/Unix systems.

---

## Table of Contents
- [Requirements](#requirements)
- [Utilities](#utilities)
- [Compilation](#compilation)
  - [Compiling with GCC](#compiling-with-gcc)
  - [Compiling with Makefile](#compiling-with-makefile)
- [Usage Examples](#usage-examples)
  - [my_cp - Copy a file](#my_cp---copy-a-file)
  - [my_echo - Print text](#my_echo---print-text)
  - [my_pwd - Print working directory](#my_pwd---print-working-directory)
  - [my_mv - Move a file](#my_mv---move-a-file)
  - [myFemtoShell - A simple shell](#myfemtoshell---a-simple-shell)
  - [myPicoShell - An extended shell](#mypicoshell---an-extended-shell)
  - [myNanoShell - A more advanced shell](#mynanoshell---a-more-advanced-shell)
  - [myMicroShell - A minimalistic shell](#mymicroshell---a-minimalistic-shell)
- [Future Improvements](#future-improvements)

---

## Requirements

Before compiling and running these utilities, ensure you have the following installed:

- **Linux OS** (Tested on Ubuntu, Debian, and CentOS)
- **GCC Compiler**  
  Install it using:
  ```bash
  sudo apt install gcc   # For Debian-based systems
  sudo yum install gcc   # For Red Hat-based systems
  ```

---

## Compilation

### **Compiling with GCC**
To compile the utilities manually, use the following commands:

```bash
gcc my_cp.c -o my_cp
gcc my_echo.c -o my_echo
gcc my_pwd.c -o my_pwd
gcc my_mv.c -o my_mv
gcc femto_shell.c -o myFemtoShell
gcc pico_shell.c -o myPicoShell
gcc nano_shell.c -o myNanoShell
gcc micro_shell.c -o myMicroShell
```

### **Compiling with Makefile**
For easier compilation, you can use a `Makefile`.  
Here’s an example:

```Makefile
all: my_cp my_echo my_pwd my_mv myFemtoShell myPicoShell myNanoShell myMicroShell

my_cp: my_cp.c
	gcc my_cp.c -o my_cp

my_echo: my_echo.c
	gcc my_echo.c -o my_echo

my_pwd: my_pwd.c
	gcc my_pwd.c -o my_pwd

my_mv: mv_command.c
	gcc mv_command.c -o my_mv

myFemtoShell: femto_shell.c
	gcc femto_shell.c -o myFemtoShell

myPicoShell: pico_shell.c
	gcc pico_shell.c -o myPicoShell

myNanoShell: nano_shell.c
	gcc nano_shell.c -o myNanoShell

myMicroShell: micro_shell.c
	gcc micro_shell.c -o myMicroShell

clean:
	rm -f my_cp my_echo my_pwd my_mv myFemtoShell myPicoShell myNanoShell myMicroShell
```

Run:
```bash
make
```
To remove compiled binaries, run:
```bash
make clean
```

---

## Usage Examples

### `my_cp` - Copy a file
```bash
./my_cp source.txt destination.txt
```
**Expected output:**
```
File copied successfully
```

### `my_echo` - Print text
```bash
./my_echo Hello, world!
```
**Expected output:**
```
Hello, world!
```

### `my_pwd` - Print working directory
```bash
./my_pwd
```
**Expected output:**
```
/home/user/projects
```

### `my_mv` - Move a file
```bash
./my_mv source.txt new_location.txt
```
**Expected output:**
```
File moved successfully.
```

---

### `myFemtoShell` - A simple shell
```bash
./myFemtoShell
```
**Example Session:**
```
MiniShell > echo Hello my shell
Hello my shell
MiniShell > ls
Invalid command
MiniShell > exit
Good Bye :)
```

---

### `myPicoShell` - An extended shell
```bash
./myPicoShell
```
**Example Session:**
```
PicoShell > echo Hello PicoShell
Hello PicoShell
PicoShell > ls
bin  boot  dev  etc  home  lib  usr  var
PicoShell > exit
Good Bye :)
```
**Features:**
- Supports `echo` and `exit` like `myFemtoShell`.
- Executes external commands (`ls`, `date`, etc.).
- Uses `execvp` for command execution.

---

### `myNanoShell` - A more advanced shell
```bash
./myNanoShell
```
**Example Session:**
```
NanoShell > echo Hello NanoShell
Hello NanoShell
NanoShell > ls | grep txt
current_time.txt
NanoShell > exit
Good Bye :)
```
**Features:**
- Executes external commands like `myPicoShell`.
- Implements input/output redirection (`>`, `<`).
- Supports command chaining with piping (`|`).

---

### `myMicroShell` - A minimalistic shell
```bash
./myMicroShell
```
**Example Session:**
```
MicroShell > echo Hello MicroShell
Hello MicroShell
MicroShell > ls
bin  boot  dev  etc  home  lib  usr  var
MicroShell > exit
Good Bye :)
```
**Features:**
- Supports basic command execution using `execvp`.
- Minimal error handling: displays an error message for unknown commands.

---

## Future Improvements

- **Enhance `my_cp` and `my_mv`**:  
  - Add support for directories.
  - Implement `-r` (recursive) option.
- **Improve error handling**:  
  - Use `perror()` for detailed error messages.
- **Optimize `myNanoShell`**:  
  - Add support for **aliasing**.
  - Implement **command history (`Up Arrow`)**.
  - Improve **redirection handling**.
  - Extend support for **job control (`fg`, `bg`)**.
  - Add **script execution** (`.sh` support).

---
