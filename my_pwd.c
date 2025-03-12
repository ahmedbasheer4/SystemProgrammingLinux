#include <stdio.h>
#include <stdlib.h>                                                                                                                                         #include <unistd.h>                                                                                                                                                                                                                                                                                                     #define PATH_MAX 100

int main()
{
    char cwd[PATH_MAX];
    if ((getcwd(cwd, PATH_MAX)) != NULL) {
        printf("%s\n", cwd); // Success                                                                                                                                        exit(0); // Success
    } else {                                                                                                                                                        printf("Error: Could not get current working directory");
        return EXIT_FAILURE; // Failure
    }
    return EXIT_SUCCESS;
}
