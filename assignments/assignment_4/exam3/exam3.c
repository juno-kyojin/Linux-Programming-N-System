// BT3. Sử Dụng Condition Variables
// Viết một chương trình có hai threads: producer và consumer như sau:
// Producer: Tạo ra một số ngẫu nhiên từ 1 đến 10 và lưu vào một biến toàn cục data.
// Consumer: Đợi producer cung cấp dữ liệu, sau đó đọc và in ra dữ liệu đó.
// Sử dụng pthread_cond_wait và pthread_cond_signal để đồng bộ hóa giữa producer và consumer, 
// đảm bảo rằng consumer chỉ đọc dữ liệu khi producer đã cung cấp xong.
// Lặp lại quá trình trên 10 lần và in ra tất cả các giá trị do consumer đọc được.
// Gợi ý: Sử dụng pthread_cond_wait để cho consumer đợi cho đến khi có tín hiệu từ producer rằng dữ liệu đã sẵn sàng.


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

// Biến toàn cục
int data = 0;              // Dữ liệu do producer tạo
int data_ready = 0;        // Cờ: 0 = chưa sẵn sàng, 1 = sẵn sàng
int count = 0;             // Đếm số lần lặp
int values[10];            // Lưu các giá trị consumer đọc được
pthread_mutex_t mutex;      // Mutex bảo vệ biến chung
pthread_cond_t cond;        // Condition variable để đồng bộ hóa

// Hàm producer
void* producer(void* arg) {
    while (count < 10) {
        pthread_mutex_lock(&mutex);
        // Chờ nếu consumer chưa đọc dữ liệu
        while (data_ready == 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        // Tạo số ngẫu nhiên từ 1 đến 10
        data = (rand() % 10) + 1;
        data_ready = 1;
        count++;
        printf("Producer created: %d\n", data);
        // Gửi tín hiệu cho consumer
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Hàm consumer
void* consumer(void* arg) {
    while (count < 10) {
        pthread_mutex_lock(&mutex);
        // Chờ nếu dữ liệu chưa sẵn sàng
        while (data_ready == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        // Đọc và lưu dữ liệu
        values[count - 1] = data;
        data_ready = 0;
        printf("Consumer read: %d\n", data);
        // Gửi tín hiệu cho producer
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    // Khởi tạo seed cho số ngẫu nhiên
    srand(time(NULL));
    
    // Khởi tạo mutex và condition variable
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex init failed");
        return 1;
    }
    if (pthread_cond_init(&cond, NULL) != 0) {
        perror("Condition variable init failed");
        return 1;
    }

    // Tạo hai thread
    pthread_t prod_thread, cons_thread;
    if (pthread_create(&prod_thread, NULL, producer, NULL) != 0) {
        perror("Producer thread creation failed");
        return 1;
    }
    if (pthread_create(&cons_thread, NULL, consumer, NULL) != 0) {
        perror("Consumer thread creation failed");
        return 1;
    }

    // Chờ các thread hoàn thành
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // In tất cả giá trị consumer đọc được
    printf("\nAll values read by consumer:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");

    // Hủy mutex và condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}