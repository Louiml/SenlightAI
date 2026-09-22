Write a C++ function `long long countMovements(const std::vector<long long>& a)` that takes a non-empty vector of non-negative integers (each in the range `[0, 1e9]`) and simulates the following process: traverse the array from left to right. Whenever you encounter a non-zero element, add its value to a running total and set a flag. If you later encounter a zero and the flag is set (i.e., at least one non-zero number has appeared before), add 1 to the total for that zero. If you encounter zeros before any non-zero, ignore them. The function returns the final total. The input vector length `n` satisfies `1 ≤ n ≤ 10^5`.
// The algorithm processes the array in a single left-to-right pass. Maintain a running sum `cnt` and a boolean flag `seenNonZero` (initialized to false). For each element:
// - If the element is non-zero, add its full value to `cnt` and set `seenNonZero = true`.
// - If the element is zero and `seenNonZero` is true, increment `cnt` by 1.
// - If the element is zero and `seenNonZero` is false, do nothing.
//
// This works because only zeros that appear after a non-zero contribute to the movement count. The flag ensures we ignore leading zeros. Edge cases: all zeros → output 0; a single non-zero → output its value; zeros between non-zeros are counted once each. Time complexity is O(n) with a single pass, and space complexity is O(1) auxiliary (ignoring input storage). The function does not modify the input vector, so it takes a const reference.
#include <vector>
#include <cstddef>

// Simulate the movement counting process described.
// Returns the total after processing all elements.
long long countMovements(const std::vector<long long>& a) {
    long long cnt = 0;
    bool seenNonZero = false;
    
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] != 0) {
            cnt += a[i];
            seenNonZero = true;
        } else if (seenNonZero) {
            cnt += 1;
        }
    }
    return cnt;
}
#include <cassert>
#include <vector>

// The solution function is declared above; for testing, we include it directly.
long long countMovements(const std::vector<long long>& a);

int main() {
    // Basic examples
    assert(countMovements({1, 2, 3}) == 6);
    assert(countMovements({0, 1, 2}) == 3);
    assert(countMovements({1, 0, 2}) == 4); // 1 + zero(1) + 2
    assert(countMovements({0, 0, 5}) == 5);
    assert(countMovements({5, 0, 0, 1}) == 7); // 5 + zero(1) + zero(1) + 1
    assert(countMovements({0}) == 0);
    assert(countMovements({0, 0, 0}) == 0);
    assert(countMovements({7}) == 7);
    // Larger vector with mixed values
    assert(countMovements({2, 0, 0, 3, 0, 1}) == 8); // 2 +1+1 +3 +1 +1
    assert(countMovements({1000000000LL, 0, 0}) == 1000000002LL);
    return 0;
}
