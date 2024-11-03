#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define FILEPATH "/tmp/file"
#define FILESIZE 4096

// ID дочернего процесса, который родитель будет использовать для отправки сигнала
pid_t child_pid;

// Обработчик сигнала, необходимый для дочернего процесса
void signal_handler(int sig) {
    // Ничего не делаем; просто ожидаем сигнала
}

int main() {
    int fd;
    char* mapped;

    // Открываем файл с правами на чтение и запись
    fd = open(FILEPATH, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file.");
        exit(1);
    }

    // Устанавливаем размер файла
    if (ftruncate(fd, FILESIZE) == -1) {
        perror("Error setting file size.");
        close(fd);
        exit(1);
    }

    // Отображаем файл
    mapped = (char*)mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Error mapping file.");
        close(fd);
        exit(1);
    }
    close(fd);

    // Создаём дочерний процесс
    child_pid = fork();
    if (child_pid == -1) {
        perror("Fork failed.");
        munmap(mapped, FILESIZE);
        exit(1);
    } else if (child_pid == 0) {
        // Дочерний процесс: установка обработчика для SIGUSR1
        signal(SIGUSR1, signal_handler);
        
        // Ожидаем сигнал от родительского процесса
        pause();

        // Запускаем дочерний процесс через execve
        char filesize_str[10];
        sprintf(filesize_str, "%d", FILESIZE);
        const char* args[] = {"./child", FILEPATH, filesize_str, NULL};

        execve("./child", (char* const*)args, NULL); 
        perror("execve failed.");
        exit(1);
    } else {
        // Родительский процесс

        // Ввод чисел
        char input_data[FILESIZE];
        printf("Введите числа через пробел: ");
        fgets(input_data, sizeof(input_data), stdin);

        // Копируем данные в отображаемый файл
        memcpy(mapped, input_data, FILESIZE);

        // Отправляем сигнал SIGUSR1 дочернему процессу
        kill(child_pid, SIGUSR1);

        // Ожидаем завершения дочернего процесса
        wait(NULL);

        // Освобождаем память
        munmap(mapped, FILESIZE);
    }

    return 0;
}
