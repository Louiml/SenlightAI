Given a non-empty vector of integers where one element appears more than `n / 2` times (where `n` is the vector size), write a C++ function that returns that majority element. The input may contain negative numbers, duplicates, and large sizes, but you can assume the majority element always exists. The function must work without sorting or using extra containers, and must be efficient for vectors up to 10^6 elements. Implement the function with `const` reference parameter and return an `int`.
The optimal algorithm is Boyer–Moore Majority Vote. It works by maintaining a candidate `element` and a `count`. Initially both are zero. Iterate through the array: if `count` is zero, set `element` to the current value and `count` to 1. Otherwise, if the current value equals `element`, increment `count`; else decrement `count`. At the end, `element` holds the majority candidate. Because the majority element appears more than half the time, its net contribution to `count` remains positive, so even if other elements temporarily cancel it, it will eventually become the candidate. Edge cases: a single-element vector returns that element; all elements are the same, count grows monotonically; for a guaranteed majority, the algorithm always returns a valid answer without needing a verification pass (though in a general problem, you’d verify). Time complexity is \(O(n)\) with one pass, and space complexity is \(O(1)\). The solution is robust for negative numbers and duplicates.
#include <vector>

// Returns the majority element (appears more than n/2 times) in nums.
// Assumes the majority element always exists.
int majorityElement(const std::vector<int>& nums) {
    int count = 0;
    int candidate = 0;

    for (int value : nums) {
        if (count == 0) {
            candidate = value;
            count = 1;
        } else if (value == candidate) {
            ++count;
        } else {
            --count;
        }
    }
    return candidate;
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {3, 2, 3};
    assert(majorityElement(v1) == 3);

    std::vector<int> v2 = {2, 2, 1, 1, 1, 2, 2};
    assert(majorityElement(v2) == 2);

    std::vector<int> v3 = {5};
    assert(majorityElement(v3) == 5);

    std::vector<int> v4 = {-1, -1, -1, 2, 2};
    assert(majorityElement(v4) == -1);

    std::vector<int> v5 = {1, 2, 1, 3, 1, 4, 1};
    assert(majorityElement(v5) == 1);

    std::vector<int> v6 = {9, 9, 9, 9, 8, 8};
    assert(majorityElement(v6) == 9);

    std::vector<int> v7 = {0, 0, 0, 0, 0};
    assert(majorityElement(v7) == 0);

    std::vector<int> v8 = {100, -100, 100, -100, 100};
    assert(majorityElement(v8) == 100);

    return 0;
}
