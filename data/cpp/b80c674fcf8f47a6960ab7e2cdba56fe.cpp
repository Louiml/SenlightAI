// Write a C++ function `long long xorParityResult(const std::vector<int>& nums)` that: (1) computes the bitwise XOR of all integers in the input vector; (2) if the number of elements is odd, returns that XOR value; (3) if the number of elements is even, returns `0` when the XOR is `0`, otherwise returns `-1`. The function must handle vectors with zero elements by returning `0` for even length (0 is even) and XOR of empty set is `0`. Ensure the function is `const`-correct and uses appropriate types.

The core algorithm is straightforward: iterate through the vector once, maintaining a running XOR (initialized to 0). After processing all elements, check the parity of the vector's size. For an odd-sized vector, the result is simply the running XOR. For an even-sized vector, if the XOR equals 0, the answer is 0; otherwise, it is -1. The empty vector (size 0, which is even) yields XOR 0, so the answer is 0. The algorithm runs in O(n) time and uses O(1) extra space. Edge cases include a vector with a single element (odd size) returning that element, and a vector with two identical elements (even size, XOR=0) returning 0. Negative numbers work naturally with bitwise XOR. No special handling of overflow is needed since XOR operates on bit patterns and the result fits in a 64-bit signed type for typical integer inputs.

#include <vector>
#include <cstdint>

// Computes the XOR parity result based on vector size and XOR of elements.
// Returns XOR of all elements if size is odd; returns 0 if even and XOR==0; otherwise -1.
long long xorParityResult(const std::vector<int>& nums) {
    long long runningXor = 0;
    for (int val : nums) {
        runningXor ^= val;
    }
    if (nums.size() % 2 == 1) {
        return runningXor;
    } else {
        return (runningXor == 0) ? 0 : -1;
    }
}

#include <cassert>
#include <vector>

int main() {
    // Odd-length vectors: return XOR of all elements
    assert(xorParityResult({1, 2, 3}) == (1 ^ 2 ^ 3)); // 0
    assert(xorParityResult({7}) == 7);
    assert(xorParityResult({5, 5, 5}) == (5 ^ 5 ^ 5)); // 5
    assert(xorParityResult({-1, 2, -3}) == (-1 ^ 2 ^ -3));

    // Even-length vectors with XOR == 0 return 0
    assert(xorParityResult({3, 3}) == 0);
    assert(xorParityResult({1, 2, 3, 0}) == 0); // 1^2^3^0 = 0
    assert(xorParityResult({}) == 0); // empty even, XOR=0

    // Even-length vectors with non-zero XOR return -1
    assert(xorParityResult({1, 2}) == -1);
    assert(xorParityResult({1, 2, 3, 4}) == -1); // 1^2^3^4 = 4 != 0
}
