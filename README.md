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
