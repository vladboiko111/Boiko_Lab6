
#include <iostream>
#include <pthread.h>
#include <cstring>

void* firstThread(void*) {
    std::cout << "Перший потік: Привіт від потоку №1!"
              << std::endl;
    return nullptr;
}

void* secondThread(void*) {
    std::cout << "Другий потік: Привіт від потоку №2!"
              << std::endl;
    return nullptr;
}

int main() {
    pthread_t thread1, thread2;

    std::cout << "Лабораторна робота №6\n";
    std::cout << "Студент: Бойко Владислав Олександрович\n";

    int result1 = pthread_create(
        &thread1, nullptr, firstThread, nullptr
    );

    if (result1 != 0) {
        std::cerr << "Помилка створення потоку 1: "
                  << std::strerror(result1) << std::endl;
        return 1;
    }

    int result2 = pthread_create(
        &thread2, nullptr, secondThread, nullptr
    );

    if (result2 != 0) {
        std::cerr << "Помилка створення потоку 2: "
                  << std::strerror(result2) << std::endl;
        pthread_join(thread1, nullptr);
        return 1;
    }

    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    std::cout << "Обидва потоки завершили роботу."
              << std::endl;

    return 0;
}
