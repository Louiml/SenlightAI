// Write a C++ function `int minSecondsToFillCups(vector<int>& amount)` that takes a vector of three non-negative integers representing the number of cups of three different types that need to be filled. In one second you can fill exactly two cups, each of a different type. If only one type remains, you can fill one cup per second. The function should return the minimum number of seconds required to fill all cups. The input vector always has exactly three elements, but any of them may be zero. The algorithm should be efficient and handle edge cases such as when one type dominates the others or when all are zero.

// The problem reduces to minimizing the total number of operations where each operation reduces two different counts by 1 (or reduces one count if only one type remains). The key insight is to sort the three counts so that we can reason about the largest count. Let `a <= b <= c` after sorting.  
// - If `a + b <= c`, then the two smaller piles can be completely paired with cups from the largest pile, and after using `a+b` seconds to fill those pairs, only `c - (a+b)` cups of the largest type remain, which take that many more single-cup seconds. Total = `c`.  
// - Otherwise, `a + b > c`, meaning even after pairing all cups from the largest pile with cups from the other two, there are extra cups in the smaller piles. The total number of pair operations is `(a+b+c)/2`, but since we can't have a fraction, we need `ceil((a+b+c)/2)`. This gives the minimum seconds because each second fills at most two cups, and we can always pair them until at most one cup remains.  
// Edge cases: all zeros → answer 0; one large value compared to others → answer equals that value; mixed values → the ceiling of half the sum. Time complexity is O(1) (constant sort of 3 elements), space O(1).

#include <vector>
#include <algorithm>

// Returns the minimum number of seconds to fill all cups given three counts.
int minSecondsToFillCups(std::vector<int>& amount) {
    // Sort the three counts in ascending order.
    std::sort(amount.begin(), amount.end());
    int a = amount[0], b = amount[1], c = amount[2];
    // If the two smallest can be fully matched with the largest, answer is largest.
    if (a + b <= c) {
        return c;
    }
    // Otherwise, each second fills two different types, so total pairs = ceil(sum/2).
    return (a + b + c + 1) / 2;
}

#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {1, 2, 3};
    assert(minSecondsToFillCups(v1) == 3);

    std::vector<int> v2 = {5, 0, 0};
    assert(minSecondsToFillCups(v2) == 5);

    std::vector<int> v3 = {4, 1, 1};
    assert(minSecondsToFillCups(v3) == 3);

    std::vector<int> v4 = {0, 0, 0};
    assert(minSecondsToFillCups(v4) == 0);

    std::vector<int> v5 = {2, 2, 2};
    assert(minSecondsToFillCups(v5) == 3);

    std::vector<int> v6 = {7, 3, 3};
    assert(minSecondsToFillCups(v6) == 7);

    std::vector<int> v7 = {1, 1, 10};
    assert(minSecondsToFillCups(v7) == 10);

    std::vector<int> v8 = {3, 3, 0};
    assert(minSecondsToFillCups(v8) == 3);

    std::vector<int> v9 = {100, 1, 1};
    assert(minSecondsToFillCups(v9) == 100);
}
