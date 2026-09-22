// Given two positive integers x and y, write a C++ function that computes the smallest positive integer z such that the z-th term of a certain sequence is greater than x. The sequence is defined as follows: for indices 3 and 4, the terms are y+1 and 2*y respectively, and for every index i >= 5, term i = term(i-1) + term(i-2) - 2. The function should return z-1, i.e., the largest index whose term is <= x. If x is smaller than term 3, return 2 (since no term at index 3 or above is <= x, but index 2 is implicitly 0 or negative). The inputs satisfy 1 ≤ y ≤ 10^9 and 1 ≤ x ≤ 10^18.
// The sequence grows roughly like Fibonacci (but with a constant -2 per step), so it grows exponentially. We can compute the sequence iteratively until we exceed x. Since each step adds approximately the previous two terms, the sequence grows at least as fast as Fibonacci; given that term 3 = y+1 ≥ 2 and term 4 = 2y ≥ 2, the sequence grows extremely fast. For x up to 10^18, the number of iterations needed is at most about 90 (since Fibonacci numbers exceed 10^18 around index 90). Therefore, we can simply compute terms starting from index 3 upward, storing only the last two values to save memory. The loop stops when the current term exceeds x, and then the answer is the previous index (one less than the index where we stopped). Edge case: if the first computed term (index 3) already exceeds x, then we return 2, because index 2 is the largest index with term <= x (since term 2 is not defined but implicitly less than term 3; we treat it as 0 or negative). Time complexity is O(k) where k is the number of steps until exceeding x, which is at most 90. Space complexity is O(1) using only a few variables.
#include <cstdint>

// Given x and y, return the largest index z such that sequence term at z is <= x.
// Sequence: term[3] = y+1, term[4] = 2*y, term[i] = term[i-1] + term[i-2] - 2 for i>=5.
// If even term[3] > x, returns 2.
int64_t largest_index_not_exceeding(int64_t x, int64_t y) {
    if (y + 1 > x) {
        return 2;
    }
    int64_t prev2 = y + 1;  // term[3]
    if (prev2 > x) return 2;
    if (2 * y > x) {
        return 3;
    }
    int64_t prev1 = 2 * y;  // term[4]
    int64_t index = 4;
    while (true) {
        // Compute next term: term[i] = prev1 + prev2 - 2
        int64_t next = prev1 + prev2 - 2;
        if (next > x) {
            return index - 1;
        }
        prev2 = prev1;
        prev1 = next;
        ++index;
    }
}
#include <cassert>
#include <cstdint>

// Function declaration (definition is in Solution section)
int64_t largest_index_not_exceeding(int64_t x, int64_t y);

int main() {
    // y=1 gives constant sequence of 2; never exceeds x=5 within range, so returns 100004.
    assert(largest_index_not_exceeding(5, 1) == 100004);
    // y=2: terms: 3,4,5,7,10,15,... For x=10, largest index with term<=10 is 7.
    assert(largest_index_not_exceeding(10, 2) == 7);
    // y=3: terms: 4,6,8,12,18,28,44,70,112,... For x=100, largest index is 10.
    assert(largest_index_not_exceeding(100, 3) == 10);
    // Even first term exceeds x, return 2.
    assert(largest_index_not_exceeding(1, 1) == 2);
    assert(largest_index_not_exceeding(2, 10) == 2);
    // Large values: just verify result is within valid range.
    int64_t r = largest_index_not_exceeding(1000000000000000000LL, 1000000000LL);
    assert(r >= 3 && r <= 100004);
    // Another large y and moderate x: y=1000, x=1000000. Compute a few terms manually to verify.
    // term3=1001, term4=2000, term5=2999, term6=4997, term7=7994, term8=12989, term9=20981, term10=33968, term11=54947, term12=88913, term13=143858 > 1e6? Actually 143858 < 1e6, term14=232769, term15=376625, term16=609392, term17=986015, term18=1595405 > 1e6, so largest index is 17.
    assert(largest_index_not_exceeding(1000000, 1000) == 17);
    return 0;
}
