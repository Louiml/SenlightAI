Write a C++ function `minimumAdditionsToNonDecreasing` that takes a non-empty `std::vector<long long>` of integers and returns the minimum total number of increments needed to make the entire array non-decreasing (i.e., each element is greater than or equal to the previous one). You may only increase elements; you cannot decrease or reorder them. For example, given `[3, 1, 2]`, you must increase the second element from 1 to 3 (adding 2) and the third element from 2 to 3 (adding 1), totaling 3. The input fits within 64-bit signed integers, and the result will also fit within 64-bit signed integers.
The problem is equivalent to computing the minimum total additions so that the sequence becomes non-decreasing. The optimal strategy is to iterate from left to right, maintaining the current maximum value seen so far (`current_max`). For each element at index `i` (starting from index 1), if the element is less than `current_max`, we must raise it to `current_max` by adding `(current_max - nums[i])` to the answer. Then update `current_max` to `max(current_max, nums[i])` (which, after raising, is simply `current_max`). This works because making an element equal to the previous maximum ensures the property holds, and any smaller addition would still be invalid. Edge cases: the array has only one element → answer is 0; all elements already non-decreasing → answer is 0; negative numbers are handled naturally since we only add positive differences. The algorithm runs in O(n) time and O(1) auxiliary space.
#include <vector>
#include <algorithm>

// Returns the minimum total increments required to make the vector non-decreasing.
// Elements can only be increased, never decreased or reordered.
long long minimumAdditionsToNonDecreasing(const std::vector<long long>& nums) {
    if (nums.empty()) return 0;

    long long total_additions = 0;
    long long current_max = nums[0];

    for (size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] < current_max) {
            total_additions += (current_max - nums[i]);
        } else {
            current_max = nums[i]; // update if nums[i] is larger
        }
    }
    return total_additions;
}
#include <cassert>
#include <vector>

int main() {
    assert(minimumAdditionsToNonDecreasing({1, 2, 3}) == 0);
    assert(minimumAdditionsToNonDecreasing({3, 1, 2}) == 3);
    assert(minimumAdditionsToNonDecreasing({5, 4, 3, 2, 1}) == 10); // 1+2+3+4
    assert(minimumAdditionsToNonDecreasing({10}) == 0);
    assert(minimumAdditionsToNonDecreasing({-3, -5, 4}) == 2); // raise -5 to -3
    assert(minimumAdditionsToNonDecreasing({2, 2, 2}) == 0);
    assert(minimumAdditionsToNonDecreasing({1, 100, 2, 3}) == 197); // raise 2->100 (98), 3->100 (97)
    assert(minimumAdditionsToNonDecreasing({0, -10, 20}) == 10);
}
