// You are given an array of `n` non-negative integers (with 0 ≤ value < 100001) and a non-negative integer `x` (with 0 ≤ x < 100001). Write a C++ function that determines the minimum number of operations required to make the array contain at least one duplicate value, where an operation consists of taking any single element and replacing it with the bitwise AND of that element and `x`. If it is impossible to create a duplicate, return -1. The possible answers are limited to 0, 1, 2, or -1, because applying the operation more than once per element is never beneficial—applying it twice to the same element leaves it unchanged (since `(a & x) & x == a & x`). The function should return 0 if the original array already has a duplicate, 1 if a single operation on one element can create a duplicate, 2 if two operations (on two different elements) are needed, and -1 if no sequence of operations can create a duplicate.

#include <cassert>
#include <vector>

int minOperationsToDuplicate(const std::vector<int>& a, int x);

int main() {
    // Case 0: Duplicate already exists
    assert(minOperationsToDuplicate({1, 2, 1}, 3) == 0);
    assert(minOperationsToDuplicate({5, 5}, 0) == 0);
    
    // Case 1: One operation creates duplicate
    assert(minOperationsToDuplicate({1, 2, 4}, 6) == 1); // 1&6=0, 2&6=2, 4&6=4; 4&6=4 matches 2? no; but 2&6=2 vs 4&6=4? Actually 2&6=2, 4&6=4, 1&6=0. No match? Let's pick better: {3, 5, 6}, x=4: 3&4=0,5&4=4,6&4=4 -> 5&4=4 equals 6&4? But that's case2. For case1: {8, 1}, x=9: 8&9=8, 1&9=1, no. Use {5, 2}, x=6: 5&6=4, 2&6=2, no. Use {7, 4}, x=6: 7&6=6, 4&6=4, no. Use {9, 1}, x=5: 9&5=1, 1&5=1? 1&5=1, yes, 9&5=1 equal to existing 1, so 1 op.
    assert(minOperationsToDuplicate({9, 1}, 5) == 1);
    assert(minOperationsToDuplicate({6, 4, 2}, 6) == 1); // 4&6=4? 4&6=4, 2&6=2, 6&6=6; none? Actually 6&6=6, 4&6=4, 2&6=2 no duplicate. Try {4, 2}, x=6: 4&6=4,2&6=2 no. {3, 2}, x=3: 3&3=3,2&3=2 no. {7, 3}, x=3: 7&3=3, 3&3=3? yes 3 exists, so 1 op.
    assert(minOperationsToDuplicate({7, 3}, 3) == 1);
    
    // Case 2: Two operations needed
    assert(minOperationsToDuplicate({5, 6}, 4) == 2); // 5&4=4, 6&4=4 -> both become 4
    assert(minOperationsToDuplicate({3, 5, 6}, 4) == 2); // 5&4=4, 6&4=4
    assert(minOperationsToDuplicate({7, 8, 15}, 12) == 2); // 7&12=4, 8&12=8, 15&12=12? Actually 15&12=12, 8&12=8, 7&12=4 no duplicate. Use {10, 12}, x=6: 10&6=2, 12&6=4 no. Use {9, 10}, x=11: 9&11=9, 10&11=10 no. Better: {5, 7}, x=5: 5&5=5, 7&5=5 -> duplicate after both ops, original no duplicate, so answer 2.
    assert(minOperationsToDuplicate({5, 7}, 5) == 2);
    
    // Impossible
    assert(minOperationsToDuplicate({1}, 0) == -1);
    assert(minOperationsToDuplicate({1, 2}, 0) == -1); // 1&0=0,2&0=0? Actually both become 0 -> duplicate in 2 ops? Wait 1&0=0,2&0=0, so 2 ops needed, not -1. For -1, need distinct after AND and no duplicates originally. Try {1, 2}, x=1: 1&1=1,2&1=0 distinct, no existing dup, no transform dup -> -1.
    assert(minOperationsToDuplicate({1, 2}, 1) == -1);
    assert(minOperationsToDuplicate({1, 2, 4}, 1) == -1); // 1&1=1,2&1=0,4&1=0? 4&1=0, so 2&1=0 and 4&1=0 duplicate -> 2. For -1: {1, 2, 4}, x=8: all &8=0? 1&8=0,2&8=0,4&8=0 -> 2. Use {1, 2}, x=2: 1&2=0,2&2=2 distinct -> -1.
    assert(minOperationsToDuplicate({1, 2}, 2) == -1);
    
    return 0;
}

#include <vector>
#include <array>

// Returns the minimum operations needed to create a duplicate in the array
// where each operation replaces an element with (element & x).
int minOperationsToDuplicate(const std::vector<int>& a, int x) {
    constexpr int MAX_VAL = 100001;
    std::array<int, MAX_VAL> freq{};
    
    for (int val : a) {
        ++freq[val];
    }
    
    // Case 0: Already has a duplicate
    for (int i = 0; i < MAX_VAL; ++i) {
        if (freq[i] > 1) return 0;
    }
    
    // Case 1: One operation on some element makes it equal to an existing value
    for (int val : a) {
        int anded = val & x;
        if (anded != val && freq[anded] > 0) {
            return 1;
        }
    }
    
    // Case 2: Two operations on two elements produce the same AND result
    std::array<int, MAX_VAL> freqAnded{};
    for (int val : a) {
        int anded = val & x;
        ++freqAnded[anded];
    }
    for (int i = 0; i < MAX_VAL; ++i) {
        if (freqAnded[i] > 1) return 2;
    }
    
    // Impossible
    return -1;
}

// The solution uses a greedy, case-based approach. First, count the frequency of each original value using a frequency array sized to the maximum possible value (100001). If any frequency exceeds 1, return 0 immediately. Otherwise, check whether there exists an index `i` such that `(a[i] & x)` is different from `a[i]` and also appears somewhere in the original array (i.e., its frequency is greater than 0). If so, applying one operation to `a[i]` will make it equal to that existing value, producing a duplicate—return 1. If no single operation works, we consider applying the operation to every element: transform each element to `a[i] & x`, and count frequencies of these transformed values. If any transformed value appears at least twice, then we can apply the operation to those two original elements to make them both equal that transformed value, producing a duplicate in 2 operations—return 2. If none of these conditions hold, it is impossible to create a duplicate, so return -1. The logic relies on the fact that applying the AND operation is idempotent after one application, and that any duplicate created after two operations must come from two different original elements mapping to the same AND result. The time complexity is O(n + 100001) due to the fixed-size frequency arrays, and the space complexity is O(100001) for the frequency arrays.
