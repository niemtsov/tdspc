#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <windows.h>

using namespace std;

// Функція для обчислення кількості "градин" (кроків до 1)
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

// Функція, яку виконують потоки (без синхронізації)
void process_range(int start, int end, long long& local_sum) {
    long long sum = 0;
    for (int i = start; i < end; i++) {
        sum += collatz_steps(i);
    }
    local_sum = sum; 
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    const int N = 10000000;
    const int num_threads = thread::hardware_concurrency();

    cout << "Використовується потоків: " << num_threads << endl;

    auto start_time = chrono::high_resolution_clock::now();

    vector<thread> threads;
    vector<long long> partial_sums(num_threads, 0);

    int chunk_size = N / num_threads;
    int start = 1;

    // Створюємо потоки
    for (int t = 0; t < num_threads; t++) {
        int end = (t == num_threads - 1) ? N + 1 : start + chunk_size;
        threads.emplace_back(process_range, start, end, ref(partial_sums[t]));
        start = end;
    }

    // Очікуємо завершення всіх потоків
    for (auto& th : threads) {
        th.join();
    }

    // Об'єднуємо часткові результати
    long long total_steps = 0;
    for (auto sum : partial_sums) {
        total_steps += sum;
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

// Час виконання деколи на декілька відсотків краще за попередню реалізацію, деколи майже не відрізняєтся. Загалом результати майже однакові, присутня деяка несуттєва похибка