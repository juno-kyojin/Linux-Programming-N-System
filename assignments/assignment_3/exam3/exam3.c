// BT3. Tín hiệu Process
// Mô tả: Viết một chương trình tạo hai process cha - con. Tiến trình cha sẽ gửi tín hiệu SIGUSR1 tới tiến trình con sau một khoảng thời gian.
// Yêu cầu:
// Sử dụng signal() để định nghĩa hành vi của tiến trình con khi nhận tín hiệu SIGUSR1.
// In ra một thông báo khi tiến trình con nhận được tín hiệu.


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

//hàm xử lý tín hiệu SIGUSR1
void handle_sigusr1(int sig) {
    printf("Tiến trình con (PID: %d) đã nhận tín hiệu SIGUSR1 từ tiến trình cha.\n", getpid());
}

int main() {
    //tạo tiến trình con
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        //đây là tiến trình con
        //đăng ký hàm xử lý tín hiệu SIGUSR1
        if (signal(SIGUSR1, handle_sigusr1) == SIG_ERR) {
            perror("signal registration failed");
            exit(1);
        }

        printf("Tiến trình con (PID: %d) đang chờ tín hiệu SIGUSR1...\n", getpid());

        //tạm dừng chờ tín hiệu
        pause();
        //thoát khi nhận tín hiệu
        exit(0);
    } else {
        //tiến trình cha
        printf("tiến trình cha (PID: %d) đã tạo tiến trình con (PID:%d) \n", getpid(), pid);
        //chờ một khoảng thời gian trước khi gửi tín hiệu
        sleep(3);

        //gửi tín hiệu SIGUSR1 tới tiến trình con
        printf("tiến trình cha (PID: %d) gửi SIGUSR1 tới tiến trình con (PID: %d)\n", getpid(), pid);
        if (kill(pid, SIGUSR1) == -1) {
            perror("kill failed");
            exit(1);
        }
        //chờ tiến trình con kết thúc
        int status;
        waitpid(pid, &status, 0);
        printf("tiến trình con đã kết thúc với trạng thái: %d\n", WEXITSTATUS(status));
    }
    return 0;
}