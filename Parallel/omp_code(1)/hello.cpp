#include <iostream>
#include <omp.h>

int main() {

    std::cout << "=== Before Parallel Region ===" << std::endl;
    std::cout << "Max threads available: " << omp_get_max_threads() << "\n\n";
    int thread_id, num_threads;
    
    #pragma omp parallel
    {
        thread_id   = omp_get_thread_num();
        num_threads = omp_get_num_threads();

        
        
        std::cout << "Hello from thread "
                      << thread_id
                      << " of "
                      << num_threads
                      << std::endl;
        
    }

    std::cout << "\n=== After Parallel Region ===" << std::endl;
    std::cout << "Back to serial. Thread id: " << omp_get_thread_num() << std::endl;

    return 0;
}