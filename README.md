# Simple Unix Utilities in C

This repository contains a collection of simple Unix utility programs implemented in C. These programs mimic the basic functionality of common command-line tools found in Linux/Unix systems.

## Utilities

The following utilities are included in this repository:

- **`my_cp`**: Copies a file from a source location to a destination location, similar to the `cp` command.
- **`my_echo`**: Prints its command-line arguments to standard output, separated by spaces, followed by a newline.
- **`my_pwd`**: Prints the current working directory to standard output, mimicking the `pwd` command.
- **`my_mv`**: Moves a file from a source location to a destination location. If the rename operation fails (e.g., across different filesystems), it falls back to copying and deleting the original file.

## Compilation

To compile the utilities, use the following commands with GCC:

```bash
gcc my_cp.c -o my_cp
gcc my_echo.c -o my_echo
gcc my_pwd.c -o my_pwd
gcc mv_command.c -o my_mv
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

## Error Handling and Exit Codes

| Exit Code | Description |
|-----------|------------|
|  0        | Success |
|  1        | General failure (replaces negative exit codes) |
|  2        | Incorrect number of arguments |
|  3        | Error opening source file |
|  4        | Error opening destination file |
|  5        | Error writing to destination file |
|  6        | Incomplete write to destination file |
|  7        | Error reading from source file |
|  8        | Error deleting source file after move |

## Limitations

- These utilities do not handle symbolic links, directories, or special file types.
- No interactive prompts (e.g., confirmation before overwriting files).
- Minimal error checking (e.g., disk space availability is not verified before writing).

## Future Improvements

- Add support for directories in `my_cp` and `my_mv`.
- Implement `-r` (recursive) option for `my_cp`.
- Improve error handling with more descriptive messages using `perror()`.
- Use standard `PATH_MAX` from `<limits.h>` in `my_pwd`.
- Add support for additional options like `-i` (interactive) and `-v` (verbose).

---

This repository serves as a foundational exercise in system programming and file manipulation using C. Feel free to contribute or suggest improvements!
