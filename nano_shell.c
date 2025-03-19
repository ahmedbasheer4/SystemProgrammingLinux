#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>

#define MAX_INPUT 256  // Maximum length of input line
#define MAX_ARGS 64    // Maximum number of arguments
#define DELIMITERS " \t\r\n" // Delimiters for parsing input

// Structure to store shell variables
typedef struct {
    char* name;  // Variable name
    char* value; // Variable value
} ShellVar;

// Global array to store shell variables
ShellVar* shellVars = NULL; // Array of shell variables
int numShellVars = 0;       // Number of shell variables currently stored
int maxShellVars = 0;       // Maximum capacity of the shellVars array

// Function prototypes
char** parse_input(char* input, int* argc);
void execute_command(char** argv, int argc);
int execute_builtin(char** argv, int argc);
void free_arguments(char** argv, int argc);
void add_shell_var(const char* name, const char* value);
char* get_shell_var(const char* name);
void free_shell_vars();
char* substitute_variables(char* input);
int is_valid_assignment(const char* input);
void export_variable(const char* name);

int main() {
    char input[MAX_INPUT]; // Buffer for user input

    printf("Welcome to Nano Shell! Type 'exit' to quit.\n");

    while (1) {
        printf("Nano Shell Prompt > ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) { // Read a line of input
            printf("\nGood Bye :)\n");
            break; // Exit on EOF or error
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Check for variable assignment
        if (strchr(input, '=') != NULL) { // Check if the input contains an '='
            if (is_valid_assignment(input)) { // Check if it's a valid assignment
                char* eq_ptr = strchr(input, '='); // Find the '=' sign
                *eq_ptr = '\0'; // Split the string into name and value
                add_shell_var(input, eq_ptr + 1); // Add the variable to the shell
            } else {
                fprintf(stderr, "Invalid command\n"); // Print error for invalid assignment
            }
            continue; // Go to the next iteration
        }

        char* substituted_input = substitute_variables(input); // Substitute variables in the input
        int argc;
        char** argv = parse_input(substituted_input, &argc); // Parse the input into arguments
        free(substituted_input); // Free the memory allocated by substitute_variables

        if (argc > 0) { // If there are arguments
            if (execute_builtin(argv, argc) == 0) { // Check if it's a built-in command
                execute_command(argv, argc); // Execute external command
            }
        }
        free_arguments(argv, argc); // Free the memory allocated for arguments
    }

    free_shell_vars(); // Free the memory allocated for shell variables
    return EXIT_SUCCESS;
}

// Parses the input string into an array of arguments
char** parse_input(char* input, int* argc) {
    char** argv = (char**)malloc(sizeof(char*) * MAX_ARGS); // Allocate memory for arguments
    if (argv == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(input, DELIMITERS); // Tokenize the input string
    *argc = 0; // Initialize argument count

    while (token != NULL) {
        argv[*argc] = (char*)malloc(sizeof(char) * (strlen(token) + 1)); // Allocate memory for each argument
        if (argv[*argc] == NULL) {
            perror("malloc failed");
            // Free previously allocated memory
            for (int i = 0; i < *argc; i++) {
                free(argv[i]);
            }
            free(argv);
            exit(EXIT_FAILURE);
        }
        strcpy(argv[*argc], token); // Copy the token into the argument array
        (*argc)++; // Increment argument count

        if (*argc >= MAX_ARGS) {
            fprintf(stderr, "Too many arguments\n");
            break; // Stop if too many arguments
        }

        token = strtok(NULL, DELIMITERS); // Get the next token
    }
    argv[*argc] = NULL; // Null-terminate the argument array
    return argv;
}

// Executes built-in commands
int execute_builtin(char** argv, int argc) {
    if (strcmp(argv[0], "exit") == 0) { // Exit command
        printf("Good Bye :)\n");
        exit(EXIT_SUCCESS);
    } else if (strcmp(argv[0], "echo") == 0) { // Echo command
        for (int i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
        return 1; // Indicate that it's a built-in command
    } else if (strcmp(argv[0], "pwd") == 0) { // Print working directory command
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("getcwd error");
        }
        return 1; // Indicate that it's a built-in command
    } else if (strcmp(argv[0], "cd") == 0) { // Change directory command
        if (argc > 2) {
            fprintf(stderr, "cd: too many arguments\n");
        } else {
            char* dir = (argc == 1) ? getenv("HOME") : argv[1]; // Get the directory to change to
            if (chdir(dir) != 0) {
                perror("chdir error");
            }
        }
        return 1; // Indicate that it's a built-in command
    } else if (strcmp(argv[0], "export") == 0) { // Export command
        if (argc != 2) {
            fprintf(stderr, "export: invalid number of arguments\n");
        } else {
            export_variable(argv[1]); // Export the variable
        }
        return 1; // Indicate that it's a built-in command
    }
    else if (strcmp(argv[0], "printenv") == 0) // Print environment variables command
    {
        extern char** environ; // Access the environment variables
        for (char** env = environ; *env != 0; env++)
        {
            char* thisEnv = *env;
            printf("%s\n", thisEnv);
        }
        return 1; // Indicate that it's a built-in command
    }
    return 0; // Not a built-in command
}

// Executes external commands
void execute_command(char** argv, int argc) {
    pid_t pid = fork(); // Fork a child process
    if (pid == -1) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        // Child process
        execvp(argv[0], argv); // Execute the command

        // If execvp returns, there was an error
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) { // Wait for the child process to finish
            perror("waitpid failed");
            return;
        }
        if (WIFEXITED(status)) { // Check if the child process exited normally
            if (WEXITSTATUS(status) != 0) { // Check the exit status
                fprintf(stderr, "command failed\n");
            }
        }
    }
}

// Frees the memory allocated for arguments
void free_arguments(char** argv, int argc) {
    for (int i = 0; i < argc; i++) {
        free(argv[i]); // Free each argument
    }
    free(argv); // Free the argument array
}

// Adds a shell variable
void add_shell_var(const char* name, const char* value) {
    // Resize the array if needed
    if (numShellVars == maxShellVars) {
        maxShellVars = (maxShellVars == 0) ? 1 : maxShellVars * 2; // Double the capacity
        ShellVar* newShellVars = (ShellVar*)realloc(shellVars, sizeof(ShellVar) * maxShellVars); // Reallocate memory
        if (newShellVars == NULL) {
            perror("realloc failed");
            exit(EXIT_FAILURE);
        }
        shellVars = newShellVars; // Update the pointer
    }

    // Allocate memory for the name and value
    shellVars[numShellVars].name = strdup(name); // Duplicate the name
    shellVars[numShellVars].value = strdup(value); // Duplicate the value
    if (shellVars[numShellVars].name == NULL || shellVars[numShellVars].value == NULL) {
        perror("strdup failed");
        exit(EXIT_FAILURE);
    }

    numShellVars++; // Increment the number of shell variables
}

// Gets the value of a shell variable
char* get_shell_var(const char* name) {
    for (int i = 0; i < numShellVars; i++) {
        if (strcmp(shellVars[i].name, name) == 0) { // Compare the name
            return shellVars[i].value; // Return the value
        }
    }
    return NULL; // Variable not found
}

// Frees the memory allocated for shell variables
void free_shell_vars() {
    for (int i = 0; i < numShellVars; i++) {
        free(shellVars[i].name); // Free the name
        free(shellVars[i].value); // Free the value
    }
    free(shellVars); // Free the array
}

// Substitutes variables in the input string
char* substitute_variables(char* input) {
    char* result = (char*)malloc(strlen(input) * 2 + 1); // Allocate enough space (worst case: all $variables)
    if (result == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    int result_index = 0;
    int i = 0;
    while (input[i] != '\0') {
        if (input[i] == '$') { // Check for variable substitution
            i++;
            char var_name[MAX_INPUT];
            int j = 0;
            while (input[i] != '\0' && input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && input[i] != '\r') { // Extract the variable name
                var_name[j++] = input[i++];
            }
            var_name[j] = '\0';
            char* var_value = get_shell_var(var_name); // Get the variable value
            if (var_value != NULL) {
                strcpy(result + result_index, var_value); // Copy the value to the result
                result_index += strlen(var_value);
            }
        } else {
            result[result_index++] = input[i++]; // Copy the character to the result
        }
    }
    result[result_index] = '\0'; // Null-terminate the result
    return result;
}

// Checks if the input is a valid variable assignment
int is_valid_assignment(const char* input) {
    const char* eq_ptr = strchr(input, '='); // Find the '=' sign
    if (eq_ptr == NULL) {
        return 0; // No '=' sign
    }
    if (strchr(eq_ptr + 1, '=') != NULL) // check if there is more than one '='
    {
        return 0; // more than one '=' sign
    }
    if (strchr(input,' ') != NULL || strchr(input,'\t') != NULL) // check if there is space or tab
    {
        if (strchr(input,' ') < eq_ptr || strchr(input,'\t') < eq_ptr) // check if the space or tab is before '='
        {
            return 0; // space or tab before '='
        }
    }
    return 1; // Valid assignment
}

// Exports a shell variable to the environment
void export_variable(const char* name) {
    char* value = get_shell_var(name); // Get the variable value
    if (value != NULL) {
        if (setenv(name, value, 1) != 0) { // Set the environment variable
            perror("setenv failed");
        }
    } else {
        fprintf(stderr, "export: variable not found\n"); // Variable not found
    }
}
