Write a C++ function named `findModeCount` that takes a `std::array<int, 5>` by const reference and returns an `int` representing the number of times the most frequently occurring element appears in the array. The array may contain duplicate values, and you must count the maximum frequency of any single value. For example, for `{3, 4, 7, 2, 7}`, the value `7` appears twice, so the function should return `2`. If all elements are distinct, return `1`. The array always has exactly 5 elements, and you may assume the input is valid (no need to handle empty arrays). Use only standard library facilities and ensure the function is `const`-correct.
#include <cassert>
#include <array>

// Forward declaration of the solution function (as defined above)
int findModeCount(const std::array<int, 5>& arr);

int main() {
    std::array<int, 5> a1 = {3, 4, 7, 2, 7};
    assert(findModeCount(a1) == 2);

    std::array<int, 5> a2 = {1, 1, 1, 1, 1};
    assert(findModeCount(a2) == 5);

    std::array<int, 5> a3 = {1, 2, 3, 4, 5};
    assert(findModeCount(a3) == 1);

    std::array<int, 5> a4 = {9, 9, 3, 9, 2};
    assert(findModeCount(a4) == 3);

    std::array<int, 5> a5 = {-1, -1, -1, 0, 0};
    assert(findModeCount(a5) == 3);

    std::array<int, 5> a6 = {7, 7, 7, 2, 2};
    assert(findModeCount(a6) == 3);

    std::array<int, 5> a7 = {0, 0, 1, 1, 1};
    assert(findModeCount(a7) == 3);

    std::array<int, 5> a8 = {5, 5, 5, 5, 3};
    assert(findModeCount(a8) == 4);

    std::array<int, 5> a9 = {2, 3, 3, 3, 3};
    assert(findModeCount(a9) == 4);

    std::array<int, 5> a10 = {100, -100, 100, -100, 100};
    assert(findModeCount(a10) == 3);

    return 0;
}
#include <array>

// Returns the maximum frequency of any element in a fixed-size array of 5 ints.
// Example: {3,4,7,2,7} returns 2 because 7 appears twice.
int findModeCount(const std::array<int, 5>& arr) {
    int maxCount = 0;
    const int n = arr.size(); // n is 5

    for (int i = 0; i < n; ++i) {
        // Skip if this element has already been processed (appeared earlier)
        bool isDuplicate = false;
        for (int j = 0; j < i; ++j) {
            if (arr[j] == arr[i]) {
                isDuplicate = true;
                break;
            }
        }
        if (isDuplicate) continue;

        // Count occurrences of arr[i] from i to end
        int currentCount = 0;
        for (int k = i; k < n; ++k) {
            if (arr[k] == arr[i]) {
                ++currentCount;
            }
        }
        if (currentCount > maxCount) {
            maxCount = currentCount;
        }
    }
    return maxCount;
}
// The solution involves iterating over the fixed-size array and counting occurrences of each element. Since the array size is constant (5), a straightforward nested loop works: for each element at index `i`, count how many times that element appears in the entire array, and track the maximum count seen. However, this naive approach double-counts duplicates (e.g., for `7` at indices 2 and 4, it would count 2 twice). To avoid redundancy, we can instead use a frequency map (e.g., `std::map<int, int>` or `std::unordered_map`), but given the small fixed size, a simpler in-place approach is to iterate through each unique element only once by checking if the current element has been seen before at an earlier index; if not, count its total occurrences and update the maximum. Edge cases include all elements distinct (max frequency = 1), all elements identical (max frequency = 5), and mixed duplicates. Time complexity is O(n^2) for the nested loop with a check for earlier occurrences, but with n=5 this is constant. Alternatively, using a frequency map yields O(n) time and O(n) space, but since n is constant, both are acceptable; we choose the map for clarity and correctness, or the nested loop with uniqueness check to save space. The space complexity is O(1) extra for the nested-loop approach (only a few ints) or O(n) for the map, but both are constant due to fixed n. The main algorithm: initialize `maxCount = 0`, loop `i` from 0 to 4, if the current element hasn't appeared before index `i` (check by scanning from 0 to i-1), then count all occurrences from `i` to 4, update `maxCount` with that count. Return `maxCount`.
