#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
{
    int source_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];

    // Validate the number of arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open the source file for reading
    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1)
    {
        perror("Error opening source file");
        return EXIT_FAILURE;
    }

    // Open the destination file for writing
    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1)
    {
        perror("Error opening destination file");
        close(source_fd);
        return EXIT_FAILURE;
    }

    // Copy the content from source to destination
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0)
    {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written == -1)
        {
            perror("Error writing to destination file");
            close(source_fd);
            close(dest_fd);
            return EXIT_FAILURE;
        }

        if (bytes_written < bytes_read)
        {
            fprintf(stderr, "Error: Incomplete write to destination file '%s'\n", argv[2]);
            close(source_fd);
            close(dest_fd);
            return EXIT_FAILURE;
        }
    }

    if (bytes_read == -1)
    {
        perror("Error reading from source file");
        close(source_fd);
        close(dest_fd);
        return EXIT_FAILURE;
    }

    // Close the files
    close(source_fd);
    close(dest_fd);

    printf("File copied successfully\n");
    return EXIT_SUCCESS;
}
