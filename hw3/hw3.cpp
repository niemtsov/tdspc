#include <iostream>
#include <vector>
#include <future>
#include <numeric>
#include <chrono>
#include <windows.h>

using namespace std;

// Функція для обчислення кількості "градин"
int collatz_steps(unsigned long long n) {
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

// Завдання для пулу потоків
long long process_range(int start, int end) {
    long long local_sum = 0;
    for (int i = start; i < end; i++) {
        local_sum += collatz_steps(i);
    }
    return local_sum;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    const int N = 10000000;
    const int num_threads = thread::hardware_concurrency(); // кількість потоків у пулі
    cout << "Використовується потоків: " << num_threads << endl;

    // Формуємо діапазони для кожного потоку
    vector<pair<int, int>> ranges;
    int chunk_size = N / num_threads;
    int start = 1;
    for (int t = 0; t < num_threads; t++) {
        int end = (t == num_threads - 1) ? N + 1 : start + chunk_size;
        ranges.emplace_back(start, end);
        start = end;
    }

    auto start_time = chrono::high_resolution_clock::now();

    // Запуск завдань через ThreadPool (std::async)
    vector<future<long long>> futures;
    for (auto& r : ranges) {
        futures.push_back(async(launch::async, process_range, r.first, r.second));
    }

    long long total_steps = 0;
    for (auto& f : futures) {
        total_steps += f.get();
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;

    double average_steps = static_cast<double>(total_steps) / N;
    cout << "Середня кількість кроків: " << average_steps << endl;
    cout << "Час виконання: " << duration.count() << " секунд" << endl;

    cout << "\nНатисніть Enter, щоб завершити...";
    cin.get();
    return 0;
}
