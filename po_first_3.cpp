#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>

int main() {
    using hr_clock = std::chrono::high_resolution_clock;

    const int n = 10000;
    const int m = 10000;

    const int physCores = 6;
    const int logCores  = 12;

    std::vector<int> threadCounts = {
        physCores / 2,
        physCores,
        logCores,
        logCores * 2,
        logCores * 4,
        logCores * 8,
        logCores * 16
    };

    int** matrix = new int*[n];
    for (int i = 0; i < n; ++i) {
        matrix[i] = new int[m];
        for (int j = 0; j < m; ++j) {
            matrix[i][j] = j + 1;
        }
    }

    std::cout << std::setw(8) << "Threads" << " | "
              << std::setw(10) << "Time (µs)" << "\n";
    std::cout << "-----------------------\n";

    for (int numThreads : threadCounts) {
        std::vector<long long> partialSums(numThreads, 0LL);
        std::vector<std::thread> threads;
        threads.reserve(numThreads);

        int chunkSize = n / numThreads;

        auto t0 = hr_clock::now();

        for (int t = 0; t < numThreads; ++t) {
            int start = t * chunkSize;
            int end   = (t + 1 == numThreads) ? n : (t + 1) * chunkSize;

            threads.emplace_back(
                [&, start, end, t]() {
                    long long localSum = 0;
                    for (int i = start; i < end; ++i) {
                        for (int j = 0; j < m; ++j) {
                            localSum += matrix[i][j];
                        }
                    }
                    partialSums[t] = localSum;
                }
            );
        }

        for (auto& th : threads) {
            th.join();
        }

        long long totalSum = 0;
        for (auto s : partialSums) {
            totalSum += s;
        }

        auto t1 = hr_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

        std::cout << std::setw(8) << numThreads << " | "
                  << std::setw(10) << duration << "\n";
    }

    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}
