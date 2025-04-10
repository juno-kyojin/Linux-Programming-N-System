// BT1. Viết một chương trình mở một file bất kì và sử dụng cờ O_APPEND, 
// sau đó thực hiện seek về đầu file rồi ghi một vài dữ  liệu vào file đó. 
// Dữ liệu sẽ xuất hiện ở vị trí nào của file và tại sao lại như vậy?

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Mở file "example.txt" với cờ O_RDWR (đọc/ghi) và O_APPEND
    int fd = open("example.txt", O_RDWR | O_APPEND | O_CREAT, 0644);
    if (fd == -1) {
        perror("Lỗi khi mở file");
        return 1;
    }

    // Di chuyển con trỏ về đầu file
    off_t offset = lseek(fd, 0, SEEK_SET);
    if (offset == -1) {
        perror("Lỗi khi seek");
        close(fd);
        return 1;
    }

    // Ghi dữ liệu vào file
    const char *data = "Hello, World!\n";
    ssize_t bytes_written = write(fd, data, strlen(data));
    if (bytes_written == -1) {
        perror("Lỗi khi ghi file");
        close(fd);
        return 1;
    }

    printf("Đã ghi %zd bytes vào file.\n", bytes_written);

    // Đóng file
    close(fd);
    return 0;
}




// Dữ liệu "Hello, World!\n" sẽ được ghi ở cuối file, chứ không phải ở đầu file,Lý do nằm ở cờ O_APPEND:
// Khi mở file với O_APPEND, hệ điều hành bỏ qua vị trí con trỏ hiện tại (do lseek thiết lập) và luôn ghi dữ liệu ở cuối file.
// lseek chỉ thay đổi con trỏ trong bộ nhớ của chương trình, nhưng O_APPEND có hiệu lực cao hơn, ép buộc mọi thao tác write diễn ra ở cuối file.