#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_INPUT 256
#define MAX_ARGS 64
#define DELIMITERS " \t\r\n"

// Structure to store shell variables
typedef struct {
    char* name;
    char* value;
} ShellVar;

// Global array to store shell variables
ShellVar* shellVars = NULL;
int numShellVars = 0;
int maxShellVars = 0;

// Structure to hold redirection information
typedef struct {
    char* input_file;
    char* output_file;
    char* error_file;
} RedirectionInfo;

// Function prototypes
char** parse_input(char* input, int* argc, RedirectionInfo* redir_info);
void execute_command(char** argv, int argc, RedirectionInfo* redir_info);
int execute_builtin(char** argv, int argc);
void free_arguments(char** argv, int argc);
void add_shell_var(const char* name, const char* value);
char* get_shell_var(const char* name);
void free_shell_vars();
char* substitute_variables(char* input);
int is_valid_assignment(const char* input);
void export_variable(const char* name);
void free_redirection_info(RedirectionInfo* redir_info);

int main() {
    char input[MAX_INPUT];

    printf("Welcome to Nano Shell! Type 'exit' to quit.\n");

    while (1) {
        printf("Nano Shell Prompt > ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\nGood Bye :)\n");
            break;
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Check for variable assignment
        if (strchr(input, '=') != NULL) {
            if (is_valid_assignment(input)) {
                char* eq_ptr = strchr(input, '=');
                *eq_ptr = '\0';
                add_shell_var(input, eq_ptr + 1);
            } else {
                fprintf(stderr, "Invalid command\n");
            }
            continue;
        }

        char* substituted_input = substitute_variables(input);
        int argc;
        RedirectionInfo redir_info = {NULL, NULL, NULL};
        char** argv = parse_input(substituted_input, &argc, &redir_info);
        free(substituted_input);

        if (argc > 0) {
            if (execute_builtin(argv, argc) == 0) {
                execute_command(argv, argc, &redir_info);
            }
        }
        free_arguments(argv, argc);
        free_redirection_info(&redir_info);
    }

    free_shell_vars();
    return EXIT_SUCCESS;
}

char** parse_input(char* input, int* argc, RedirectionInfo* redir_info) {
    char** argv = (char**)malloc(sizeof(char*) * MAX_ARGS);
    if (argv == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(input, DELIMITERS);
    *argc = 0;
    int redirection_mode = 0; // 0: normal, 1: input, 2: output, 3: error

    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            redirection_mode = 1;
        } else if (strcmp(token, ">") == 0) {
            redirection_mode = 2;
        } else if (strcmp(token, "2>") == 0) {
            redirection_mode = 3;
        } else if (redirection_mode == 1) {
            redir_info->input_file = strdup(token);
            redirection_mode = 0;
        } else if (redirection_mode == 2) {
            redir_info->output_file = strdup(token);
            redirection_mode = 0;
        } else if (redirection_mode == 3) {
            redir_info->error_file = strdup(token);
            redirection_mode = 0;
        } else {
            argv[*argc] = (char*)malloc(sizeof(char) * (strlen(token) + 1));
            if (argv[*argc] == NULL) {
                perror("malloc failed");
                // Free previously allocated memory
                for (int i = 0; i < *argc; i++) {
                    free(argv[i]);
                }
                free(argv);
                exit(EXIT_FAILURE);
            }
            strcpy(argv[*argc], token);
            (*argc)++;

            if (*argc >= MAX_ARGS) {
                fprintf(stderr, "Too many arguments\n");
                break;
            }
        }

        token = strtok(NULL, DELIMITERS);
    }
    argv[*argc] = NULL;
    return argv;
}

int execute_builtin(char** argv, int argc) {
    if (strcmp(argv[0], "exit") == 0) {
        printf("Good Bye :)\n");
        exit(EXIT_SUCCESS);
    } else if (strcmp(argv[0], "echo") == 0) {
        for (int i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
        return 1;
    } else if (strcmp(argv[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("getcwd error");
        }
        return 1;
    } else if (strcmp(argv[0], "cd") == 0) {
        if (argc > 2) {
            fprintf(stderr, "cd: too many arguments\n");
        } else {
            char* dir = (argc == 1) ? getenv("HOME") : argv[1];
            if (chdir(dir) != 0) {
                perror("chdir error");
            }
        }
        return 1;
    } else if (strcmp(argv[0], "export") == 0) {
        if (argc != 2) {
            fprintf(stderr, "export: invalid number of arguments\n");
        } else {
            export_variable(argv[1]);
        }
        return 1;
    }
    else if (strcmp(argv[0], "printenv") == 0)
    {
        extern char** environ;
        for (char** env = environ; *env != 0; env++)
        {
            char* thisEnv = *env;
            printf("%s\n", thisEnv);
        }
        return 1;
    }
    return 0; // Not a built-in command
}

void execute_command(char** argv, int argc, RedirectionInfo* redir_info) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        // Child process
        int fd_in_dup = -1;
        int fd_out_dup = -1;
        int fd_err_dup = -1;
        // Handle input redirection
        if (redir_info->input_file != NULL) {
            int fd_in = open(redir_info->input_file, O_RDONLY);
            if (fd_in == -1) {
                perror("open input file failed");
                exit(EXIT_FAILURE);
            }
            fd_in_dup = dup(0); // Duplicate stdin
            if (fd_in_dup == -1) {
                perror("dup input failed");
                close(fd_in);
                exit(EXIT_FAILURE);
            }
            if (dup2(fd_in, 0) == -1) {
                perror("dup2 input failed");
                close(fd_in);
                exit(EXIT_FAILURE);
            }
            close(fd_in);
        }

        // Handle output redirection
        if (redir_info->output_file != NULL) {
            int fd_out = open(redir_info->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out == -1) {
                perror("open output file failed");
                exit(EXIT_FAILURE);
            }
            fd_out_dup = dup(1); // Duplicate stdout
            if (fd_out_dup == -1) {
                perror("dup output failed");
                close(fd_out);
                exit(EXIT_FAILURE);
            }
            if (dup2(fd_out, 1) == -1) {
                perror("dup2 output failed");
                close(fd_out);
                exit(EXIT_FAILURE);
            }
            close(fd_out);
        }

        // Handle error redirection
        if (redir_info->error_file != NULL) {
            int fd_err = open(redir_info->error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_err == -1) {
                perror("open error file failed");
                exit(EXIT_FAILURE);
            }
            fd_err_dup = dup(2); // Duplicate stderr
            if (fd_err_dup == -1) {
                perror("dup error failed");
                close(fd_err);
                exit(EXIT_FAILURE);
            }
            if (dup2(fd_err, 2) == -1) {
                perror("dup2 error failed");
                close(fd_err);
                exit(EXIT_FAILURE);
            }
            close(fd_err);
        }

        execvp(argv[0], argv);

        // If execvp returns, there was an error
        perror("execvp failed");
        
        // Restore file descriptors if needed
        if (fd_in_dup != -1) {
            dup2(fd_in_dup, 0);
            close(fd_in_dup);
        }
        if (fd_out_dup != -1) {
            dup2(fd_out_dup, 1);
            close(fd_out_dup);
        }
        if (fd_err_dup != -1) {
            dup2(fd_err_dup, 2);
            close(fd_err_dup);
        }
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return;
        }
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                fprintf(stderr, "command failed\n");
            }
        }
    }
}

void free_arguments(char** argv, int argc) {
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
}

void add_shell_var(const char* name, const char* value) {
    // Resize the array if needed
    if (numShellVars == maxShellVars) {
        maxShellVars = (maxShellVars == 0) ? 1 : maxShellVars * 2;
        ShellVar* newShellVars = (ShellVar*)realloc(shellVars, sizeof(ShellVar) * maxShellVars);
        if (newShellVars == NULL) {
            perror("realloc failed");
            exit(EXIT_FAILURE);
        }
        shellVars = newShellVars;
    }

    // Allocate memory for the name and value
    shellVars[numShellVars].name = strdup(name);
    shellVars[numShellVars].value = strdup(value);
    if (shellVars[numShellVars].name == NULL || shellVars[numShellVars].value == NULL) {
        perror("strdup failed");
        exit(EXIT_FAILURE);
    }

    numShellVars++;
}

char* get_shell_var(const char* name) {
    for (int i = 0; i < numShellVars; i++) {
        if (strcmp(shellVars[i].name, name) == 0) {
            return shellVars[i].value;
        }
    }
    return NULL;
}

void free_shell_vars() {
    for (int i = 0; i < numShellVars; i++) {
        free(shellVars[i].name);
        free(shellVars[i].value);
    }
    free(shellVars);
}

char* substitute_variables(char* input) {
    char* result = (char*)malloc(strlen(input) * 2 + 1); // Allocate enough space
    if (result == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    int result_index = 0;
    int i = 0;
    while (input[i] != '\0') {
        if (input[i] == '$') {
            i++;
            char var_name[MAX_INPUT];
            int j = 0;
            while (input[i] != '\0' && input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && input[i] != '\r') {
                var_name[j++] = input[i++];
            }
            var_name[j] = '\0';
            char* var_value = get_shell_var(var_name);
            if (var_value != NULL) {
                strcpy(result + result_index, var_value);
                result_index += strlen(var_value);
            }
        } else {
            result[result_index++] = input[i++];
        }
    }
    result[result_index] = '\0';
    return result;
}

int is_valid_assignment(const char* input) {
    const char* eq_ptr = strchr(input, '=');
    if (eq_ptr == NULL) {
        return 0;
    }
    if (strchr(eq_ptr + 1, '=') != NULL)
    {
        return 0;
    }
    if (strchr(input,' ') != NULL || strchr(input,'\t') != NULL)
    {
        if (strchr(input,' ') < eq_ptr || strchr(input,'\t') < eq_ptr)
        {
            return 0;
        }
    }
    
    return 1;
}
void export_variable(const char* name) {
    char* value = get_shell_var(name);
    if (value != NULL) {
        if (setenv(name, value, 1) != 0) {
            perror("setenv failed");
        }
    } else {
        fprintf(stderr, "export: variable not found\n");
    }
}
void free_redirection_info(RedirectionInfo* redir_info) {
    if (redir_info->input_file != NULL) {
        free(redir_info->input_file);
    }
    if (redir_info->output_file != NULL) {
        free(redir_info->output_file);
    }
    if (redir_info->error_file != NULL) {
        free(redir_info->error_file);
    }
}
