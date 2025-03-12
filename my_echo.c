#include <stdio.h>
#include <stdlib.h>                                                                                                                                         #include <unistd.h>                                                                                                                                                                                                                                                                                                     #define PATH_MAX 100

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        // No arguments provided, just print a newline
        printf("\n");
    }
    else
    {
        for(int i = 1; i < argc;i++)
        {
            printf("%s",argv[i]);
            // Add a space between arguments, except for the last one
            if (i < (argc - 1))
            {
                printf(" ");
            }
        }
        printf("\n"); // Add a newline at the end
    }
    return 0;
}
