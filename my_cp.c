#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
{
    int source_fd = 0, dest_fd = 0;
    ssize_t bytes_read = 0, bytes_written = 0;
    char buffer[BUFFER_SIZE];
    // Validate the number of Parameters
    if (argc != 3)
    {
        printf("Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(-2);
    }
    else
    {
        // Open the source file for reading
        source_fd = open(argv[1], O_RDONLY);
        if (source_fd == -1)
        {
            printf("Error opening source file '%s'\n", argv[1]);
            exit(-3);
        }
        else
        {
            // Open the destination file for writing, create it if it doesn't exist,
            // and truncate it if it does exist. Permissions are set to 0644 (rw-r--r--)
            dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (dest_fd == -1)
            {
                printf("Error opening destination file '%s'\n", argv[2]);
                close(source_fd); // Close the source file if the destination fails
                exit(-4);
            }
            else
            {
                /* Do Nothing */
            }
        }
    }
    // Copy the content from source to destination
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0)
    {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written == -1)
        {
            printf("Error writing to destination file '%s'\n", argv[2]);
            close(source_fd);
            close(dest_fd);
            exit(-5);
        }
        else
        {
            /* Do Nothing */
        }
        if (bytes_written < bytes_read)
        {
            printf("Error: Incomplete write to destination file '%s'\n", argv[2]);
            close(source_fd);
            close(dest_fd);
            exit(-6);
        }
        else
        {
            /* Do Nothing */
        }
    }

    if (bytes_read == -1)
    {
        printf("Error reading from source file '%s'\n", argv[1]);
        exit(-7);
    }
    else
    {
        /* Do Nothing */
    }
    // Close the files
    close(source_fd);
    close(dest_fd);

    if(bytes_read != -1){
        printf("File copied successfully\n");
        exit(-8);
    }
    else
    {
        /* Do Nothing */
    }
    return 1;
}
