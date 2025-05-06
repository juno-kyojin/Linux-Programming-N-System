// BT1. Tạo và Chạy Threads Cơ Bản
// Viết một chương trình sử dụng thư viện pthread.h để thực hiện các thao tác sau:
// Tạo hai threads. Mỗi thread sẽ in ra một chuỗi thông báo kèm theo ID của thread 
// (ví dụ: "Thread 1: Hello from thread").
// Sử dụng hàm pthread_join để đợi cả hai threads hoàn thành trước khi chương trình kết thúc.
// Sử dụng pthread_create và pthread_join như thế nào? Khi nào thread kết thúc?
// Gợi ý: Sử dụng hàm pthread_create để tạo threads và pthread_join để chờ các threads hoàn thành.


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2
void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    printf("Thread %d: Hello from thread %d\n", thread_id, thread_id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS] = {1, 2};
    int result;

    for (int i = 0; i < NUM_THREADS; i++) {
        result = pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
        if (result != 0) {
            perror("Error creating thread");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        result = pthread_join(threads[i], NULL);
        if (result != 0) {
            perror("Error joining thread");
            return 1;
        }
    }
    printf("All threads completed.\n");
    return 0;
}