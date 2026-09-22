Given a multiset of positive integers where the multiset contains all divisors of two unknown positive integers x and y (with x ≥ y), write a C++ function `std::pair<int, int> findXY(const std::vector<int>& divisors)` that returns the pair {x, y}. The input vector length is at least 2 and at most 128, and every element is a divisor of either x or y (or both). The function must reconstruct x and y uniquely. The list may contain duplicates (e.g., a divisor common to both x and y appears twice) and is not necessarily sorted. The returned pair must be in non-increasing order (x first, then y). Assume the input is always valid and exactly reconstructs a unique pair.
// The key insight is that the largest number in the list must be x (since x ≥ y and x divides itself, and no divisor of y can exceed y ≤ x). After identifying x, we must remove from the multiset all divisors of x—each occurrence of a divisor of x that appears in the list must be removed exactly once (because the list contains exactly the union of the divisors of x and y, with common divisors appearing twice). After removing all divisors of x, the remaining largest number is y. To handle duplicates correctly, use a frequency map (e.g., an `unordered_map<int,int>`) to count occurrences. First, take the maximum value as x. Then for every divisor d of x (from 1 to sqrt(x), and if d divides x, also x/d), decrement the frequency of d by one if it exists and is positive. After this removal, the maximum value with a positive frequency is y. Edge cases: if x itself is removed (since x divides itself), its frequency becomes zero. Also, if y is also a divisor of x (i.e., y divides x), then after removing divisors of x, the frequency of y originally had at least 2 (one from x's divisors, one from y's own divisor list), so after decrementing once, it remains positive. Time complexity: O(sqrt(x) + n) for scanning and divisor generation, where n is the input size. Space complexity: O(n) for the frequency map.
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>

std::pair<int, int> findXY(const std::vector<int>& divisors) {
    // Count frequencies of each divisor
    std::unordered_map<int, int> freq;
    int maxVal = 0;
    for (int d : divisors) {
        freq[d]++;
        if (d > maxVal) maxVal = d;
    }

    int x = maxVal;
    // Remove all divisors of x from the frequency map
    for (int i = 1; i * i <= x; ++i) {
        if (x % i == 0) {
            if (freq.count(i) && freq[i] > 0) freq[i]--;
            int other = x / i;
            if (other != i && freq.count(other) && freq[other] > 0) freq[other]--;
        }
    }

    // The largest remaining value is y
    int y = 0;
    for (const auto& entry : freq) {
        if (entry.second > 0 && entry.first > y) y = entry.first;
    }

    return {x, y};
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is defined above. Below is the test harness.
int main() {
    // Example from problem statement: x=12, y=4 (divisors of 12: 1,2,3,4,6,12; divisors of 4: 1,2,4)
    std::vector<int> test1 = {1, 2, 3, 4, 6, 12, 1, 2, 4};
    assert(findXY(test1) == std::make_pair(12, 4));

    // x=8, y=8 (both same, so list is every divisor twice)
    std::vector<int> test2 = {1, 2, 4, 8, 1, 2, 4, 8};
    assert(findXY(test2) == std::make_pair(8, 8));

    // x=6, y=1 (divisors of 6: 1,2,3,6; divisor of 1: 1)
    std::vector<int> test3 = {1, 2, 3, 6, 1};
    assert(findXY(test3) == std::make_pair(6, 1));

    // x=100, y=25 (divisors of 100: 1,2,4,5,10,20,25,50,100; divisors of 25: 1,5,25)
    std::vector<int> test4 = {1,2,4,5,10,20,25,50,100, 1,5,25};
    assert(findXY(test4) == std::make_pair(100, 25));

    // Minimal case: x=2, y=1
    std::vector<int> test5 = {1, 2, 1};
    assert(findXY(test5) == std::make_pair(2, 1));

    // x=16, y=9 (no common divisors except 1)
    std::vector<int> test6 = {1,2,4,8,16, 1,3,9};
    assert(findXY(test6) == std::make_pair(16, 9));

    // Unsorted input with duplicates
    std::vector<int> test7 = {3, 1, 6, 2, 3, 1, 3, 9};
    // x=9, y=6 (divisors of 9: 1,3,9; divisors of 6: 1,2,3,6)
    assert(findXY(test7) == std::make_pair(9, 6));

    // Large numbers
    std::vector<int> test8 = {1, 2, 5, 10, 25, 50, 125, 250, 625, 1250, 1, 2, 5, 10};
    // x=1250, y=10
    assert(findXY(test8) == std::make_pair(1250, 10));

    // Both are prime, no common divisors except 1
    std::vector<int> test9 = {1, 7, 1, 11};
    assert(findXY(test9) == std::make_pair(11, 7));

    // Duplicate x itself (case where y also equals x)
    std::vector<int> test10 = {1, 3, 3, 9, 9};
    assert(findXY(test10) == std::make_pair(9, 9));

    return 0;
}
