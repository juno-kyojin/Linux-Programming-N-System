// BT2. Đồng Bộ Hóa Threads với Mutex
// Viết một chương trình sử dụng pthread và mutex để thực hiện các thao tác sau:
// Tạo một biến toàn cục counter khởi tạo bằng 0.
// Tạo ba threads, mỗi thread tăng giá trị của counter lên 1, 1.000.000 lần.
// Sử dụng mutex để đảm bảo rằng việc tăng counter được thực hiện an toàn.
// In ra giá trị cuối cùng của counter khi tất cả các threads hoàn thành.
// Hỏi: Tại sao cần mutex trong bài này? Điều gì xảy ra nếu bỏ mutex?
// Gợi ý: Sử dụng pthread_mutex_lock và pthread_mutex_unlock để khóa và mở khóa mutex khi truy cập vào counter.


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Global variables
int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread function
void* increment_counter(void* arg) {
    for(int i = 0; i < 1000000; i++) {
        // Lock mutex before accessing counter
        pthread_mutex_lock(&mutex);
        counter++;
        // Unlock mutex after accessing counter
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[3];
    
    // Create 3 threads
    for(int i = 0; i < 3; i++) {
        if(pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }
    
    // Wait for all threads to complete
    for(int i = 0; i < 3; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }
    
    // Print final counter value
    printf("Final counter value: %d\n", counter);
    
    // Destroy mutex
    pthread_mutex_destroy(&mutex);
    
    return 0;
}

