// Write a C++ function `std::vector<int> fixPermutation(int N, const std::vector<int>& input)` that takes a positive integer `N` and a vector of `N` integers. The input contains a sequence of integers, some of which may be duplicates. The function must process the sequence from left to right, keeping only the first occurrence of each distinct integer in their original relative order. The first kept integer becomes the first element of the result vector; for each subsequent kept integer, place it at its original position, but insert the immediately previous kept integer at that position (i.e., shift the previous kept value forward). Leave all other positions as 0 initially. After processing, if the set of kept distinct integers has size exactly 1, return a vector containing just `-1`. Otherwise, fill all zero positions from left to right with the smallest positive integers not already present in the set of kept values (starting from 1 upward), using each once. Return the fully constructed vector of length `N`. For example, with `N=5` and input `[3, 1, 3, 2, 1]`, the kept values in order are `[3, 1, 2]`; the result vector after placement becomes `[0, 0, 3, 1, 2]`; then zeros at positions 0 and 1 are filled with 4 and 5 (since 1,2,3 already present), yielding `[4, 5, 3, 1, 2]`.
// The algorithm maintains an unordered set `seen` to track distinct values encountered as we iterate. We also keep a vector `result` initialized to zeros of length `N`. We keep a variable `prevKept` that stores the last kept integer. For each index `i`, if `input[i]` is not in `seen`, we insert it, and if `prevKept` is non-zero, we place `prevKept` at `result[i]` (because at this position we want the previous kept value). Then we update `prevKept` to `input[i]`. After processing all elements, the last kept value (`prevKept`) should be placed at index 0 (since the first kept value is placed at the end of the chain? Wait, careful reading: For the first kept value, there is no previous, so we do not place anything; after processing all, we set `result[0] = prevKept`? Actually the snippet does `order[0] = prev;` after loop, which sets the first element to the last kept value. That is a bit odd but matches the snippet: The logic is that each kept value except the first gets placed at its position, and the first kept value is placed at index 0 after the loop. So yes, after the loop, set `result[0] = prevKept` (which is the last kept value). If `seen.size() == 1`, return `[-1]`. Otherwise, fill zeros: maintain `curr = 1`, for each index i, if `result[i]==0`, while `seen.find(curr)!=seen.end()`, increment `curr`; then set `result[i]=curr` and increment `curr`. Complexity: O(N) time and O(N) space for the set and result. Edge cases: N=1 with a single value → seen size 1 → return -1. Duplicates do not affect the placement because only first occurrences are processed. If input contains only one distinct value but N>1, the result after placement would be `[v, 0,0,...]` and then we return -1.
#include <vector>
#include <unordered_set>
#include <algorithm>

// Returns a permutation of length N based on the described rule.
std::vector<int> fixPermutation(int N, const std::vector<int>& input) {
    std::vector<int> result(N, 0);
    std::unordered_set<int> seen;
    int prevKept = 0;  // 0 indicates no previous kept value

    for (int i = 0; i < N; ++i) {
        int num = input[i];
        if (seen.find(num) == seen.end()) {
            seen.insert(num);
            if (prevKept != 0) {
                result[i] = prevKept;
            }
            prevKept = num;
        }
    }
    result[0] = prevKept;  // place the last kept value at the front

    if (seen.size() == 1) {
        return {-1};
    }

    int currBank = 1;
    for (int i = 0; i < N; ++i) {
        if (result[i] == 0) {
            while (seen.find(currBank) != seen.end()) {
                ++currBank;
            }
            result[i] = currBank;
            ++currBank;
        }
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic example from description
    std::vector<int> input1 = {3, 1, 3, 2, 1};
    std::vector<int> expected1 = {4, 5, 3, 1, 2};
    assert(fixPermutation(5, input1) == expected1);

    // All distinct values
    std::vector<int> input2 = {1, 2, 3, 4};
    std::vector<int> expected2 = {4, 1, 2, 3};
    assert(fixPermutation(4, input2) == expected2);

    // Single distinct value → return -1
    std::vector<int> input3 = {7, 7, 7, 7};
    assert(fixPermutation(4, input3) == std::vector<int>{-1});

    // N=1 with one distinct
    std::vector<int> input4 = {5};
    assert(fixPermutation(1, input4) == std::vector<int>{-1});

    // Duplicates non-consecutive, size 2 distinct
    std::vector<int> input5 = {2, 5, 2, 1, 5};
    // Kept order: 2,5,1. Placement: index1=5, index3=1, result[0]=1? Wait: prevKept after loop is last kept 1, so result[0]=1, result[1]=5, result[3]=1? Actually check: i0=2 (first kept, prevKept=0 so no placement, prevKept=2), i1=5 (first kept, result[1]=2, prevKept=5), i2=2 duplicate skip, i3=1 first kept, result[3]=5, prevKept=1), i4=5 duplicate skip. After loop result: [0,2,0,5,0], then set result[0]=1 → [1,2,0,5,0]. Fill zeros with 3,4: positions 2→3,4→4 → [1,2,3,5,4].
    std::vector<int> expected5 = {1, 2, 3, 5, 4};
    assert(fixPermutation(5, input5) == expected5);

    // All same but N=3, still -1
    std::vector<int> input6 = {9, 9, 9};
    assert(fixPermutation(3, input6) == std::vector<int>{-1});

    // No zeros after placement? e.g., N=2, input {1,2} → kept both, result[1]=1, result[0]=2 → [2,1], no zeros.
    std::vector<int> input7 = {1, 2};
    std::vector<int> expected7 = {2, 1};
    assert(fixPermutation(2, input7) == expected7);

    // Larger test with more distinct
    std::vector<int> input8 = {1, 1, 1, 2, 2, 3};
    // Kept order: 1,2,3. Placement: i0=1 first, no placement, prev=1; i3=2 first, result[3]=1, prev=2; i5=3 first, result[5]=2, prev=3. After loop result[0]=3 → [3,0,0,1,0,2]. Fill zeros with 4,5,6 at positions 1,2,4 → [3,4,5,1,6,2].
    std::vector<int> expected8 = {3, 4, 5, 1, 6, 2};
    assert(fixPermutation(6, input8) == expected8);

    // Minimal N=2 with one distinct
    std::vector<int> input9 = {4, 4};
    assert(fixPermutation(2, input9) == std::vector<int>{-1});
}
