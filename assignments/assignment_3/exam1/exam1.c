//Mô tả: Viết một chương trình C tạo một process con bằng cách sử dụng fork(). 
//Sau khi tạo process, tiến trình con sẽ in ra PID của nó 
//và tiến trình cha in ra PID của tiến trình con.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork(); // Tạo process con

    if (pid < 0) {
        // Lỗi khi tạo process
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Đây là process con
        printf("Process child PID: %d\n", getpid());
    } else {
        // Đây là process cha
        printf("Process parent PID: %d\n", getpid());
        printf("Process child PID: %d\n", pid);
    }

    return 0;
}