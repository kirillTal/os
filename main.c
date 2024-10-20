#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int** read_data(const char* path, size_t* k, size_t* m) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fscanf(file, "%lu %lu", k, m);
    int** arrays = (int**)malloc(sizeof(int*) * (*k));
    if (arrays == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return NULL;
    }

    for (size_t i = 0; i < *k; ++i) {
        arrays[i] = (int*)malloc(sizeof(int) * (*m));
        if (arrays[i] == NULL) {
            perror("Memory allocation error");
            fclose(file);
            return NULL;
        }

        for (size_t j = 0; j < *m; ++j) {
            fscanf(file, "%d", &arrays[i][j]);
        }
    }

    fclose(file);
    return arrays;
}

typedef struct {
    int** arrays;
    size_t num_arrays;
    size_t size;
    int* result;
    pthread_mutex_t* mutex;
} ThreadData;

int get_element_from_2d_array(int** arrs, size_t i, size_t j) {
    // return arrs[i][j];
    return rand() % 201 - 100;
}

void* sum_array(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int local_sum = 0;

    for (size_t i = 0; i < data->num_arrays; ++i) {
        for (size_t j = 0; j < data->size; ++j) {
            local_sum += get_element_from_2d_array(data->arrays, i, j);
        }
    }

    pthread_mutex_lock(data->mutex);
    *(data->result) += local_sum;
    pthread_mutex_unlock(data->mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <number of threads> <number of arrays> <length of arrays>\n", argv[0]);
        return 1;
    }

    size_t threads_numbers = atoi(argv[1]);

    size_t k = atoll(argv[2]);
    size_t m = atoll(argv[3]);

    int result = 0;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    size_t num_threads = (k < threads_numbers) ? k : threads_numbers;
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    ThreadData* th_data = (ThreadData*)malloc(sizeof(ThreadData) * num_threads);

    size_t arrays_number_for_thread = (k / num_threads) + (k % num_threads);
    size_t rest_arrays = k;
    for (size_t i = 0; i < num_threads; ++i) {
        // th_data[i].arrays = arrays + i * arrays_number_for_thread;
        th_data[i].arrays = NULL;
        if (rest_arrays < arrays_number_for_thread) {
            th_data[i].num_arrays = rest_arrays;
        } else {
            th_data[i].num_arrays = arrays_number_for_thread;
        }

        th_data[i].size = m;
        th_data[i].result = &result;
        th_data[i].mutex = &mutex;
        if (pthread_create(&threads[i], NULL, sum_array, &th_data[i]) != 0) {
            perror("Error creating thread");
            return 1;
        }
    }

    // Ожидание завершения всех созданных потоков
    for (size_t i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("%d", result);

    pthread_mutex_destroy(&mutex);
    free(th_data);
    free(threads);

    return 0;
}
