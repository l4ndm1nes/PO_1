#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

int main() {
	const int n = 10000;
	const int m = 10000;

	int** matrix = new int*[n];
	for(int i = 0; i < n; ++i) {
    	matrix[i] = new int[m];
    	for(int j = 0; j < m; ++j) {
        	matrix[i][j] = j + 1;
    	}
	}

	unsigned int numThreads = std::thread::hardware_concurrency();
	if(numThreads == 0) numThreads = 1;

	std::vector<long long> partialSums(numThreads, 0LL);

	std::vector<std::thread> threads;
	threads.reserve(numThreads);

	int chunkSize = n / numThreads;

	auto t0 = std::chrono::high_resolution_clock::now();

	for(unsigned int t = 0; t < numThreads; ++t) {
    	int start = t * chunkSize;
    	int end   = (t + 1 == numThreads) ? n : (t + 1) * chunkSize;

    	threads.emplace_back(
        	[&, start, end, t]() {
            	long long localSum = 0;
            	for(int i = start; i < end; ++i) {
                	for(int j = 0; j < m; ++j) {
                    	localSum += matrix[i][j];
                	}
            	}
            	partialSums[t] = localSum;
        	}
    	);
	}

	for(auto& th : threads) {
    	th.join();
	}

	long long totalSum = 0;
	for(auto s : partialSums) {
    	totalSum += s;
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

	std::cout << "Sum of elements in the array: " << totalSum << "\n";
	std::cout << "Parallel (" << numThreads << " threads) Time: "
          	<< duration.count() << " microseconds\n";

	for(int i = 0; i < n; ++i) delete[] matrix[i];
	delete[] matrix;

	return 0;
}
