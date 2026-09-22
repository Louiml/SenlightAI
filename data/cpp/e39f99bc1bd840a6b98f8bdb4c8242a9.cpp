// Write a C++ function `searchRange` that takes a sorted vector of integers (`nums`) and a `target` integer, and returns a `std::vector<int>` of size 2 containing the starting and ending indices of all occurrences of `target` in the vector. If `target` is not present, return `{-1, -1}`. The vector may be empty, may contain duplicates, and is sorted in non-decreasing order. Your solution must be efficient, avoiding a full linear scan, and must correctly handle edge cases such as `target` appearing once, appearing multiple times at the very beginning or end of the vector, and an empty input.
The main algorithm first uses binary search (`search`) to locate any occurrence of `target`. If none is found, immediately return `{-1, -1}`. Once an index is found, we expand leftwards and rightwards from that index to find the boundaries of the contiguous run of `target` values. However, a naive linear expansion could be `O(n)` in the worst case. To guarantee `O(log n)` time, we should instead use two additional binary searches: one to find the first occurrence (lower bound) and one to find the last occurrence (upper bound - 1). However, given the problem statement often accepts the linear expansion approach (which is `O(n)` worst-case but simple), we'll present the binary-search-based approach for the reference solution to satisfy efficiency. The time complexity is `O(log n)` for each of the three searches (or two searches if we use lower/upper bound), and space complexity is `O(1)` besides the output vector. Edge cases: empty vector returns `{-1,-1}`; `target` smaller than all elements or larger than all elements returns `{-1,-1}`; `target` appears exactly once returns `{i,i}`; all elements identical and equal to target returns `{0, n-1}`.
#include <vector>
#include <algorithm>

// Returns the first and last index of target in sorted nums, or {-1,-1} if absent.
std::vector<int> searchRange(const std::vector<int>& nums, int target) {
    auto lower = std::lower_bound(nums.begin(), nums.end(), target);
    if (lower == nums.end() || *lower != target) {
        return {-1, -1};
    }
    auto upper = std::upper_bound(nums.begin(), nums.end(), target);
    int start = static_cast<int>(lower - nums.begin());
    int end = static_cast<int>(upper - nums.begin()) - 1;
    return {start, end};
}
#include <cassert>
#include <vector>

// Forward declaration (the solution function is defined above in the same file)
std::vector<int> searchRange(const std::vector<int>& nums, int target);

int main() {
    // Example from prompt
    std::vector<int> vec1 = {5,7,7,8,8,10};
    assert(searchRange(vec1, 8) == std::vector<int>({3,4}));
    assert(searchRange(vec1, 6) == std::vector<int>({-1,-1}));

    // Target appears once
    assert(searchRange(vec1, 5) == std::vector<int>({0,0}));
    assert(searchRange(vec1, 10) == std::vector<int>({5,5}));

    // Empty vector
    std::vector<int> empty;
    assert(searchRange(empty, 1) == std::vector<int>({-1,-1}));

    // All elements equal target
    std::vector<int> allSame = {7,7,7,7};
    assert(searchRange(allSame, 7) == std::vector<int>({0,3}));

    // Target at very beginning multiple times
    std::vector<int> startRepeated = {2,2,3,5};
    assert(searchRange(startRepeated, 2) == std::vector<int>({0,1}));

    // Target at very end multiple times
    std::vector<int> endRepeated = {1,3,4,4};
    assert(searchRange(endRepeated, 4) == std::vector<int>({2,3}));

    // Target not present but between values
    assert(searchRange(vec1, 9) == std::vector<int>({-1,-1}));

    // Single element vector
    std::vector<int> single = {3};
    assert(searchRange(single, 3) == std::vector<int>({0,0}));
    assert(searchRange(single, 2) == std::vector<int>({-1,-1}));

    return 0;
}
