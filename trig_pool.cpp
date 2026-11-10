
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <array>
#include <atomic>
#include <cmath>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

using namespace std;

enum class Function {
    SIN, COS, TG, CTG
};

struct Task {
    int angle;
    Function function;
};

struct Result {
    double value = 0;
    bool defined = true;
};

struct ThreadPoolData {
    vector<Task> tasks;
    vector<Result> results;
    atomic<size_t> nextTask{0};
};

struct WorkerData {
    ThreadPoolData* pool;
    int number;
    int completed = 0;
};

void* workerFunction(void* argument) {
    WorkerData* worker =
        static_cast<WorkerData*>(argument);

    ThreadPoolData* pool = worker->pool;

    while (true) {
        size_t index = pool->nextTask.fetch_add(1);

        if (index >= pool->tasks.size()) {
            break;
        }

        Task task = pool->tasks[index];
        Result result;

        const double PI = acos(-1.0);
        double radians = task.angle * PI / 180.0;

        switch (task.function) {
            case Function::SIN:
                result.value = sin(radians);
                break;

            case Function::COS:
                result.value = cos(radians);
                break;

            case Function::TG:
                if (task.angle == 90) {
                    result.defined = false;
                } else {
                    result.value =
                        sin(radians) / cos(radians);
                }
                break;

            case Function::CTG:
                if (task.angle == 0 ||
                    task.angle == 180) {
                    result.defined = false;
                } else {
                    result.value =
                        cos(radians) / sin(radians);
                }
                break;
        }

        if (result.defined &&
            fabs(result.value) < 1e-12) {
            result.value = 0;
        }

        pool->results[index] = result;
        worker->completed++;

        // Невелика затримка для демонстрації
        // паралельного виконання завдань
        usleep(3000);
    }

    return nullptr;
}

string formatResult(const Result& result) {
    if (!result.defined) {
        return "N/A";
    }

    ostringstream output;
    output << fixed << setprecision(4)
           << result.value;

    return output.str();
}

int main() {
    cout << "Лабораторна робота №6\n";
    cout << "Студент: Бойко Владислав Олександрович\n";
    cout << "Індивідуальне завдання: пул потоків\n\n";

    ThreadPoolData pool;

    for (int angle = 0; angle <= 180; angle += 15) {
        pool.tasks.push_back({angle, Function::SIN});
        pool.tasks.push_back({angle, Function::COS});
        pool.tasks.push_back({angle, Function::TG});
        pool.tasks.push_back({angle, Function::CTG});
    }

    pool.results.resize(pool.tasks.size());

    const int THREAD_COUNT = 4;

    array<pthread_t, THREAD_COUNT> threads;
    array<WorkerData, THREAD_COUNT> workers;

    cout << "Кількість потоків: "
         << THREAD_COUNT << "\n";

    cout << "Кількість завдань: "
         << pool.tasks.size() << "\n\n";

    int created = 0;

    for (int i = 0; i < THREAD_COUNT; i++) {
        workers[i] = {&pool, i + 1, 0};

        int rc = pthread_create(
            &threads[i],
            nullptr,
            workerFunction,
            &workers[i]
        );

        if (rc != 0) {
            cerr << "Помилка створення потоку: "
                 << strerror(rc) << "\n";

            for (int j = 0; j < created; j++) {
                pthread_join(threads[j], nullptr);
            }

            return 1;
        }

        created++;
    }

    for (int i = 0; i < created; i++) {
        int rc = pthread_join(threads[i], nullptr);

        if (rc != 0) {
            cerr << "Помилка pthread_join: "
                 << strerror(rc) << "\n";
            return 1;
        }
    }

    cout << left
         << setw(8)  << "Кут"
         << setw(12) << "sin"
         << setw(12) << "cos"
         << setw(12) << "tg"
         << setw(12) << "ctg"
         << "\n";

    cout << string(56, '-') << "\n";

    for (size_t i = 0; i < pool.results.size(); i += 4) {
        int angle = pool.tasks[i].angle;

        cout << left << setw(8) << angle;

        for (int j = 0; j < 4; j++) {
            cout << setw(12)
                 << formatResult(pool.results[i + j]);
        }

        cout << "\n";
    }

    cout << "\nРобота потоків:\n";

    int total = 0;

    for (int i = 0; i < THREAD_COUNT; i++) {
        cout << "Потік " << workers[i].number
             << ": " << workers[i].completed
             << " завдань\n";

        total += workers[i].completed;
    }

    cout << "\nВсього виконано: "
         << total << " завдань\n";

    cout << "Обчислення завершено успішно!\n";

    return 0;
}
