
#include <iostream>
#include <pthread.h>
#include <cstring>
#include <new>

struct Numbers {
    int a;
    int b;
};

void* calculateSum(void* arg) {
    Numbers* nums = static_cast<Numbers*>(arg);

    int* result = new (std::nothrow) int;
    if (result == nullptr) {
        pthread_exit(nullptr);
    }

    *result = nums->a + nums->b;
    pthread_exit(result);
}

void* calculateProduct(void* arg) {
    Numbers* nums = static_cast<Numbers*>(arg);

    int* result = new (std::nothrow) int;
    if (result == nullptr) {
        pthread_exit(nullptr);
    }

    *result = nums->a * nums->b;
    pthread_exit(result);
}

int main() {
    std::cout << "Лабораторна робота №6\n";
    std::cout << "Студент: Бойко Владислав Олександрович\n";

    Numbers nums = {5, 7};

    std::cout << "Числа: " << nums.a
              << " і " << nums.b << "\n";

    pthread_t thread1, thread2;

    int rc1 = pthread_create(
        &thread1, nullptr, calculateSum, &nums
    );

    if (rc1 != 0) {
        std::cerr << "Помилка потоку 1: "
                  << std::strerror(rc1) << "\n";
        return 1;
    }

    int rc2 = pthread_create(
        &thread2, nullptr, calculateProduct, &nums
    );

    if (rc2 != 0) {
        std::cerr << "Помилка потоку 2: "
                  << std::strerror(rc2) << "\n";
        void* firstResult = nullptr;
        pthread_join(thread1, &firstResult);
        delete static_cast<int*>(firstResult);
        return 1;
    }

    void* sumPtr = nullptr;
    void* productPtr = nullptr;

    int join1 = pthread_join(thread1, &sumPtr);
    int join2 = pthread_join(thread2, &productPtr);

    if (join1 != 0 || join2 != 0 ||
        sumPtr == nullptr || productPtr == nullptr) {
        std::cerr << "Помилка отримання результатів.\n";
        delete static_cast<int*>(sumPtr);
        delete static_cast<int*>(productPtr);
        return 1;
    }

    int sum = *static_cast<int*>(sumPtr);
    int product = *static_cast<int*>(productPtr);

    delete static_cast<int*>(sumPtr);
    delete static_cast<int*>(productPtr);

    std::cout << "Сума: " << sum << "\n";
    std::cout << "Добуток: " << product << "\n";

    if (sum > product) {
        std::cout << "Сума більша за добуток.\n";
    } else if (sum < product) {
        std::cout << "Добуток більший за суму.\n";
    } else {
        std::cout << "Сума дорівнює добутку.\n";
    }

    return 0;
}
