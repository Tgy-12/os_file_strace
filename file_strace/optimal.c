/*
 * File Copying Program Implementation (POSIX API)
 * Course: Operating Systems
 * what is POSIX?
 * ====> POSIX is a user level interface which is used to give services from an operating system kernel about sth like any others os.
 */

//The mandatory libraries to be includded to be perfecto ....
#include <stdio.h>      // Standard I/O (for printf, scanf, perror)
#include <stdlib.h>     // Standard library (for exit codes)
#include <fcntl.h>      // File Control (Crucial for open() flags)
#include <unistd.h>     // POSIX API (Crucial for read(), write(), close())
#include <errno.h>      // Error numbers (to get specific error messages)

#define RESET   "\033[0m"
#define RED     "\033[91m"
#define GREEN   "\033[92m"
#define YELLOW  "\033[93m"
#define BLUE    "\033[94m"
#define MAGENTA "\033[95m"
#define CYAN    "\033[96m"
#define WHITE   "\033[97m"
#define BOLD    "\033[1m"

#define BUFFER_SIZE 2048 // the critical performance choice...so we can change it.
#define MAX_FILENAME 256

int main() {
    // Declare the basics
    char source_path[MAX_FILENAME];
    char dest_path[MAX_FILENAME];

    int src_fd, dest_fd;             // File Descriptors (integers)
    ssize_t bytes_read, bytes_written; // Used for I/O functions (can be -1 for error)
    char buffer[BUFFER_SIZE];          // The temporary storage area for data

    // --- 1. User Input ---
    printf(MAGENTA BOLD "=== File Copying Program ===\n" RESET);
    printf(GREEN BOLD "Enter source file path:==> " RESET);
    if (scanf("%255s", source_path) != 1) {
        fprintf(stderr, RED "Error reading source path.\n" RESET);
        return 1;
    }
    printf(GREEN BOLD "Enter destination file path:==> " RESET);
    //1-succes, 0-no string found, (space and if input npot match %s),  -1, EOF,,if file input ends unexpectedly
    if (scanf("%255s", dest_path) != 1) {
        fprintf(stderr, RED "Error reading destination path.\n" RESET);
        return 1;
    }
// --- 2. Open Source File ---
// O_RDONLY: is used to   open for reading only.
src_fd = open(source_path, O_RDONLY);
if (src_fd == -1) {//if src_fd <0 , open failed, src file not exist , no permission and src_fd >=0 success
    // perror prints the string passed to it followed by the system error message
    fprintf(stderr, RED "Error: " RESET);
    perror("opening source file");
    return 1;
}
/*
* -----3, process the source files ----
* O_WRONLY: Write only means open the file for write operations only.
* O_CREAT: Create file if it doesn't exist
* O_TRUNC: Truncate file to length 0 if it already exists (overwrite)
* 0644: Permissions (rw-r--r--) and defines who can access the new file.
*/
dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
if (dest_fd == -1) {
    fprintf(stderr, RED "Error: " RESET);
    perror("opening/creating destination file");
    close(src_fd); // Clean up source before exiting
    return 1;// exit with error code but if it returns 0 means success
}
// --- 4. The Copy Loop ---
printf(CYAN "Copying data..." RESET);

// read() returns the number of bytes read, 0 on EOF, or -1 on error
while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
    printf(".");
    fflush(stdout);// Ensure the dot is printed immediately fflush means flush the output buffer of a stream. It forces the written data to be output immediately instead of being buffered.
    // Write exactly the amount of data we read
    bytes_written = write(dest_fd, buffer, bytes_read);

    if (bytes_written != bytes_read) {
        // This catches disk full errors or other write interruptions
        fprintf(stderr, "\n" RED "Error: " RESET);
        perror("writing to destination file");
        close(src_fd);
        close(dest_fd);
        return 1;
    }
}
printf("\n");
// --- 5. Final Error Checking ---
if (bytes_read == -1) {//read oprn failed, no permsion, HW error , disk problem
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
printf(MAGENTA BOLD "\n=== Program Completed ===\n" RESET);
return 0;
}
