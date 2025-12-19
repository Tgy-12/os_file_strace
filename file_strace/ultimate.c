/*
 * File Copying Program Implementation (POSIX API)
 * Course: Operating Systems
 */

#include <stdio.h> //used for printf, fprintf, perror
#include <stdlib.h> //used for exit codes
#include <fcntl.h>//used for open() flags
#include <unistd.h>//used for read(), write(), close()
#include <errno.h>//used for specific error messages
#include <time.h>//for performance timing
//======color optimizations========== in terminal "\033[0m" means reset to default without color and the characters after \033[ start the color code.033 is the escape character.and means octal 33 = decimal 27 = hex 1B and [ starts the control sequence.and m ends it the numbers in between define the color/style.91-97 are foreground colors.meaning red to white bright versions.the word m after the numbers ends the color code.
#define RESET   "\033[0m"
#define RED     "\033[91m"
#define GREEN   "\033[92m"
#define YELLOW  "\033[93m"
#define BLUE    "\033[94m"
#define MAGENTA "\033[95m"
#define CYAN    "\033[96m"
#define WHITE   "\033[97m"
#define BOLD    "\033[1m"

#define BUFFER_SIZE 2048
#define MAX_FILENAME 256

int main() {
    char source_path[MAX_FILENAME];
    char dest_path[MAX_FILENAME];
    int src_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];

    // ====  Performance counters ====
    int read_count = 0;
    int write_count = 0;
    int loop_count = 0;
    long total_bytes_read = 0;
    long total_bytes_written = 0;
    struct timespec start_time, end_time;
    double elapsed_seconds;

    // Start timer
    //clock_gettime(CLOCK_MONOTONIC, &start_time);

    // --- 1. User Input ---
    printf(MAGENTA BOLD "=== File Copying Program ===\n" RESET);
    printf(GREEN BOLD "Enter source file path:==> " RESET);
    if (scanf("%255s", source_path) != 1) {
        fprintf(stderr, RED "Error reading source path.\n" RESET);
        return 1;
    }
    printf(GREEN BOLD "Enter destination file path:==> " RESET);
    if (scanf("%255s", dest_path) != 1) {
        fprintf(stderr, RED "Error reading destination path.\n" RESET);
        return 1;
    }

    // --- 2. Open Source File ---
    src_fd = open(source_path, O_RDONLY);
    if (src_fd == -1) {
        fprintf(stderr, RED "Error: " RESET);
        perror("opening source file");
        return 1;
    }

    // --- 3. Open Destination File ---
    dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        fprintf(stderr, RED "Error: " RESET);
        perror("opening/creating destination file");
        close(src_fd);
        return 1;
    }

    // --- 4. The Copy Loop ---
    printf(CYAN "Copying data..." RESET);

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        // ==== Count read operation ====
        read_count++;
        total_bytes_read += bytes_read;
        loop_count++;

        printf(".");
        fflush(stdout);//this means flush the output buffer of a stream to ensure the dot is printed immediately instead of being buffered.

        bytes_written = write(dest_fd, buffer, bytes_read);

        // ====  Count write operation ====
        write_count++;
        total_bytes_written += bytes_written;

        if (bytes_written != bytes_read) {
            fprintf(stderr, "\n" RED "Error: " RESET);
            perror("writing to destination file");
            close(src_fd);
            close(dest_fd);
            return 1;//end program with error code
        }
    }
    printf("\n");

    // --- 5. Final Error Checking ---
    if (bytes_read == -1) {
        fprintf(stderr, RED "Error: " RESET);
        perror("reading from source file");
    } else {
        printf(GREEN BOLD "✅ Success!" RESET GREEN " File is copied from '%s' to '%s'.\n" RESET,
               source_path, dest_path);
    }

    // --- 6. Cleanup ---
    if (close(src_fd) == -1) {
        fprintf(stderr, YELLOW "Warning: " RESET);
        perror("closing source file");
    }
    if (close(dest_fd) == -1) {
        fprintf(stderr, YELLOW "Warning: " RESET);
        perror("closing destination file");
    }

    // ====  Performance Summary ====
   // clock_gettime(CLOCK_MONOTONIC, &end_time);
    elapsed_seconds = (end_time.tv_sec - start_time.tv_sec) +
                     (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf(MAGENTA BOLD "\n=== Performance Statistics ===\n" RESET);
    printf(CYAN "┌─────────────────────────────────────────────┐\n" RESET);
    printf(CYAN "│ " YELLOW BOLD "System Call Analysis" CYAN "                         │\n" RESET);
    printf(CYAN "├─────────────────────────────────────────────┤\n" RESET);
    printf(CYAN "│ " RESET "Total read() calls:      %-10d      " CYAN "│\n" RESET, read_count);
    printf(CYAN "│ " RESET "Total write() calls:     %-10d      " CYAN "│\n" RESET, write_count);
    printf(CYAN "│ " RESET "Total loops/iterations:  %-10d      " CYAN "│\n" RESET, loop_count);
    printf(CYAN "│ " RESET "Total bytes read:        %-10ld      " CYAN "│\n" RESET, total_bytes_read);
    printf(CYAN "│ " RESET "Total bytes written:     %-10ld      " CYAN "│\n" RESET, total_bytes_written);
    printf(CYAN "│ " RESET "Buffer size:             %-10d      " CYAN "│\n" RESET, BUFFER_SIZE);

    if (elapsed_seconds > 0) {
        double speed_mb = (total_bytes_read / (1024.0 * 1024.0)) / elapsed_seconds;
        printf(CYAN "│ " RESET "Copy time:              %-8.3f sec      " CYAN "│\n" RESET, elapsed_seconds);
        printf(CYAN "│ " RESET "Transfer speed:         %-8.2f MB/s     " CYAN "│\n" RESET, speed_mb);
    }

    printf(CYAN "└─────────────────────────────────────────────┘\n" RESET);

    printf(MAGENTA BOLD "\n=== Program Completed ===\n" RESET);
    return 0;
}
