Write a C++ function `std::vector<long long> generateArray(long long n)` that, given a positive integer `n` (where `2 ≤ n ≤ 10^5`), constructs an array of length `n` according to the following rules: If `n` is even, the array starts with `n-1` and `n` in the first two positions, and then for indices `2` through `n-1` (0-based), the value is `index - 1` (so positions 2,3,... get values 1,2,...). If `n` is odd, the array is simply the integers from `n` down to `1` in descending order. The function must return the constructed array as a `std::vector<long long>`. The output order is exactly as described, and duplicate values are allowed. The solution must be efficient even for large `n` and avoid unnecessary allocations.

// The task involves two distinct cases based on parity of `n`. For even `n`, we directly assign the first two elements as `n-1` and `n`, then for the remaining positions `i` from 2 to `n-1`, assign `i-1` (which yields the sequence 1, 2, 3, ..., `n-2`). For odd `n`, we fill the array with a reverse loop from `n` down to 1. The main edge case is when `n=2`: even branch gives `[1,2]`, which matches the pattern. For odd `n=3`, it gives `[3,2,1]`. The algorithm runs in O(n) time because each element is assigned exactly once. Space complexity is O(n) for the output vector, which is required to store the result. No extra auxiliary space beyond a few loop counters is used. Care must be taken to use `long long` for the vector elements since both `n` and the values can be as large as 10^5, but within 64-bit range; using `int` would also suffice, but `long long` matches the original snippet's style.

#include <vector>

// Generate an array of length n as specified:
// If n is even: {n-1, n, 1, 2, ..., n-2}
// If n is odd:  {n, n-1, ..., 2, 1}
std::vector<long long> generateArray(long long n) {
    std::vector<long long> result(n);
    if (n % 2 == 0) {
        result[0] = n - 1;
        result[1] = n;
        for (long long i = 2; i < n; ++i) {
            result[i] = i - 1;
        }
    } else {
        for (long long i = 0; i < n; ++i) {
            result[i] = n - i;
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Solution function (as above) is assumed to be available.
std::vector<long long> generateArray(long long n);

int main() {
    // Even n = 2: {1, 2}
    std::vector<long long> e2 = generateArray(2);
    assert(e2.size() == 2);
    assert(e2[0] == 1 && e2[1] == 2);

    // Even n = 4: {3, 4, 1, 2}
    std::vector<long long> e4 = generateArray(4);
    assert(e4.size() == 4);
    assert(e4[0] == 3 && e4[1] == 4 && e4[2] == 1 && e4[3] == 2);

    // Even n = 6: {5, 6, 1, 2, 3, 4}
    std::vector<long long> e6 = generateArray(6);
    assert(e6.size() == 6);
    for (long long i = 0; i < 6; ++i) {
        long long expected = (i < 2) ? (5 + i) : (i - 1);
        assert(e6[i] == expected);
    }

    // Odd n = 3: {3, 2, 1}
    std::vector<long long> o3 = generateArray(3);
    assert(o3.size() == 3);
    assert(o3[0] == 3 && o3[1] == 2 && o3[2] == 1);

    // Odd n = 5: {5, 4, 3, 2, 1}
    std::vector<long long> o5 = generateArray(5);
    assert(o5.size() == 5);
    for (long long i = 0; i < 5; ++i) {
        assert(o5[i] == 5 - i);
    }

    // Odd n = 1 (smallest odd): {1}
    std::vector<long long> o1 = generateArray(1);
    assert(o1.size() == 1);
    assert(o1[0] == 1);

    // Large n = 100001 (odd) check first/last values
    long long bigOdd = 100001;
    std::vector<long long> oBig = generateArray(bigOdd);
    assert(oBig[0] == bigOdd);
    assert(oBig[bigOdd - 1] == 1);

    // Large n = 100000 (even) check first/last values
    long long bigEven = 100000;
    std::vector<long long> eBig = generateArray(bigEven);
    assert(eBig[0] == bigEven - 1);
    assert(eBig[1] == bigEven);
    assert(eBig[2] == 1);
    assert(eBig[bigEven - 1] == bigEven - 2);
}
