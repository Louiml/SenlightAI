Write a C++ function that takes a `std::vector<int>` and an execution policy (either `thrust::host` or `thrust::device`) as parameters, and uses Thrust's `for_each` to print each element with a formatted message that includes the value and the execution space name (e.g., "printing 5 on host" or "printing 5 on device"). The function should return the total number of elements processed. The function must be generic enough to work with Thrust's universal vectors and respect the given execution policy. You may assume the input is non-empty and the execution policy is valid.

The solution uses Thrust's `for_each` algorithm with the provided execution policy to iterate over the range `[vec.begin(), vec.end())`. The lambda captures nothing by reference but takes `int val` by value and prints it using `std::printf` with `ach::execution_space()` to identify the current execution space. Since `for_each` returns the number of elements processed (the distance between iterators), we can return that count directly. The main edge case is ensuring the lambda works for both host and device execution; using `__host__ __device__` annotation (or just `__host__` for simplicity in host-only compilation, but to support device we use `__host__ __device__`) ensures it can be called from both. Time complexity is O(n) where n is the number of elements, and space complexity is O(1) auxiliary. The function does not modify the input vector, so it takes a `const` reference to the vector to enforce `const` correctness, but Thrust iterators from a const vector still allow read-only access.

#include <thrust/for_each.h>
#include <thrust/execution_policy.h>
#include <thrust/universal_vector.h>
#include <cstdio>
#include "ach.h" // provides ach::execution_space()

// Prints each element of the input vector using the specified Thrust execution policy.
// Returns the number of elements processed.
template <typename Policy>
std::size_t print_elements(const thrust::universal_vector<int>& vec, Policy policy) {
    return thrust::for_each(policy, vec.begin(), vec.end(),
        [] __host__ __device__ (int val) {
            std::printf("printing %d on %s\n", val, ach::execution_space());
        });
}

#include <cassert>
#include <thrust/universal_vector.h>
#include "ach.h"

// Forward declaration of the solution function template
template <typename Policy>
std::size_t print_elements(const thrust::universal_vector<int>& vec, Policy policy);

int main() {
    thrust::universal_vector<int> vec{1, 2, 3, 4, 5};
    
    // Test with host policy
    std::size_t count_host = print_elements(thrust::host, vec);
    assert(count_host == 5);
    
    // Test with device policy (requires CUDA-aware Thrust and device support)
    // The call below should compile and run on GPU if available.
    std::size_t count_device = print_elements(thrust::device, vec);
    assert(count_device == 5);
    
    // Test with a single element vector
    thrust::universal_vector<int> single{42};
    assert(print_elements(thrust::host, single) == 1);
    
    // Test with an empty vector (though task says non-empty, just ensure robustness)
    thrust::universal_vector<int> empty;
    // Note: for_each on empty range returns 0, but we don't call because task assumes non-empty.
    // The function still works but we skip assertion for brevity.
    
    // Test with a larger vector (1000 elements)
    thrust::universal_vector<int> big(1000);
    thrust::fill(big.begin(), big.end(), 7);
    assert(print_elements(thrust::host, big) == 1000);
    
    return 0;
}
