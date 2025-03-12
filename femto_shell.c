#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 256

int main() {
    char input[MAX_INPUT];

    printf("Welcome to MiniShell! Type 'exit' to quit.\n");

    while (1) {
        printf("MiniShell > ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\nGood Bye :)\n");
            break;
        }

        // Remove newline character if present
        input[strcspn(input, "\n")] = '\0';

        // Handle built-in commands
        if (strcmp(input, "exit") == 0) {
            printf("Good Bye :)\n");
            break;
        } else if (strncmp(input, "echo ", 5) == 0) {
            printf("%s\n", input + 5);
        } else {
            printf("Invalid command\n");
        }
    }

    return EXIT_SUCCESS;
}
