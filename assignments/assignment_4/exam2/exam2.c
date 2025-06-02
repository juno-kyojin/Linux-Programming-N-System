// BT2. Đồng Bộ Hóa Threads với Mutex
// Viết một chương trình sử dụng pthread và mutex để thực hiện các thao tác sau:
// Tạo một biến toàn cục counter khởi tạo bằng 0.
// Tạo ba threads, mỗi thread tăng giá trị của counter lên 1, 1.000.000 lần.
// Sử dụng mutex để đảm bảo rằng việc tăng counter được thực hiện an toàn.
// In ra giá trị cuối cùng của counter khi tất cả các threads hoàn thành.
// Hỏi: Tại sao cần mutex trong bài này? Điều gì xảy ra nếu bỏ mutex?
// Gợi ý: Sử dụng pthread_mutex_lock và pthread_mutex_unlock để khóa và mở khóa mutex khi truy cập vào counter.

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

long long counter = 0;
pthread_mutex_t mutex;

void* increment_counter(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        if (pthread_mutex_lock(&mutex) != 0) {
            perror("Mutex lock failed");
            exit(1);
        }
        counter++;
        if (pthread_mutex_unlock(&mutex) != 0) {
            perror("Mutex unlock failed");
            exit(1);
        }
    }
    return NULL;
}

int main() {
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex init failed");
        return 1;
    }

    pthread_t threads[3];
    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("Thread creation failed");
            pthread_mutex_destroy(&mutex);
            return 1;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Thread join failed");
            pthread_mutex_destroy(&mutex);
            return 1;
        }
    }

    printf("Final counter value: %lld\n", counter);
    pthread_mutex_destroy(&mutex);

    return 0;
}