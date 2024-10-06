#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <pipe_read_fd> <output_file>\n", argv[0]);
        return 1;
    }

    // Получаем дескриптор pipe для чтения
    int pipe_read_fd = atoi(argv[1]);

    // Открываем файл для записи результата
    int file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file == -1) {
        perror("open");
        return 1;
    }

    // Чтение данных из pipe
    char buffer[256];
    read(pipe_read_fd, buffer, sizeof(buffer));
    close(pipe_read_fd);

    // Складываем числа
    int sum = 0;
    char *token = strtok(buffer, " ");
    while (token != NULL) {
        sum += atoi(token);
        token = strtok(NULL, " ");
    }

    // Записываем результат в файл
    dprintf(file, "%d", sum);
    close(file);

    return 0;
}
