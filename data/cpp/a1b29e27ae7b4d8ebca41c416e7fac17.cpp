// Write a C++ function that, given a positive integer `n`, determines whether `n` is a "perfect number"—that is, whether the sum of its proper positive divisors (all positive divisors excluding `n` itself) equals `n`. Additionally, write a second function that returns a vector of all perfect numbers in the inclusive range `[start, end]` where `start` and `end` are positive integers with `start ≤ end`. The function must handle edge cases gracefully, such as when no perfect numbers exist in the range, and should avoid checking numbers greater than the square root when computing divisors for efficiency, while still correctly summing all proper divisors.

// The core task is to implement a perfect-number check efficiently and then apply it over a range. For checking a single number `n`, the naive approach loops from 1 to `n/2` and sums divisors, giving O(n) time per number. However, we can optimize by noting that divisors come in pairs: if `i` divides `n`, then `n/i` also divides `n`. We only need to loop from 1 to sqrt(n). For each divisor `i` found, we add both `i` and `n/i` to the sum, but we must be careful: if `i == n/i` (i.e., `n` is a perfect square), we should only add `i` once. Also, we must exclude `n` itself from the sum, so we start with `sum = 1` (since 1 always divides `n` for positive `n`) and then loop from `i = 2` to `sqrt(n)`. The special case `n == 1` has no proper divisors, so the sum is 0 and it’s not perfect. The function `isPerfect(int n)` returns `sum == n`. Then, `perfectNumbersInRange(int start, int end)` iterates from `start` to `end`, calls `isPerfect` on each, and collects the results in a `std::vector<int>`. If the vector is empty, that’s a valid outcome. Time complexity: for each number in the range, checking takes O(sqrt(n)), so overall O((end - start + 1) * sqrt(end)). Space complexity is O(k), where k is the number of perfect numbers found (stored in the vector). Edge cases: `start` or `end` being 1 (1 is not perfect), ranges with no perfect numbers, and large ranges where performance matters.

#include <vector>
#include <cmath>

// Return true if the positive integer n is a perfect number.
bool isPerfect(int n) {
    if (n <= 1) return false;
    int sum = 1; // 1 is always a proper divisor
    int limit = static_cast<int>(std::sqrt(n));
    for (int i = 2; i <= limit; ++i) {
        if (n % i == 0) {
            sum += i;
            int counterpart = n / i;
            if (counterpart != i) {
                sum += counterpart;
            }
        }
    }
    return sum == n;
}

// Return a vector containing all perfect numbers in the inclusive range [start, end].
std::vector<int> perfectNumbersInRange(int start, int end) {
    std::vector<int> result;
    for (int num = start; num <= end; ++num) {
        if (isPerfect(num)) {
            result.push_back(num);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume the functions isPerfect and perfectNumbersInRange are declared above.

int main() {
    // Basic perfect number checks
    assert(isPerfect(6) == true);   // 1+2+3 = 6
    assert(isPerfect(28) == true);  // 1+2+4+7+14 = 28
    assert(isPerfect(496) == true); // classic perfect number
    assert(isPerfect(1) == false);
    assert(isPerfect(2) == false);
    assert(isPerfect(12) == false);

    // Range tests
    std::vector<int> result1 = perfectNumbersInRange(1, 10);
    assert(result1.size() == 1);
    assert(result1[0] == 6);

    std::vector<int> result2 = perfectNumbersInRange(1, 100);
    assert(result2.size() == 2);
    assert(result2[0] == 6 && result2[1] == 28);

    // Empty range result
    std::vector<int> result3 = perfectNumbersInRange(1, 1);
    assert(result3.empty());

    std::vector<int> result4 = perfectNumbersInRange(30, 30);
    assert(result4.empty());

    // Larger perfect number verification
    std::vector<int> result5 = perfectNumbersInRange(1, 500);
    assert(result5.size() == 3);
    assert(result5[0] == 6 && result5[1] == 28 && result5[2] == 496);

    return 0;
}
