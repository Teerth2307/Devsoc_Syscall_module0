#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <utime.h>
#include <time.h>

/*
 * touch: A C implementation similar to the Linux 'touch' command
 *
 * Features:
 *  - Creates a file if it does not exist when run as ./touch <filename>
 *  - Supports:
 *      - -c : Do not create file if it doesn’t exist
 *      - -a : Update access time only
 *      - -m : Update modification time only
 *  - By default, both access and modification times are updated.
 */

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-a] [-m] [-c] <filename>\n", argv[0]);
        return 1;
    }

    int create = 1;          // create files by default (disabled by -c)
    int change_access = 0;   // -a flag
    int change_modify = 0;   // -m flag
    char *filepath = NULL;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0)
            create = 0;
        else if (strcmp(argv[i], "-a") == 0)
            change_access = 1;
        else if (strcmp(argv[i], "-m") == 0)
            change_modify = 1;
        else
            filepath = argv[i];
    }

    if (filepath == NULL) {
        fprintf(stderr, "Error: No file specified.\n");
        return 1;
    }

    struct stat st;
    int fd;

    // Check file existence
    if (stat(filepath, &st) == -1) {
        // File does not exist
        if (!create) {
            printf("File '%s' does not exist (skipped due to -c flag).\n", filepath);
            return 0;
        }

        // Create file
        fd = open(filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1) {
            perror("Error creating file");
            return 1;
        }

        printf("Created new file '%s'\n", filepath);
        close(fd);
        return 0; // No timestamp change needed for new files (default behavior)
    }

    // File exists -> update timestamps
    struct utimbuf new_times;
    new_times.actime = st.st_atime;
    new_times.modtime = st.st_mtime;

    time_t now = time(NULL);

    // Apply -a or -m rules
    if (change_access && !change_modify) {
        new_times.actime = now;
    } else if (change_modify && !change_access) {
        new_times.modtime = now;
    } else {
        // Default behavior: update both
        new_times.actime = now;
        new_times.modtime = now;
    }

    if (utime(filepath, &new_times) == -1) {
        perror("Error updating timestamps");
        return 1;
    }

    printf("Successfully touched '%s'\n", filepath);
    return 0;
}
