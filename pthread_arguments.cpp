
#include <iostream>
#include <pthread.h>
#include <cstring>

struct Numbers {
    int a;
    int b;
    int c;
};

void* calculateSum(void* arg) {
    Numbers* nums = static_cast<Numbers*>(arg);

    int sum = nums->a + nums->b + nums->c;

    std::cout << "Перший потік: сума чисел = "
              << sum << std::endl;

    return nullptr;
}

void* calculateProduct(void* arg) {
    Numbers* nums = static_cast<Numbers*>(arg);

    int product = nums->a * nums->b * nums->c;

    std::cout << "Другий потік: добуток чисел = "
              << product << std::endl;

    return nullptr;
}

int main() {
    std::cout << "Лабораторна робота №6\n";
    std::cout << "Студент: Бойко Владислав Олександрович\n";

    Numbers nums = {2, 3, 4};

    std::cout << "Передані числа: "
              << nums.a << ", "
              << nums.b << ", "
              << nums.c << std::endl;

    pthread_t thread1, thread2;

    int result1 = pthread_create(
        &thread1, nullptr, calculateSum, &nums
    );

    if (result1 != 0) {
        std::cerr << "Помилка створення потоку 1: "
                  << std::strerror(result1) << std::endl;
        return 1;
    }

    int result2 = pthread_create(
        &thread2, nullptr, calculateProduct, &nums
    );

    if (result2 != 0) {
        std::cerr << "Помилка створення потоку 2: "
                  << std::strerror(result2) << std::endl;
        pthread_join(thread1, nullptr);
        return 1;
    }

    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    std::cout << "Обчислення завершено!" << std::endl;

    return 0;
}
