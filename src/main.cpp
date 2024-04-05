#include <iostream>
#include <chrono> // For chrono time

#include <pika/execution.hpp>
#include <pika/init.hpp>

#include <fmt/printf.h>

#include "Matrix.hpp"

int main(int argc, char* argv[]) {
    // Default values for command-line arguments
    int n = 3; // Number of rows of matrix M_1
    int k = 3; // Number of columns of matrix M_1 = Number of rows of matrix M_2
    int m = 3; // Number of columns of matrix M_2
    int l = 1; // Lower bound on randomized values
    int r = 10; // Upper bound on randomized values
    unsigned int s = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()); // Seed for rand

    // Parse command-line arguments
    if (argc > 1) {
        n = std::atoi(argv[1]);
    }
    if (argc > 2) {
        k = std::atoi(argv[2]);
    }
    if (argc > 3) {
        m = std::atoi(argv[3]);
    }
    if (argc > 4) {
        l = std::atoi(argv[4]);
    }
    if (argc > 5) {
        r = std::atoi(argv[5]);
    }
    if (argc > 6) {
        s = std::atoi(argv[6]);
    }

    // Set random seed
    srand(s);

    // Initialize matrices with randomized values
    assert(n > 0 && "Dimension n must be positive");
    assert(k > 0 && "Dimension k must be positive");
    assert(m > 0 && "Dimension m must be positive");

    // Instantiate matrices A, B, and C
    Matrix<int> A(n, k);
    Matrix<int> B(k, m);
    Matrix<int> C(n, m);

    // Fill matrices A and B with random values
    assert(l >= std::numeric_limits<int>::min() && "Lower bound l exceeds int limits");
    assert(r <= std::numeric_limits<int>::max() && "Upper bound r exceeds int limits");
    assert(l <= r && "Lower bound l exceeds upper bound r");

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            A(i, j) = rand() % (r - l + 1) + l;
        }
    }

    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < m; ++j) {
            B(i, j) = rand() % (r - l + 1) + l;
        }
    }

    // Perform matrix multiplication using custom operator*
    // pika::this_thread::experimental::sync_wait(
    //     pika::execution::experimental::schedule(
    //         pika::execution::experimental::thread_pool_scheduler{}) |
    //     pika::execution::experimental::then([&]() {
    C = A * B; // Perform matrix multiplication using @ operator
    //     })
    // );

    // Output matrices A, B, and result matrix C
    std::cout << "Matrix A:" << std::endl;
    std::cout << A << std::endl;

    std::cout << "Matrix B:" << std::endl;
    std::cout << B << std::endl;

    std::cout << "Result Matrix C (A * B):" << std::endl;
    std::cout << C << std::endl;

    return 0;
}