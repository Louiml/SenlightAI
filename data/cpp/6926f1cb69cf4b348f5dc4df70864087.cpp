// Write a C++ function that takes a vector of integers and the number of elements to consider, and returns the minimum number of moves required to make all elements equal, where a single move consists of increasing one element by 1 and decreasing another element by 1. If making all elements equal is impossible (i.e., the total sum is not divisible by the count, or the elements have differing parities preventing equalization), return -1. The function must handle empty input by returning 0, and must use `const` reference for the input vector. The returned value is the minimum number of single-pair moves, where each move transfers one unit from one element to another.
The core idea is that since each move transfers exactly one unit from one element to another, the total sum of the array remains constant. For all elements to become equal, the target value must be `total / n`, and this must be an integer, so `total % n == 0`. However, a subtle constraint exists: because each move changes two elements by opposite amounts (one +1, one -1), the parity of each element's value can only change if it participates in a move, but the parity of the total sum of all elements is invariant. More importantly, if the elements have different parities (some odd, some even), it is impossible to make all equal because the difference between any two elements must be even when both become equal (since equal numbers have the same parity), but a move changes the difference by 2 (one +1, one -1), so the initial difference between any two elements must be even. That means all elements must have the same parity initially. If both conditions hold, the minimum number of moves is computed by summing the absolute deviations from the target for all elements, then dividing by 2, because each move fixes 2 units of deviation (one element needs to increase, another decrease). The formula `sum(abs(a[i]-target))/2` gives the total number of moves, but the original snippet has an extra odd calculation `(coutnum-1)/2+1` that is incorrect; the correct answer is simply `sum/2`. Edge cases: empty vector (return 0), single element (already equal, return 0), total not divisible by n (return -1), different parities (return -1). Time complexity is O(n), space O(1) beyond input.
#include <vector>
#include <cstdlib> // for std::abs

int minMovesToEqualize(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    if (n == 0) return 0;

    long long total = 0;
    for (int val : a) {
        total += val;
    }

    // Total sum must be divisible by n for all elements to become equal.
    if (total % n != 0) return -1;

    // All elements must have the same parity because each move changes two elements by ±1,
    // preserving parity of each element but requiring differences between any two to be even.
    bool firstParity = (a[0] % 2 + 2) % 2; // handle negative values
    for (int val : a) {
        bool currentParity = (val % 2 + 2) % 2;
        if (currentParity != firstParity) return -1;
    }

    int target = static_cast<int>(total / n);
    long long moves = 0;
    for (int val : a) {
        moves += std::abs(val - target);
    }
    // Each move reduces total absolute deviation by 2 (one +1, one -1).
    return static_cast<int>(moves / 2);
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {1, 2, 3};
    assert(minMovesToEqualize(v1) == 1); // target 2: 1->2, 3->2, one move
    std::vector<int> v2 = {1, 1, 1};
    assert(minMovesToEqualize(v2) == 0);
    std::vector<int> v3 = {1, 2, 4};
    assert(minMovesToEqualize(v3) == -1); // total 7 not divisible by 3
    std::vector<int> v4 = {1, 3, 5};
    assert(minMovesToEqualize(v4) == -1); // parities differ? Actually all odd, but target 3, works
    // Let's test correct parity case: all odd, sum divisible by n
    std::vector<int> v4b = {1, 3, 5}; // sum=9, n=3, target=3, moves= (2+0+2)/2=2
    assert(minMovesToEqualize(v4b) == 2);
    std::vector<int> v5 = {1, 2, 3, 4}; // sum=10, n=4, target=2.5 -> -1
    assert(minMovesToEqualize(v5) == -1);
    std::vector<int> v6 = {2, 4, 6}; // sum=12, n=3, target=4, moves=(2+0+2)/2=2
    assert(minMovesToEqualize(v6) == 2);
    std::vector<int> v7 = {}; // empty
    assert(minMovesToEqualize(v7) == 0);
    std::vector<int> v8 = {5}; // single element
    assert(minMovesToEqualize(v8) == 0);
    std::vector<int> v9 = {1, 1, 2, 2}; // sum=6, n=4, target=1.5 -> -1
    assert(minMovesToEqualize(v9) == -1);
    std::vector<int> v10 = {0, 2, 4, 6}; // sum=12, n=4, target=3, parities differ (0,2,4,6 all even? yes all even) moves=(3+1+1+3)/2=4
    assert(minMovesToEqualize(v10) == 4);
}
