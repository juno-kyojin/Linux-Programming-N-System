// BT3. Viết một chương trình có só lượng command-line arguments là 3, có dạng như sau:
// $ ./example_program filename num-bytes [r/w] "Hello"
// Trong đó:
// 	1. example_grogram: Tên file thực thi
// 	2. filename: Tên file
// 	3. num-bytes: Số byte muốn read/write
// 	4. [r/w]: r -> Thực hiện đọc từ  filename và in ra màn hình
// 		    w -> Thực hiện ghi vào filename
// 	5. "Hello": Nội dung bất kì muốn read/write vào filename

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Kiểm tra số lượng đối số
    if (argc < 4 || argc > 5) {
        fprintf(stderr, "Sử dụng: %s filename num-bytes [r/w] [\"content\"]\n", argv[0]);
        return 1;
    }

    // Lấy các đối số
    char *filename = argv[1];           // Tên file
    int num_bytes = atoi(argv[2]);      // Số byte muốn đọc/ghi
    char *operation = argv[3];          // "r" hoặc "w"
    char *content = (argc == 5) ? argv[4] : NULL;  // Nội dung (nếu có)

    // Kiểm tra num-bytes hợp lệ
    if (num_bytes <= 0) {
        fprintf(stderr, "num-bytes phải là số nguyên dương\n");
        return 1;
    }

    // Kiểm tra operation hợp lệ
    if (strcmp(operation, "r") != 0 && strcmp(operation, "w") != 0) {
        fprintf(stderr, "Operation phải là 'r' hoặc 'w'\n");
        return 1;
    }

    // Thực hiện đọc hoặc ghi
    int fd;
    if (strcmp(operation, "r") == 0) {
        // Mở file để đọc
        fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("Lỗi khi mở file để đọc");
            return 1;
        }

        // Đọc dữ liệu
        char *buffer = malloc(num_bytes + 1);  // +1 để thêm null terminator
        if (buffer == NULL) {
            perror("Lỗi cấp phát bộ nhớ");
            close(fd);
            return 1;
        }

        ssize_t bytes_read = read(fd, buffer, num_bytes);
        if (bytes_read == -1) {
            perror("Lỗi khi đọc file");
            free(buffer);
            close(fd);
            return 1;
        }

        // Thêm null terminator và in ra màn hình
        buffer[bytes_read] = '\0';
        printf("Đọc được: %s\n", buffer);

        free(buffer);
        close(fd);
    } else if (strcmp(operation, "w") == 0) {
        // Kiểm tra xem có nội dung để ghi không
        if (content == NULL) {
            fprintf(stderr, "Cần cung cấp nội dung để ghi\n");
            return 1;
        }

        // Mở file để ghi (tạo mới hoặc ghi đè)
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("Lỗi khi mở file để ghi");
            return 1;
        }

        // Ghi dữ liệu
        size_t content_len = strlen(content);
        ssize_t bytes_to_write = (num_bytes < content_len) ? num_bytes : content_len;
        ssize_t bytes_written = write(fd, content, bytes_to_write);
        if (bytes_written == -1) {
            perror("Lỗi khi ghi file");
            close(fd);
            return 1;
        }

        printf("Đã ghi %zd bytes vào file\n", bytes_written);
        close(fd);
    }

    return 0;
}