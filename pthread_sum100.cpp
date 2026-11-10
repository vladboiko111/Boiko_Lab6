
#include <iostream>
#include <pthread.h>
#include <cstring>
#include <new>

void* calculateSum(void*) {
    int* result = new (std::nothrow) int;

    if (result == nullptr) {
        pthread_exit(nullptr);
    }

    *result = 0;

    for (int i = 1; i <= 100; i++) {
        *result += i;
    }

    pthread_exit(result);
}

int main() {
    std::cout << "Лабораторна робота №6\n";
    std::cout << "Студент: Бойко Владислав Олександрович\n";

    pthread_t thread;

    int rc = pthread_create(
        &thread, nullptr, calculateSum, nullptr
    );

    if (rc != 0) {
        std::cerr << "Помилка створення потоку: "
                  << std::strerror(rc) << "\n";
        return 1;
    }

    std::cout << "Потік створено успішно.\n";

    if (pthread_equal(pthread_self(), thread)) {
        std::cerr << "Помилка: потік не може очікувати сам себе!\n";
        return 1;
    }

    std::cout << "Перевірка: потік не синхронізується сам із собою.\n";

    void* resultPtr = nullptr;

    rc = pthread_join(thread, &resultPtr);

    if (rc != 0) {
        std::cerr << "Помилка pthread_join: "
                  << std::strerror(rc) << "\n";
        return 1;
    }

    if (resultPtr == nullptr) {
        std::cerr << "Помилка отримання результату.\n";
        return 1;
    }

    int sum = *static_cast<int*>(resultPtr);

    delete static_cast<int*>(resultPtr);

    std::cout << "Сума перших 100 натуральних чисел: "
              << sum << "\n";

    std::cout << "Результат успішно отримано з потоку.\n";

    return 0;
}
