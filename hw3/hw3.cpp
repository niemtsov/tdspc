#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <windows.h> 

using namespace std;

// Функція для обчислення кількості "градин" (кроків до 1)
int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0)
            n /= 2;
        else
            n = 3 * n + 1;
        steps++;
    }
    return steps;
}

// Функція, яку виконують потоки
void process_range(int start, int end, atomic<long long>& total_steps) {
    long long local_sum = 0;
    for (int i = start; i < end; i++) {
        local_sum += collatz_steps(i);
    }
    total_steps += local_sum; // атомарно додаємо результат потоку
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    const int N = 10000000;
    const int num_threads = thread::hardware_concurrency(); // кількість доступних ядер

    cout << "Використовується потоків: " << num_threads << endl;

    vector<thread> threads;
    atomic<long long> total_steps = 0;

    int chunk_size = N / num_threads;
    int start = 1;

    for (int t = 0; t < num_threads; t++) {
        int end = (t == num_threads - 1) ? N + 1 : start + chunk_size;
        threads.emplace_back(process_range, start, end, ref(total_steps));
        start = end;
    }

    for (auto& th : threads) {
        th.join();
    }

    double average_steps = static_cast<double>(total_steps) / N;
    cout << "Середня кількість кроків: " << average_steps << endl;

    cout << "\nНатисніть Enter, щоб завершити...";
    cin.get();

    return 0;
}
