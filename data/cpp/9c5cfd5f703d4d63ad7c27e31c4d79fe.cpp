// Write a C++ function `minimumDeletionsForSquareNumber` that takes an integer `N` (where `1 ≤ N ≤ 2,000,000,000`) and returns the minimum number of digits that must be deleted from its decimal representation (without reordering the remaining digits) so that the resulting number is a perfect square and has no leading zeros. If it is impossible, return `-1`. For example, given `N = 8314`, you could delete one digit (the `8`) to make `314`, but `314` is not a square; deleting `8` and `1` gives `34` (not square), deleting `8` and `3` gives `14` (not square), but deleting `8` and `4` gives `31`, and deleting `8,3,1` gives `4`, which is a square (2²), so the answer is 3 deletions. You may need to consider all subsets of digits of the original number, keeping their original relative order. Leading zeros are not allowed (e.g., from `100` you cannot make `00` or `01`).
#include <cassert>
#include <cmath>

// Forward declaration of the function being tested (the solution function).
int minimumDeletionsForSquareNumber(int N);

int main() {
    // Single digit perfect squares require zero deletions.
    assert(minimumDeletionsForSquareNumber(4) == 0);
    assert(minimumDeletionsForSquareNumber(9) == 0);
    // Single digit non-square: impossible.
    assert(minimumDeletionsForSquareNumber(7) == -1);
    // Two-digit: delete one digit to get a square.
    assert(minimumDeletionsForSquareNumber(10) == 1); // delete 1 -> 0? but leading zero disallowed, actually delete 0 -> 1, so 1 deletion.
    assert(minimumDeletionsForSquareNumber(12) == 1); // delete 2 -> 1, delete 1 -> 2 (not square), so 1.
    // Example from description: 8314 -> delete 3 digits to get 4.
    assert(minimumDeletionsForSquareNumber(8314) == 3);
    // 100 -> delete two zeros to get 1, or delete 1 and a zero? 10 not square, 00 disallowed, so 2 deletions.
    assert(minimumDeletionsForSquareNumber(100) == 2);
    // 144 -> already a square, 0 deletions.
    assert(minimumDeletionsForSquareNumber(144) == 0);
    // 169 is square, 0 deletions.
    assert(minimumDeletionsForSquareNumber(169) == 0);
    // 16 -> delete 1? 6 not square, delete 6 -> 1, so 1.
    assert(minimumDeletionsForSquareNumber(16) == 0); // 16 itself is square.
    // 25 is square.
    assert(minimumDeletionsForSquareNumber(25) == 0);
    // 99 -> can make 9 by deleting one 9, so 1.
    assert(minimumDeletionsForSquareNumber(99) == 1);
    // 123456789 -> can make 1 by deleting 8 digits, but maybe better? Let's just test a known case: 49 is square with 0 deletions from 49.
    assert(minimumDeletionsForSquareNumber(49) == 0);
    // 81 is square.
    assert(minimumDeletionsForSquareNumber(81) == 0);
    // 200 -> can make 0? disallowed, make 4? no, make 2? no, make 0? no, make 00? no, so -1.
    assert(minimumDeletionsForSquareNumber(200) == -1);
    // 101 -> can make 1 by deleting both zeros? 1 is square, so 2 deletions.
    assert(minimumDeletionsForSquareNumber(101) == 2);
    return 0;
}
#include <vector>
#include <cmath>
#include <algorithm>

// Returns the minimum number of digits that must be deleted from N
// so that the remaining digit subsequence forms a perfect square (no leading zeros).
// If impossible, returns -1.
int minimumDeletionsForSquareNumber(int N) {
    // Extract digits of N into a vector (most significant first).
    std::vector<int> digits;
    int temp = N;
    while (temp > 0) {
        digits.push_back(temp % 10);
        temp /= 10;
    }
    std::reverse(digits.begin(), digits.end());
    if (digits.empty()) digits.push_back(0); // Handle N == 0 just in case, but N >= 1 per spec.

    const int d = static_cast<int>(digits.size());
    const int totalSubsets = 1 << d;
    int best = d + 1; // More than maximum possible deletions.

    // Enumerate all non-empty subsets.
    for (int mask = 1; mask < totalSubsets; ++mask) {
        // Build the subsequence from the least significant bit to most,
        // but we need original order: iterate positions from 0 to d-1.
        std::vector<int> sub;
        for (int i = 0; i < d; ++i) {
            if (mask & (1 << i)) {
                sub.push_back(digits[i]);
            }
        }
        // Leading zero not allowed.
        if (sub[0] == 0) continue;

        // Convert subsequence to integer value.
        long long value = 0;
        for (int digit : sub) {
            value = value * 10 + digit;
        }

        // Check if value is a perfect square.
        long long root = static_cast<long long>(std::sqrt(static_cast<double>(value)));
        bool isSquare = false;
        for (long long k = root - 2; k <= root + 2; ++k) {
            if (k >= 0 && k * k == value) {
                isSquare = true;
                break;
            }
        }
        if (isSquare) {
            int deletions = d - static_cast<int>(sub.size());
            if (deletions < best) best = deletions;
        }
    }

    return (best == d + 1) ? -1 : best;
}
// The core idea is to enumerate every non-empty subset of the digit positions of the decimal string of `N`, using a bitmask over up to 11 digits (since the maximum value fits in 10 digits, but we allocate 11 for safety). For each subset, extract the digits in their original order into a vector, skip any subset whose first extracted digit is `0` (to avoid leading zeros), and convert the vector to an integer value. Then check if that value is a perfect square by computing the integer square root via `std::sqrt` and verifying nearby candidates (`sqrt - 2` to `sqrt + 2`) because floating-point precision may be off by one. Keep the minimum number of deletions (original digit count minus subset size) across all valid squares. If no subset yields a square, return `-1`. Edge cases include single-digit numbers (only `0,1,4,9` are squares, but `0` is disallowed as leading zero if it starts, though the number itself could be `0`? Since `N ≥ 1`, you can never have a value of `0` because leading zeros are forbidden, but if `N` itself is a single digit like `4`, answer is `0`). Complexity: For `d` digits, there are `2^d − 1` non-empty subsets, each requiring `O(d)` work to reconstruct and check, so `O(d * 2^d)` which is at most `O(11 * 2048) ≈ 22,000` operations — effectively constant. Space is `O(d)` for the temporary vector.
