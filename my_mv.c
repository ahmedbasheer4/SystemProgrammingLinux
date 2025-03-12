#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Try renaming first
    if (rename(argv[1], argv[2]) == 0) {
        printf("File moved successfully.\n");
        return EXIT_SUCCESS;
    }

    // Fallback: Copy and delete
    printf("Cross-device move detected. Copying and deleting...\n");

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        perror("Error opening source file");
        return EXIT_FAILURE;
    }

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error opening destination file");
        close(src_fd);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        if ((bytes_written = write(dest_fd, buffer, bytes_read)) == -1) {
            perror("Error writing to destination");
            close(src_fd);
            close(dest_fd);
            return EXIT_FAILURE;
        }
    }

    close(src_fd);
    close(dest_fd);

    if (unlink(argv[1]) != 0) {
        perror("Error deleting source file");
        return EXIT_FAILURE;
    }

    printf("File moved successfully (copied & deleted).\n");
    return EXIT_SUCCESS;
}
