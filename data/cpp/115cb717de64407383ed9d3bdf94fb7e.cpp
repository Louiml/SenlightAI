Write a C++ function `prefixCommonCount` that takes two vectors of integers `A` and `B`, both of the same length `n`, containing a permutation of the integers `1` through `n` (each number appears exactly once in each vector, but the order may differ). The function must return a vector of integers `ans` of length `n` where `ans[i]` equals the number of integers `x` such that `x` appears in the prefix `A[0..i]` AND also appears in the prefix `B[0..i]`. In other words, for each index `i`, count how many numbers have appeared in both vectors up to that index. The solution must handle all valid permutations correctly and be efficient.
The key observation is that since both `A` and `B` contain a permutation of the same set of numbers, we can track the occurrence count of each number as we scan both vectors from left to right. Maintain a hash map (or array) `count` that stores how many times a value has been seen so far (0, 1, or 2). As we process index `i`, we increment the count for `A[i]` and then for `B[i]`. If after incrementing a value's count reaches exactly 2, that means this value is now present in both prefixes up to index `i`, so we increment a running `common` counter. The prefix common count for index `i` is simply the current value of `common`. This works because each value appears at most once in each vector, so a value can only reach count 2 exactly once. Edge cases: if `A` and `B` are identical, then at each index the value at that position becomes common immediately; if the vectors are reversed order, common values accumulate gradually. Since the values are from `1` to `n`, we could even use a fixed-size array for `O(n)` space instead of a hash map, but for generality, an unordered_map is fine. Time complexity is `O(n)` because each vector element is processed once in constant time; space complexity is `O(n)` for the map and the result vector.
#include <vector>
#include <unordered_map>

// Given two permutations A and B of the integers 1..n, return a vector where
// the i-th element is the number of integers present in both A[0..i] and B[0..i].
std::vector<int> prefixCommonCount(const std::vector<int>& A, const std::vector<int>& B) {
    const int n = static_cast<int>(A.size());
    std::vector<int> ans;
    ans.reserve(n);

    std::unordered_map<int, int> count;
    int common = 0;

    for (int i = 0; i < n; ++i) {
        // Process A[i]
        ++count[A[i]];
        if (count[A[i]] == 2) {
            ++common;
        }

        // Process B[i]
        ++count[B[i]];
        if (count[B[i]] == 2) {
            ++common;
        }

        ans.push_back(common);
    }

    return ans;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be available from the section.
// Test cases:
int main() {
    // Example 1: identical permutations
    std::vector<int> A1 = {1, 2, 3};
    std::vector<int> B1 = {1, 2, 3};
    std::vector<int> result1 = prefixCommonCount(A1, B1);
    assert(result1 == std::vector<int>({1, 2, 3}));

    // Example 2: reversed order
    std::vector<int> A2 = {1, 2, 3};
    std::vector<int> B2 = {3, 2, 1};
    std::vector<int> result2 = prefixCommonCount(A2, B2);
    assert(result2 == std::vector<int>({0, 1, 3}));

    // Example 3: length 1
    std::vector<int> A3 = {5};
    std::vector<int> B3 = {5};
    std::vector<int> result3 = prefixCommonCount(A3, B3);
    assert(result3 == std::vector<int>({1}));

    // Example 4: length 4 mixed order
    std::vector<int> A4 = {1, 3, 2, 4};
    std::vector<int> B4 = {3, 1, 4, 2};
    std::vector<int> result4 = prefixCommonCount(A4, B4);
    // i=0: A0=1, B0=3 -> common=0
    // i=1: A1=3 (now sees 3 twice), B1=1 (now sees 1 twice) -> common=2
    // i=2: A2=2 (count 1), B2=4 (count 1) -> common=2
    // i=3: A3=4 (now sees 4 twice), B3=2 (now sees 2 twice) -> common=4
    assert(result4 == std::vector<int>({0, 2, 2, 4}));

    // Example 5: larger permutation with 5 elements
    std::vector<int> A5 = {2, 1, 5, 4, 3};
    std::vector<int> B5 = {5, 2, 3, 1, 4};
    std::vector<int> result5 = prefixCommonCount(A5, B5);
    // i=0: A0=2,B0=5 -> common=0
    // i=1: A1=1,B1=2 -> sees 2 twice -> common=1
    // i=2: A2=5,B2=3 -> sees 5 twice -> common=2
    // i=3: A3=4,B3=1 -> sees 1 twice -> common=3
    // i=4: A4=3,B4=4 -> sees 3 and 4 twice -> common=5
    assert(result5 == std::vector<int>({0, 1, 2, 3, 5}));

    return 0;
}
