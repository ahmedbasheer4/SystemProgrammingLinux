#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT 256
#define MAX_ARGS 64
#define DELIMITERS " \t\r\n"

// Function prototypes
char** parse_input(char* input, int* argc);
void execute_command(char** argv, int argc);
int execute_builtin(char** argv, int argc);
void free_arguments(char** argv, int argc);

int main() {
    char input[MAX_INPUT];

    printf("Welcome to Pico Shell! Type 'exit' to quit.\n");

    while (1) {
        printf("PicoShell > ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\nGood Bye :)\n");
            break;
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        int argc;
        char** argv = parse_input(input, &argc);

        if (argc > 0)
        {
            if (execute_builtin(argv, argc) == 0)
            {
                 execute_command(argv, argc);
            }
        }
        free_arguments(argv, argc);
    }

    return EXIT_SUCCESS;
}

char** parse_input(char* input, int* argc) {
    char** argv = (char**)malloc(sizeof(char*) * MAX_ARGS);
    if (argv == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(input, DELIMITERS);
    *argc = 0;

    while (token != NULL) {
        argv[*argc] = (char*)malloc(sizeof(char) * (strlen(token) + 1));
        if (argv[*argc] == NULL) {
            perror("malloc failed");
                // Free previously allocated memory
                for(int i = 0; i < *argc; i++)
                {
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
            char* dir = argc == 1 ? getenv("HOME") : argv[1];
            if (chdir(dir) != 0) {
                perror("chdir error");
            }
        }
        return 1;
    }
    return 0; // Not a built-in command
}
void execute_command(char** argv, int argc) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        // Child process
        execvp(argv[0], argv);

        // If execvp returns, there was an error
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1)
        {
             perror("waitpid failed");
             return;
        }
        if(WIFEXITED(status))
        {
            if (WEXITSTATUS(status) != 0)
            {
                fprintf(stderr,"command failed\n");
            }
        }

    }
}
void free_arguments(char** argv, int argc)
{
     for(int i = 0; i < argc; i++)
    {
         free(argv[i]);
    }
    free(argv);
}
