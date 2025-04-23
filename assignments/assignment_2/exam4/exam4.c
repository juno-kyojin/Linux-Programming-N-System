// BT4. Tạo file bất kì và ghi một đoạn dữ liệu vào file đó. 
// Thực hiện ghi ra màn hình các thông tin: Loại file, tên file, thời gian chỉnh sửa file lần cuối, kích thước.
//  - Sử dụng struct stat
//


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

int main() {
    // Define the file name
    const char* filename = "test_file.txt";
    
    // Create and write data to file
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    // Write some data to the file
    fprintf(file, "This is a test file created for assignment 2.\n");
    fprintf(file, "This file demonstrates using stat() function.\n");
    
    // Close the file
    fclose(file);
    
    //  Get file information using stat
    struct stat file_info;
    if (stat(filename, &file_info) != 0) {
        perror("Error getting file stats");
        return 1;
    }
    
    printf("File Information:\n");
    
    // File type
    printf("File Type: ");
    if (S_ISREG(file_info.st_mode))
        printf("Regular File\n");
    else if (S_ISDIR(file_info.st_mode))
        printf("Directory\n");
    else if (S_ISLNK(file_info.st_mode))
        printf("Symbolic Link\n");
    else if (S_ISCHR(file_info.st_mode))
        printf("Character Device\n");
    else if (S_ISBLK(file_info.st_mode))
        printf("Block Device\n");
    else if (S_ISFIFO(file_info.st_mode))
        printf("FIFO/Pipe\n");
    else if (S_ISSOCK(file_info.st_mode))
        printf("Socket\n");
    else
        printf("Unknown\n");
    
    // File name
    printf("File Name: %s\n", filename);
    
    // Last modification time
    printf("Last Modified: %s", ctime(&file_info.st_mtime));
    
    // File size
    printf("File Size: %ld bytes\n", file_info.st_size);
    
    return 0;
}
