// BT4. Đếm Số Chẵn và Số Lẻ với Threads
// Viết một chương trình để thực hiện các bước sau:
// Tạo một mảng ngẫu nhiên chứa 100 số nguyên từ 1 đến 100.
// Tạo hai threads: một thread đếm số chẵn và một thread đếm số lẻ trong mảng.
// Khi hai threads hoàn tất việc đếm, in ra tổng số số chẵn và tổng số số lẻ.
// Sử dụng pthread_join để đảm bảo rằng chương trình chỉ kết thúc khi cả hai threads hoàn thành nhiệm vụ.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Biến toàn cục
int array[100]; // Mảng chứa 100 số nguyên
int even_count = 0; // Biến đếm số chẵn
int odd_count = 0; // Biến đếm số lẻ

//Hàm thread đếm số chẵn
void* count_even(void* arg) {
    int*arr = (int*)arg; // Chuyển đổi kiểu dữ liệu từ void* sang int*
    for (int i = 0; i < 100; i++) {
        if (arr[i] % 2 == 0) {
            even_count++;
        }
    }
    return NULL;
}
//Hàm thread đếm số lẻ
void* count_odd(void* arg) {
    int*arr = (int*)arg; // Chuyển đổi kiểu dữ liệu từ void* sang int*
    for (int i = 0; i < 100; i++) {
        if (arr[i] % 2 != 0) {
            odd_count++;
        }
    }
    return NULL;
}

int main (){
    //Khởi tạo mảng ngẫu nhiên
    srand(time(NULL)); // Khởi tạo seed cho hàm rand
    // Tạo mảng ngẫu nhiên chứa 100 số nguyên từ 1 đến 100
    for (int i = 0; i < 100; i++) {
        array[i] = rand() % 100 + 1; // Sinh số ngẫu nhiên từ 1 đến 100
    }

    //Tao hai threads
    pthread_t even_thread, odd_thread;
    if (pthread_create(&even_thread, NULL, count_even, (void*)array) != 0) {
        perror("Error creating even thread");
        return 1;
    }
    if (pthread_create(&odd_thread, NULL, count_odd, (void*)array) != 0) {
        perror("Error creating odd thread");
        return 1;
    }

    //Chờ hai threads hoàn thành
    if (pthread_join(even_thread, NULL) != 0) {
        perror("Error joining even thread");
        return 1;
    }
    if (pthread_join(odd_thread, NULL) != 0) {
        perror("Error joining odd thread");
        return 1;
    }
    //In kết quả
    printf("Number of even numbers: %d\n", even_count);
    printf("Number of odd numbers: %d\n", odd_count);
    printf("Total numbers: %d\n", even_count + odd_count);
    return 0;
}