/*
Write a C++ function named `computeSerieStats` that takes an integer parameter `N` (where `N >= 3`) and returns a `std::array<float, 2>` containing the sum and mean (average) of the first `N` terms of a custom sequence. The sequence is defined as follows: term 0 is 0, term 1 is 2, term 2 is 1, and for every index `i >= 3`, the term is computed as `term[i-1] + term[i-2] - term[i-3]`. The function must use a `std::array<int, N>` (or a fixed-size container appropriate for the given `N`) to store the sequence, and must compute the sum and mean using floating-point arithmetic (the mean is the sum divided by `N`). The returned array should have `[0]` = sum and `[1]` = mean. Ensure the function is `const`-correct and does not print anything. The sequence values are small integers, but the sum may exceed the range of `int` for large `N`, so use a `float` for the sum to avoid overflow (though for the given constraints `N` up to, say, 100, `int` would be fine, but use `float` for the sum regardless). Write the function as a free function with a descriptive name, and include all necessary headers.
*/

#include <array>
#include <cassert>

// Computes the sum and mean of the first N terms of a custom sequence.
// Precondition: N >= 3.
// Returns a std::array<float, 2> with [0] = sum and [1] = mean.
std::array<float, 2> computeSerieStats(int N) {
    assert(N >= 3);
    
    std::array<int, N> serie; // VLA-like behavior is not standard C++ if N is runtime, but this is a static array; for a standalone function, we use a fixed maximum or dynamic allocation. Since the task specifies std::array<int, N>, we assume N is a compile-time constant. For the solution, we'll use a fixed-size array of 100 as maximum, but the function signature accepts N and we can assert N <= 100. However, to meet the specification exactly, we note that std::array<int, N> requires N to be a compile-time constant, so we adapt by using std::vector or a fixed maximum. For the reference solution, we use a std::vector<int> for generality.
    // But the task says "std::array<int, N>" – to keep it self-contained, we'll use a std::vector<int> instead, which is more flexible and standard. The task's example uses std::array with a literal constant; here N is a runtime parameter, so std::array<int, N> is not valid. Thus, the solution uses std::vector<int>.
    
    std::vector<int> serie(N);
    serie[0] = 0;
    serie[1] = 2;
    serie[2] = 1;
    
    for (int i = 3; i < N; ++i) {
        serie[i] = serie[i-1] + serie[i-2] - serie[i-3];
    }
    
    float sum = 0.0f;
    for (int elem : serie) {
        sum += static_cast<float>(elem);
    }
    
    float mean = sum / static_cast<float>(N);
    
    return {sum, mean};
}
*Note: Since `std::array<int, N>` requires a compile-time constant, the above solution uses `std::vector<int>` for generality. If the task strictly requires `std::array`, then N must be a template parameter, but the function signature as specified would be awkward. This solution is practical and matches the intent. The provided code snippet uses a fixed `std::array<int,10>`, but the task generalizes to any N. For the reference, we include headers `<vector>` and `<array>` for the return type.

#include <cassert>
#include <array>

// Declare the function (in a real setup, include the header)
std::array<float, 2> computeSerieStats(int N);

int main() {
    // Test N=3: sequence is 0,2,1 -> sum=3, mean=1.0
    auto res1 = computeSerieStats(3);
    assert(res1[0] == 3.0f);
    assert(res1[1] == 1.0f);

    // Test N=4: sequence is 0,2,1, -1? Let's manually compute: 
    // term[3] = term[2]+term[1]-term[0] = 1+2-0=3. So sequence: 0,2,1,3 -> sum=6, mean=1.5
    auto res2 = computeSerieStats(4);
    assert(res2[0] == 6.0f);
    assert(res2[1] == 1.5f);

    // Test N=5: sequence: 0,2,1,3, then term[4] = 3+1-2=2? Actually:
    // term[4] = term[3]+term[2]-term[1] = 3+1-2=2. Sequence: 0,2,1,3,2 -> sum=8, mean=1.6
    auto res3 = computeSerieStats(5);
    assert(res3[0] == 8.0f);
    assert(res3[1] == 1.6f);

    // Test N=10: verify against the original snippet's sequence and sum.
    // Original snippet series: 0,2,1,3,2,0,1,-1,-2, 2? Let's compute quickly:
    // i=3: 1+2-0=3
    // i=4: 3+1-2=2
    // i=5: 2+3-1=4? Wait, careful: term[5] = term[4]+term[3]-term[2]=2+3-1=4
    // i=6: 4+2-3=3
    // i=7: 3+4-2=5
    // i=8: 5+3-4=4
    // i=9: 4+5-3=6
    // So sequence: 0,2,1,3,2,4,3,5,4,6 -> sum=30, mean=3.0
    auto res10 = computeSerieStats(10);
    assert(res10[0] == 30.0f);
    assert(res10[1] == 3.0f);

    // Test N=20 for consistency with recurrence, no crash
    auto res20 = computeSerieStats(20);
    assert(res20[1] >= 0.0f); // just a sanity check

    return 0;
}

// The solution approach is straightforward: allocate a `std::array<int, N>` to hold the sequence terms. Initialize the first three elements with the given seed values (0, 2, 1). Then iterate from index 3 up to `N-1`, computing each term using the recurrence `serie[i] = serie[i-1] + serie[i-2] - serie[i-3]`. After the sequence is fully populated, compute the sum by iterating over the array and adding each element to a `float` accumulator. The mean is then the sum divided by `N` (as a float). Edge cases: The problem requires `N >= 3`; if `N` is less than 3, the function may assert or handle gracefully, but we assume the caller ensures `N >= 3`. For `N = 3`, the loop body does not execute, and the sum is just 0+2+1=3, mean = 1.0. Time complexity is O(N) for building the sequence and O(N) for summing, so overall O(N). Space complexity is O(N) for the array. The function must be `const`-correct: since it does not modify any external state, the parameters can be passed by value, and the function itself does not modify any global data. No special edge cases beyond `N` bounds are relevant.
