/*
BT2. Sử dụng exec và Biến Môi Trường
Mô tả: Viết một chương trình trong đó tiến trình con sẽ thay thế chương trình đang chạy 
bằng một chương trình khác, ví dụ ls hoặc date, sử dụng hàm execlp() hoặc execvp().
Yêu cầu:
Truyền các biến môi trường vào chương trình và thực hiện các công việc khác nhau 
(ví dụ biến môi trường được truyền vào là 1 thì thực hiện lệnh ls, nếu là 2 thì thực hiện lệnh date).
Giải thích điều gì xảy ra với tiến trình ban đầu sau khi exec được gọi.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[], char *envp[]) {

    // Lấy giá trị từ biến môi trường COMMAND_TYPE
    char *command_type = getenv("COMMAND_TYPE");
    if (command_type == NULL) {
        fprintf(stderr, "Không tìm thấy biến môi trường COMMAND_TYPE\n");
        fprintf(stderr, "Vui lòng thiết lập biến môi trường với lệnh: export COMMAND_TYPE=1 hoặc export COMMAND_TYPE=2\n");
        return 1;
    }

    // Tạo tiến trình con
    pid_t pid = fork();
    
    if (pid < 0) {
        // Lỗi khi tạo tiến trình con
        perror("fork failed");
        return 1;
    } 
    else if (pid == 0) {
        // Đây là tiến trình con
        // Kiểm tra biến môi trường
        if (strcmp(command_type, "1") == 0) {
            // Thay thế chương trình hiện tại bằng lệnh ls
            execlp("ls", "ls", "-l", NULL);
        } else if (strcmp(command_type, "2") == 0) {
            // Thay thế chương trình hiện tại bằng lệnh date
            execlp("date", "date", NULL);
        } else {
            fprintf(stderr, "Biến môi trường COMMAND_TYPE không hợp lệ. Sử dụng 1 hoặc 2.\n");
            exit(1);
        }
        
        // Nếu exec thành công, dòng này sẽ không bao giờ được thực thi
        perror("exec failed");
        exit(1);
    } 
    else {
        // Đây là tiến trình cha
        printf("Tiến trình cha (PID: %d) đang chờ tiến trình con (PID: %d) kết thúc...\n", getpid(), pid);
        
        // Chờ tiến trình con kết thúc
        int status;
        waitpid(pid, &status, 0);
        
        printf("Tiến trình con đã kết thúc với trạng thái: %d\n", WEXITSTATUS(status));
    }
    
    return 0;
}



// // Sau khi exec thành công:
// Tiến trình con (tiến trình ban đầu gọi exec) bị thay thế hoàn toàn bởi chương trình mới (ls hoặc date).
// PID, biến môi trường, và một số thuộc tính được giữ nguyên, nhưng mã nguồn ban đầu biến mất.
// Không có mã nào sau exec trong tiến trình con được thực thi.
// Sau khi exec thất bại:
// Tiến trình con giữ nguyên chương trình ban đầu, thực thi mã xử lý lỗi, và thường thoát.
// Tiến trình cha:
// Không bị ảnh hưởng bởi exec và tiếp tục chạy, thường chờ tiến trình con hoàn thành.