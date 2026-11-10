
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstdlib>

extern char **environ;

int main() {
    std::cout << "Лабораторна робота №6\n";
    std::cout << "Студент: Бойко Владислав Олександрович\n";
    std::cout << "Запуск команди ls через fork() та execve()\n";
    std::cout << std::flush;

    pid_t pid = fork();

    if (pid < 0) {
        perror("Помилка fork");
        return 1;
    }

    if (pid == 0) {
        std::cout << "Дочірній процес запускає ls\n";
        std::cout << "PID дочірнього процесу: "
                  << getpid() << std::endl;

        char program[] = "ls";
        char option[] = "-l";
        char directory[] = ".";

        char* args[] = {
            program,
            option,
            directory,
            nullptr
        };

        execve("/bin/ls", args, environ);

        perror("Помилка execve");
        _exit(127);
    } else {
        std::cout << "Батьківський процес працює\n";
        std::cout << "PID батьківського процесу: "
                  << getpid() << std::endl;

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("Помилка waitpid");
            return 1;
        }

        std::cout << "Дочірній процес завершився\n";

        if (WIFEXITED(status)) {
            std::cout << "Код завершення: "
                      << WEXITSTATUS(status) << std::endl;
        }
    }

    return 0;
}
