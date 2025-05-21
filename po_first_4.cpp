#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>

int main() {
    using hr_clock = std::chrono::high_resolution_clock;

    const int numThreads = 12;

    std::vector<std::pair<int,int>> sizes = {
        {   100, 10000000 },
        {  1000,  1000000 },
        { 10000,   100000 },
        {100000,    10000 },
        {1000000,    1000 },
        {10000000,     100 }
    };

    std::cout << std::setw(15) << "Size (n×m)"
              << " | " << std::setw(15) << "Time (12 threads, μs)" << "\n";
    std::cout << "------------------------------------------------\n";

    for (auto [n, m] : sizes) {
        int** matrix = new int*[n];
        for (int i = 0; i < n; ++i) {
            matrix[i] = new int[m];
            for (int j = 0; j < m; ++j) {
                matrix[i][j] = j + 1;
            }
        }

        std::vector<long long> partialSums(numThreads, 0LL);
        std::vector<std::thread> threads;
        threads.reserve(numThreads);
        int chunkSize = n / numThreads;

        auto t0 = hr_clock::now();

        for (int t = 0; t < numThreads; ++t) {
            int start = t * chunkSize;
            int end   = (t + 1 == numThreads ? n : (t + 1) * chunkSize);
            threads.emplace_back(
                [&, start, end, t]() {
                    long long local = 0;
                    for (int i = start; i < end; ++i)
                        for (int j = 0; j < m; ++j)
                            local += matrix[i][j];
                    partialSums[t] = local;
                }
            );
        }

        for (auto& th : threads) th.join();

        auto t1 = hr_clock::now();
        long long duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

        std::ostringstream label;
        label << n << "×" << m;
        std::cout << std::setw(15) << label.str()
                  << " | " << std::setw(15) << duration << "\n";

        for (int i = 0; i < n; ++i) delete[] matrix[i];
        delete[] matrix;
    }

    return 0;
}
