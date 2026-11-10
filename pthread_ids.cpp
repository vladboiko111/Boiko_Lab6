
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

struct ThreadData {
    int number;
    pthread_t id;
};

void* threadFunction(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);

    data->id = pthread_self();

    // Затримка, щоб обидва потоки існували одночасно
    sleep(1);

    return nullptr;
}

int main() {
    std::cout << "Лабораторна робота №6\n";
    std::cout << "Студент: Бойко Владислав Олександрович\n";

    pthread_t thread1, thread2;

    ThreadData data1{1, {}};
    ThreadData data2{2, {}};

    int rc1 = pthread_create(
        &thread1, nullptr, threadFunction, &data1
    );

    if (rc1 != 0) {
        std::cerr << "Помилка потоку 1: "
                  << std::strerror(rc1) << "\n";
        return 1;
    }

    int rc2 = pthread_create(
        &thread2, nullptr, threadFunction, &data2
    );

    if (rc2 != 0) {
        std::cerr << "Помилка потоку 2: "
                  << std::strerror(rc2) << "\n";
        pthread_join(thread1, nullptr);
        return 1;
    }

    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    std::cout << "ID першого потоку: "
              << static_cast<unsigned long>(data1.id)
              << "\n";

    std::cout << "ID другого потоку: "
              << static_cast<unsigned long>(data2.id)
              << "\n";

    if (pthread_equal(data1.id, data2.id)) {
        std::cout << "Ідентифікатори однакові!\n";
    } else {
        std::cout << "Ідентифікатори різні!\n";
    }

    std::cout << "Перевірка завершена.\n";

    return 0;
}
