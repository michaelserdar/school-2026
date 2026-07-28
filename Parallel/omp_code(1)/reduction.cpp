#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    const int N = 8;
    std::vector<int> A = {3, 1, 4, 1, 5, 9, 2, 6};

    int sum    = 0;
    int product = 1;
    int maxval = A[0];
    int minval = A[0];

    // ── Sum ───────────────────────────────────────────────────────────────
    #pragma omp parallel for reduction(+:sum) num_threads(4)
    for (int i = 0; i < N; i++)
        sum += A[i];

    // ── Product ───────────────────────────────────────────────────────────
    #pragma omp parallel for reduction(*:product) num_threads(4)
    for (int i = 0; i < N; i++)
        product *= A[i];

    // ── Max ───────────────────────────────────────────────────────────────
    #pragma omp parallel for reduction(max:maxval) num_threads(4)
    for (int i = 0; i < N; i++)
        if (A[i] > maxval) maxval = A[i];

    // ── Min ───────────────────────────────────────────────────────────────
    #pragma omp parallel for reduction(min:minval) num_threads(4)
    for (int i = 0; i < N; i++)
        if (A[i] < minval) minval = A[i];

    // ── Print results ─────────────────────────────────────────────────────
    std::cout << "Array: ";
    for (int x : A) std::cout << x << " ";
    std::cout << "\n\n";

    std::cout << "sum     = " << sum     << "  (serial check: 31)\n";
    std::cout << "product = " << product << "  (serial check: 6480)\n";
    std::cout << "max     = " << maxval  << "  (serial check: 9)\n";
    std::cout << "min     = " << minval  << "  (serial check: 1)\n";

    return 0;
}