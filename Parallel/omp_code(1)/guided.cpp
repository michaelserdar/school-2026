#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    const int N = 16;
    std::vector<int> C(N), serial_C(N), owner(N);

    // Serial reference
    for (int i = 0; i < N; i++)
        serial_C[i] = i * i;

    // Parallel with guided schedule
    #pragma omp parallel for schedule(guided, 2) num_threads(4)
    for (int i = 0; i < N; i++) {
        C[i]     = i * i;
        owner[i] = omp_get_thread_num();
    }

    // Print assignments
    for (int t = 0; t < 4; t++) {
        std::cout << "Thread " << t << " → ";
        for (int i = 0; i < N; i++)
            if (owner[i] == t) std::cout << "i=" << i << " ";
        std::cout << "\n";
    }

    // Correctness check
    bool ok = true;
    for (int i = 0; i < N; i++)
        if (C[i] != serial_C[i]) { ok = false; break; }

    std::cout << (ok ? "\n✅ Correct!" : "\n❌ Wrong!") << "\n";
    return 0;
}