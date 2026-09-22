Write a C++ function `kthNiceNumber(int k)` that returns the k-th positive integer (1-indexed) that is **not** divisible by 3 and whose decimal representation does **not** contain the digit 3. For example, the first few such numbers are: 1, 2, 4, 5, 7, 8, 10, 11, 14, 16, 17, 19, … (notice that 3, 6, 9, 12, 13, 15, 18 are excluded). The function must handle `k` from 1 up to at least 10000 and return the integer as a `long long` to avoid overflow. You may assume `k` is positive and within the valid range. Do not precompute a global table; compute the result on‑the‑fly within the function.

The simplest correct approach is to iterate through natural numbers starting from 1, check each candidate against the two conditions (not divisible by 3 and no digit 3), and count matches until the desired `k` is reached. The check for digit 3 can be done by repeatedly taking the number modulo 10 and comparing to 3, then dividing by 10, until the number becomes 0. Since the density of valid numbers is roughly 2/3 of integers (because every third is divisible by 3, and the digit‑3 condition removes a smaller fraction), the k‑th valid number is approximately `1.5*k` plus some offset. For `k=10000`, the candidate range is around 15,000–20,000, which is tiny, so linear scanning is efficient. Edge cases: `k=1` returns 1; numbers like 3, 6, 9 are skipped; numbers containing 3 such as 13, 23, 30, 31, 32, 33, … are skipped. The algorithm runs in `O(answer)` time, which for the given limits is at most about 20,000 iterations per call, and uses `O(1)` extra space. A more mathematical formula could be derived, but the simple scan is clear and robust.

#include <cstdint>

// Returns the k-th positive integer (1-indexed) that is not divisible by 3
// and whose decimal representation does not contain the digit 3.
// Assumes k >= 1.
long long kthNiceNumber(int k) {
    long long candidate = 1;
    int found = 0;
    while (true) {
        // Check divisibility by 3
        bool divisibleBy3 = (candidate % 3 == 0);
        bool containsDigit3 = false;
        long long temp = candidate;
        while (temp > 0) {
            if (temp % 10 == 3) {
                containsDigit3 = true;
                break;
            }
            temp /= 10;
        }
        if (!divisibleBy3 && !containsDigit3) {
            ++found;
            if (found == k) {
                return candidate;
            }
        }
        ++candidate;
    }
}

#include <cassert>

// Declared solution function (must match the one in the solution section)
long long kthNiceNumber(int k);

int main() {
    // First few numbers from the sequence
    assert(kthNiceNumber(1) == 1);
    assert(kthNiceNumber(2) == 2);
    assert(kthNiceNumber(3) == 4);
    assert(kthNiceNumber(4) == 5);
    assert(kthNiceNumber(5) == 7);
    assert(kthNiceNumber(6) == 8);
    assert(kthNiceNumber(7) == 10);
    assert(kthNiceNumber(8) == 11);
    assert(kthNiceNumber(9) == 14);
    assert(kthNiceNumber(10) == 16);

    // Larger index to verify monotonic growth
    assert(kthNiceNumber(100) > kthNiceNumber(50));
    // Verify a known value: continue manually from the sequence up to 20
    assert(kthNiceNumber(15) == 25);  // after 22, 23 excluded, 24 divisible by 3, 25 valid
    assert(kthNiceNumber(20) == 34);  // check by listing valid numbers: ... 31?? (contains 3) 32?? (contains 3) 34 is valid after 29,31(no),32(no),33(no),34 yes
    return 0;
}
