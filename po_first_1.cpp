#include <iostream>
#include <chrono>

int main() {
	const int n = 10000;
	const int m = 10000;

	int** matrix = new int*[n];
	for(int i = 0; i < n; ++i) {
    	matrix[i] = new int[m];
	}

	for(int i = 0; i < n; ++i) {
    	for(int j = 0; j < m; ++j) {
        	matrix[i][j] = j + 1;
    	}
	}

	auto start_time = std::chrono::high_resolution_clock::now();

	long long sum = 0;
	for(int i = 0; i < n; ++i) {
    	for(int j = 0; j < m; ++j) {
        	sum += matrix[i][j];
    	}
	}

	auto end_time = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	std::cout << "Sum of array elements: " << sum << "\n";
	std::cout << "Sequential Execution Time: "
          	<< duration.count() << " microseconds\n";

	for(int i = 0; i < n; ++i) {
    	delete[] matrix[i];
	}
	delete[] matrix;

	return 0;
}
