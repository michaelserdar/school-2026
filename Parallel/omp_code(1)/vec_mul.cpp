#include <iostream>
#include <vector>
#include <omp.h>

// ─── Element-wise vector multiplication: C[i] = A[i] * B[i] ───────────────
void vector_multiply(const std::vector<double> &A,
                     const std::vector<double> &B,
                     std::vector<double>       &C,
                     int N) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        C[i] = A[i] * B[i];
    }
}

// ─── Dot product: result = sum of A[i] * B[i] ─────────────────────────────
double dot_product(const std::vector<double> &A,
                   const std::vector<double> &B,
                   int N) {
    double result = 0.0;

    #pragma omp parallel for schedule(static) reduction(+:result)
    for (int i = 0; i < N; i++) {
        result += A[i] * B[i];
    }

    return result;
}

int main() {
    const int N = 48;

    // ── Parallel initialization (first-touch aware) ────────────────────────
    std::vector<double> A(N), B(N), C(N, 0.0);

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        A[i] = static_cast<double>(i + 1);       // 1, 2, 3, ..., 12
        B[i] = static_cast<double>((i + 1) * 2); // 2, 4, 6, ..., 24
    }

    // ── Print input vectors ────────────────────────────────────────────────
    #pragma omp single
    {
        std::cout << "=== Input Vectors ===\n";
        std::cout << "A: ";
        for (int i = 0; i < N; i++) std::cout << A[i] << " ";
        std::cout << "\nB: ";
        for (int i = 0; i < N; i++) std::cout << B[i] << " ";
        std::cout << "\n\n";
    }

    // ── Element-wise multiplication ────────────────────────────────────────
    std::cout << "=== Element-wise Multiplication (C = A * B) ===\n";
    std::cout << "Threads used: " << omp_get_max_threads() << "\n";

    double t_start = omp_get_wtime();
    vector_multiply(A, B, C, N);
    double t_end   = omp_get_wtime();

    std::cout << "C: ";
    for (int i = 0; i < N; i++) std::cout << C[i] << " ";
    std::cout << "\n";
    std::cout << "Time: " << (t_end - t_start) * 1e6 << " microseconds\n\n";

    // ── Dot product ────────────────────────────────────────────────────────
    std::cout << "=== Dot Product (A · B) ===\n";

    t_start = omp_get_wtime();
    double dot = dot_product(A, B, N);
    t_end   = omp_get_wtime();

    std::cout << "A · B = " << dot << "\n";
    std::cout << "Time: " << (t_end - t_start) * 1e6 << " microseconds\n\n";

    // ── Verify dot product manually ────────────────────────────────────────
    double expected = 0.0;
    for (int i = 0; i < N; i++) expected += A[i] * B[i];
    std::cout << "Verification (serial): " << expected << " — "
              << (dot == expected ? "✅ MATCH" : "❌ MISMATCH") << "\n";

    return 0;
}