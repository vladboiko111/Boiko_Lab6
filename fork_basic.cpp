
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstdio>

int main() {
    std::cout << "Лабораторна робота №6" << std::endl;
    std::cout << "Студент: Бойко Владислав Олександрович"
              << std::endl;

    pid_t pid = fork();

    if (pid < 0) {
        perror("Помилка fork");
        return 1;
    }

    std::cout << "Процес запущено!" << std::endl;
    std::cout << "PID процесу: " << getpid() << std::endl;
    std::cout << "PID батьківського процесу: "
              << getppid() << std::endl;

    sleep(30);

    return 0;
}
