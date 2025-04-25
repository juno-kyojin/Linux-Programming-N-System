// BT4. Ghi nhận Trạng thái Process
// Mô tả: Viết một chương trình tạo một tiến trình con, sau đó tiến trình cha sử dụng wait() để chờ tiến trình con kết thúc và ghi nhận trạng thái kết thúc của nó.
// Yêu cầu:
// Trong tiến trình con, chọn trạng thái kết thúc bằng cách gọi exit() với các giá trị khác nhau.
// Sử dụng WIFEXITED() và WEXITSTATUS() để xác nhận và ghi nhận mã thoát của tiến trình con.


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 2) {
        fprintf(stderr, "Sử dụng: %s <exit_code>\n", argv[0]);
        return 1;
    }

    // Convert and validate exit code using strtol
    char *endptr;
    errno = 0;
    long exit_code = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || errno != 0 || exit_code < 0 || exit_code > 255) {
        fprintf(stderr, "Mã thoát không hợp lệ (phải là số nguyên từ 0 đến 255)\n");
        return 1;
    }

    // Create child process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Tiến trình con (PID: %d) đang chạy...\n", getpid());
        sleep(10); // Tạm dừng 10 giây để dễ gửi tín hiệu
        exit((int)exit_code); // Thoát với mã được chỉ định
    } else {
        // Tiến trình cha
        printf("Tiến trình cha (PID: %d) đang chờ tiến trình con (PID: %d) kết thúc...\n", getpid(), pid);
        
        // Chờ tiến trình con kết thúc
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return 1;
        }

        // Kiểm tra trạng thái kết thúc
        if (WIFEXITED(status)) {
            printf("Tiến trình con (PID: %d) đã kết thúc với mã thoát: %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Tiến trình con (PID: %d) bị gián đoạn bởi tín hiệu: %d\n", pid, WTERMSIG(status));
        } else {
            printf("Tiến trình con (PID: %d) không kết thúc bình thường.\n", pid);
        }
    }

    return 0;
}