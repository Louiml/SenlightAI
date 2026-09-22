// Given a positive integer n, write a C++ function that returns the smallest positive integer x such that the number of trailing zeros in x! (factorial) is at least n. This is the "Furious Five" problem: find the smallest multiple of 5 whose factorial has at least n trailing zeroes. For example, for n=1, the answer is 5 because 5! = 120 has 1 trailing zero; for n=5, the answer is 25 because 25! has 6 trailing zeros (the first factorial with ≥5), but 20! has only 4. The function must handle n up to 1,000,000,000 efficiently.
// The number of trailing zeros in k! is determined by counting the factors of 5 in k!, since factors of 2 are always more abundant. The standard formula: count = floor(k/5) + floor(k/25) + floor(k/125) + ... This count is a non-decreasing function of k. We need the smallest k such that count(k) >= n. Since the count grows roughly linearly with k (about k/4 asymptotically), we can use binary search on k. The upper bound: for n=1e9, the count reaches n at k ≈ 4e9, so we can set high = 5*n (ensuring count(5n) >= n because count(5n) ≥ n). The binary search finds the smallest k where count(k) >= n. Because the smallest such k must be a multiple of 5 (since adding numbers not divisible by 5 does not change the count, and the count jumps only at multiples of 5), we can simply return the found k. Important edge cases: n=0 (but here n≥1), and large n where overflow in multiplication must be handled (use long long). Time complexity: O(log n) for binary search, each count computation is O(log n) (number of terms is about log_5(n)). Total O((log n)^2), which is negligible. Space: O(1). Note: The original code uses a different approach but binary search is cleaner and correct.
#include <cstdint>

// Count trailing zeros in factorial of k (number of factors of 5)
int64_t countTrailingZeros(int64_t k) {
    int64_t count = 0;
    while (k > 0) {
        k /= 5;
        count += k;
    }
    return count;
}

// Return the smallest positive integer x such that x! has at least n trailing zeros.
int64_t furiousFive(int64_t n) {
    int64_t low = 0;
    int64_t high = n * 5;  // upper bound: count(5n) >= n because 5n includes n factors of 5
    while (low < high) {
        int64_t mid = low + (high - low) / 2;
        if (countTrailingZeros(mid) >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}
#include <cassert>

int main() {
    assert(furiousFive(1) == 5);      // 5! = 120 (1 zero)
    assert(furiousFive(5) == 25);     // 25! has 6 zeros
    assert(furiousFive(6) == 25);     // 25! has 6 zeros
    assert(furiousFive(7) == 30);     // 30! has 7 zeros
    assert(furiousFive(10) == 45);    // 45! has 10 zeros
    assert(furiousFive(100) == 405);  // 405! has 100 zeros
    assert(furiousFive(1000000000) == 4000000005LL); // large case
    return 0;
}
