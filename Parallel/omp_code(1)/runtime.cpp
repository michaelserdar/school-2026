#include <iostream>
#include <vector>
#include <string>
#include <omp.h>

void run_with_schedule(omp_sched_t kind, int chunk, const std::string &label) {
    const int N = 12;
    std::vector<int> owner(N);

    omp_set_schedule(kind, chunk);

    #pragma omp parallel for schedule(runtime) num_threads(4)
    for (int i = 0; i < N; i++)
        owner[i] = omp_get_thread_num();

    std::cout << "[" << label << "]\n";
    for (int t = 0; t < 4; t++) {
        std::cout << "  Thread " << t << " → ";
        for (int i = 0; i < N; i++)
            if (owner[i] == t) std::cout << "i=" << i << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "Same pragma, different schedules at runtime:\n\n";
    std::cout << "  #pragma omp parallel for schedule(runtime)\n\n";
    std::cout << "=========================================\n\n";

    run_with_schedule(omp_sched_static,  0, "omp_sched_static  (default chunk)");
    run_with_schedule(omp_sched_static,  2, "omp_sched_static  chunk=2");
    run_with_schedule(omp_sched_dynamic, 2, "omp_sched_dynamic chunk=2");
    run_with_schedule(omp_sched_guided,  1, "omp_sched_guided  chunk=1");

    return 0;
}