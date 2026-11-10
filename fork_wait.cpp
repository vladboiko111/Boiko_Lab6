
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>

int main() {
    std::cout << "Лабораторна робота №6\n";
    std::cout << "Студент: Бойко Владислав Олександрович\n";
    std::cout << std::flush;

    pid_t pid = fork();

    if (pid < 0) {
        perror("Помилка fork");
        return 1;
    }

    if (pid == 0) {
        std::cout << "Дочірній процес почав роботу.\n";
        std::cout << "PID дочірнього процесу: "
                  << getpid() << std::endl;

        for (int i = 1; i <= 3; i++) {
            std::cout << "Дочірній процес: крок "
                      << i << " із 3" << std::endl;
            sleep(1);
        }

        std::cout << "Дочірній процес завершив роботу."
                  << std::endl;

        return 0;
    } else {
        std::cout << "Батьківський процес почав роботу.\n";
        std::cout << "PID батьківського процесу: "
                  << getpid() << std::endl;

        std::cout << "Батьківський процес очікує..."
                  << std::endl;

        int status;
        pid_t result = wait(&status);

        if (result == -1) {
            perror("Помилка wait");
            return 1;
        }

        std::cout << "Дочірній процес завершився!"
                  << std::endl;

        if (WIFEXITED(status)) {
            std::cout << "Код завершення: "
                      << WEXITSTATUS(status) << std::endl;
        }

        std::cout << "Батьківський процес продовжив роботу."
                  << std::endl;
    }

    return 0;
}
