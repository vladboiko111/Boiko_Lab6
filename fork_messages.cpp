
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>

int main() {
    std::cout << "Лабораторна робота №6\n";
    std::cout << "Студент: Бойко Владислав Олександрович\n"
              << std::flush;

    pid_t pid = fork();

    if (pid < 0) {
        perror("Помилка fork");
        return 1;
    }

    if (pid == 0) {
        std::cout << "Дочірній процес працює!\n";
        std::cout << "PID дочірнього процесу: "
                  << getpid() << std::endl;

        sleep(2);
    } else {
        std::cout << "Батьківський процес працює!\n";
        std::cout << "PID батьківського процесу: "
                  << getpid() << std::endl;

        waitpid(pid, nullptr, 0);
    }

    return 0;
}
