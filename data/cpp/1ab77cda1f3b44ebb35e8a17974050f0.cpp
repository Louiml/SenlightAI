// Write a C++ function named `growPopulation` that takes three integers `r`, `d`, and `x0` (where `r > 1`, `d >= 0`, and `x0` can be any non-negative integer) and returns a `std::vector<long long>` containing exactly 10 values. The first value is computed as `x1 = r * x0 - d`, and each subsequent value `xi+1 = r * xi - d` for `i = 1, 2, ..., 9`. The function must handle large intermediate values by using `long long` throughout, and it must not accept negative values for `r` or `d` (if invalid, return an empty vector). The result vector must contain the 10 computed values in order, starting from `x1` and ending at `x10`.
The solution repeatedly applies the linear recurrence `x_{n+1} = r * x_n - d` ten times. Starting from the initial `x0`, we iterate exactly 10 times, updating the current value and storing each new value in the result vector. Because `r` and `d` are given as `int` but may produce large products, we cast operands to `long long` to prevent overflow (note: the original snippet uses `int` for `r`, `d`, `x`, but the recurrence can quickly exceed 32-bit limits; using `long long` is safer). Edge cases: if `r <= 1` or `d < 0`, the function returns an empty vector as specified. Duplicate values are fine—just push each result. Time complexity is O(10) = O(1) since the number of iterations is constant; space complexity is O(1) for computation plus O(10) for the returned vector (effectively O(1) as well). There are no other pitfalls because the recurrence is deterministic and each step depends only on the previous value.
#include <vector>

// Compute 10 terms of the recurrence x_{n+1} = r * x_n - d starting from x0.
// Returns an empty vector if r <= 1 or d < 0.
std::vector<long long> growPopulation(int r, int d, int x0) {
    if (r <= 1 || d < 0) {
        return {};
    }
    
    std::vector<long long> result;
    result.reserve(10);
    
    long long current = x0;
    for (int i = 0; i < 10; ++i) {
        current = static_cast<long long>(r) * current - d;
        result.push_back(current);
    }
    
    return result;
}
#include <cassert>
#include <vector>

// Declare the function (or include the solution above).
std::vector<long long> growPopulation(int r, int d, int x0);

int main() {
    // Basic example from the snippet: r=2, d=3, x0=10
    // x1=17, x2=31, x3=59, x4=115, x5=227, x6=451, x7=899, x8=1795, x9=3587, x10=7171
    std::vector<long long> expected = {17, 31, 59, 115, 227, 451, 899, 1795, 3587, 7171};
    assert(growPopulation(2, 3, 10) == expected);
    
    // Minimal case: r=2, d=0, x0=1 => 2,4,8,16,32,64,128,256,512,1024
    expected = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    assert(growPopulation(2, 0, 1) == expected);
    
    // Large values to check long long handling: r=1000, d=1, x0=0
    // x1=-1, x2= -1001, x3= -1001001, etc. We'll just check size and first few.
    std::vector<long long> large = growPopulation(1000, 1, 0);
    assert(large.size() == 10);
    assert(large[0] == -1);
    assert(large[1] == -1001);
    
    // Invalid r
    assert(growPopulation(1, 5, 100).empty());
    assert(growPopulation(0, 5, 100).empty());
    // Invalid d
    assert(growPopulation(3, -1, 100).empty());
    
    // Negative x0 is allowed if r>1 and d>=0
    expected = {-7, -17, -37, -77, -157, -317, -637, -1277, -2557, -5117}; // r=2, d=3, x0=-2 => -2*2-3=-7, etc.
    assert(growPopulation(2, 3, -2) == expected);
    
    return 0;
}
