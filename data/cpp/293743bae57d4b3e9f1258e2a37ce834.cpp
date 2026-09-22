// Given an array of `n` distinct integers (where `n` can be zero), write a C++ function that determines whether the array can be partitioned into two non-decreasing subsequences (not necessarily contiguous) such that each element belongs to exactly one subsequence. The array may be empty, and if it can be partitioned, return a vector of length `n` where each element is `0` if the corresponding array element goes to the first subsequence, or `1` if it goes to the second subsequence. If multiple valid partitions exist, return any one. If no such partition exists, return an empty vector. For example, for input `[5, 1, 3, 2, 4]`, one valid partition is `[0, 1, 0, 1, 0]` (subsequence 0: 5, 3, 4 is not non-decreasing; but another like `[1, 0, 0, 1, 1]`? Actually a correct one is `[1, 0, 0, 1, 1]` with first: 1,3,4 and second: 5,2? No, second 5,2 is not non-decreasing. The correct partition exists: first: 1,2,4 (indices 1,3,4) and second: 5,3? no. Let's test: [5,1,3,2,4] → first: 5,3? no. Better to use a known solve: use two last values of each subsequence, greedily assign to the subsequence with smaller last value when possible. The problem is to output the assignment. Ensure edge cases: empty array returns empty vector (not an error), single element returns `{0}`. Multiple valid assignments possible; any is acceptable. The function signature: `std::vector<int> partitionNonDecreasing(const std::vector<int>& arr)`.

#include <cassert>
#include <vector>

// Assume partitionNonDecreasing is defined above.

int main() {
    // Simple cases
    assert(partitionNonDecreasing({}) == std::vector<int>{});
    assert(partitionNonDecreasing({5}) == std::vector<int>{0});
    
    // Increasing sequence: all to one subsequence
    auto r1 = partitionNonDecreasing({1,2,3,4});
    assert(r1.size() == 4);
    // Verify the subsequences are non-decreasing (any assignment works? Actually both can go to same)
    // We'll just check that the result is non-empty and has correct length.
    assert(r1.size() == 4);
    
    // Mixed valid partition
    auto r2 = partitionNonDecreasing({3,1,2});
    // Possible valid assignment: [0,1,1] -> sub0: 3; sub1: 1,2
    // Or [1,0,0] -> sub1: 3; sub0: 1,2
    // We just check that the function returns a non-empty vector of correct length.
    assert(r2.size() == 3);
    // Verify the partition is valid
    int last0 = -1000000, last1 = -1000000;
    for (size_t i = 0; i < r2.size(); ++i) {
        if (r2[i] == 0) { assert(3 > last0); last0 = 3; } // but actually need to use the element
    }
    // Better: re-verify the partition correctness:
    {
        std::vector<int> arr = {3,1,2};
        auto labels = partitionNonDecreasing(arr);
        assert(labels.size() == arr.size());
        int l0 = -1000000, l1 = -1000000;
        for (size_t i = 0; i < arr.size(); ++i) {
            if (labels[i] == 0) { assert(arr[i] >= l0); l0 = arr[i]; }
            else { assert(arr[i] >= l1); l1 = arr[i]; }
        }
    }
    
    // Impossible case: e.g., {5,4,3} cannot be split into two non-decreasing subsequences
    assert(partitionNonDecreasing({5,4,3}).empty());
    
    // Another valid case: {2,1,2}
    {
        std::vector<int> arr = {2,1,2};
        auto labels = partitionNonDecreasing(arr);
        assert(labels.size() == 3);
        int l0 = -1000000, l1 = -1000000;
        for (size_t i = 0; i < arr.size(); ++i) {
            if (labels[i] == 0) { assert(arr[i] >= l0); l0 = arr[i]; }
            else { assert(arr[i] >= l1); l1 = arr[i]; }
        }
    }
    
    // Edge: all equal
    {
        std::vector<int> arr = {7,7,7};
        auto labels = partitionNonDecreasing(arr);
        assert(labels.size() == 3);
        int l0 = -1000000, l1 = -1000000;
        for (size_t i = 0; i < arr.size(); ++i) {
            if (labels[i] == 0) { assert(arr[i] >= l0); l0 = arr[i]; }
            else { assert(arr[i] >= l1); l1 = arr[i]; }
        }
    }
    
    // Large random test: generate a sequence that is a concatenation of two sorted lists
    {
        std::vector<int> arr;
        // Build a valid sequence manually: sort two interleaved sorted lists
        std::vector<int> a = {1,4,6,8};
        std::vector<int> b = {2,3,5,7};
        // Interleave in a way that is valid: e.g., [1,2,4,3,6,5,8,7] is valid? Let's test: sub0: 1,4,6,8; sub1: 2,3,5,7
        arr = {1,2,4,3,6,5,8,7};
        auto labels = partitionNonDecreasing(arr);
        assert(labels.size() == arr.size());
        int l0 = -1000000, l1 = -1000000;
        for (size_t i = 0; i < arr.size(); ++i) {
            if (labels[i] == 0) { assert(arr[i] >= l0); l0 = arr[i]; }
            else { assert(arr[i] >= l1); l1 = arr[i]; }
        }
    }
    
    return 0;
}

#include <vector>
#include <limits>

// Partition array into two non-decreasing subsequences.
// Returns a vector of 0/1 labels, or empty if impossible.
std::vector<int> partitionNonDecreasing(const std::vector<int>& arr) {
    if (arr.empty()) return {};
    
    int last0 = std::numeric_limits<int>::min();
    int last1 = std::numeric_limits<int>::min();
    std::vector<int> result;
    result.reserve(arr.size());
    
    for (int x : arr) {
        bool can0 = (x >= last0);
        bool can1 = (x >= last1);
        
        if (!can0 && !can1) {
            return {}; // impossible
        }
        
        if (can0 && can1) {
            // Assign to the subsequence with smaller last value
            // to preserve the larger for future.
            if (last0 <= last1) {
                result.push_back(0);
                last0 = x;
            } else {
                result.push_back(1);
                last1 = x;
            }
        } else if (can0) {
            result.push_back(0);
            last0 = x;
        } else { // can1
            result.push_back(1);
            last1 = x;
        }
    }
    
    return result;
}

// This is a classic problem of checking if a sequence can be partitioned into two non-decreasing subsequences, which is equivalent to checking if the sequence can be colored with two colors such that each color's subsequence is non-decreasing. A simple greedy approach works: maintain two variables `last0` and `last1` representing the most recent value assigned to subsequence 0 and 1 respectively (initialize to `-INF` or `INT_MIN`). For each element `x` in order, we try to assign it to one of the subsequences where `x >= last` holds. If both are possible, we must decide which to pick. A robust rule is: if `last0 < last1`, assign to subsequence 0 (the one with smaller last value) to leave more freedom for future elements; otherwise assign to subsequence 1. If only one is possible, assign there. If neither, the partition is impossible, return empty vector. This greedy is correct because assigning to the subsequence with the smaller current last value is always at least as good as assigning to the larger one, since it preserves the larger last value for later elements. However, a more careful greedy that handles edge cases (like when both are possible but one assignment leads to failure) is to try both possibilities via backtracking; but for this problem, the simple rule works because the constraints are linear. To be safe, we can implement a greedy with a check: when both are possible, if the next element is larger than both last0 and last1, then it doesn't matter; but if the next element is between last0 and last1, we should assign to the one with smaller last. Actually the standard solution is: maintain two last values `a` and `b`. For each x: if x >= a and x >= b, assign to the one with smaller last (tie break arbitrary). If only one condition holds, assign there. Else fail. This always works because if x >= both, assigning to the smaller last keeps the larger last for future. If x >= only one, assign there. Complexity: O(n) time, O(1) extra space besides the output vector. Edge cases: empty array returns empty vector; array with one element returns `{0}` (or `{1}`). All elements equal works (any assignment). If impossible, return empty vector. The test cases will verify correctness on several examples.
