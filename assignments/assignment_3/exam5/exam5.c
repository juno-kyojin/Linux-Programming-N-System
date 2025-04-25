// BT5. Tạo Process Zombie và Orphan
// Mô tả: Viết một chương trình tạo ra một tiến trình zombie và một tiến trình orphan để hiểu về các loại tiến trình đặc biệt này.
// Yêu cầu:
// Để tạo zombie, cho tiến trình cha kết thúc sớm trong khi tiến trình con vẫn còn chạy, 
// và sử dụng ps để quan sát trạng thái zombie của tiến trình.
// Để tạo orphan, cho tiến trình con ngủ trong một khoảng thời gian dài hơn tiến trình cha, để tiến trình con trở thành orphan.
// Giải thích trạng thái zombie và orphan, cùng lý do tại sao chúng xuất hiện trong Linux.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Sử dụng: %s <zombie|orphan>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "zombie") == 0) {
        // Tạo tiến trình zombie
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            printf("Tiến trình con (PID: %d) kết thúc ngay\n", getpid());
            exit(0);
        } else {
            printf("Tiến trình cha (PID: %d) tạo tiến trình con (PID: %d)\n", getpid(), pid);
            printf("Chạy 'ps aux | grep Z' để xem zombie\n");
            sleep(30); // Giữ cha sống, không thu thập trạng thái
        }
    } else if (strcmp(argv[1], "orphan") == 0) {
        // Tạo tiến trình orphan
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            printf("Tiến trình con (PID: %d, PPID: %d) đang chạy...\n", getpid(), getppid());
            sleep(10);
            printf("Tiến trình con (PID: %d) giờ có PPID mới: %d\n", getpid(), getppid());
            printf("Chạy 'ps -p %d -o ppid=' để kiểm tra PPID\n", getpid());
        } else {
            printf("Tiến trình cha (PID: %d) tạo tiến trình con (PID: %d) và kết thúc\n", getpid(), pid);
            exit(0);
        }
    } else {
        fprintf(stderr, "Đối số không hợp lệ (zombie hoặc orphan)\n");
        return 1;
    }

    return 0;
}