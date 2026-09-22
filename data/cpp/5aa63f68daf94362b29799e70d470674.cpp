Write a C++ function `findSecondSmallestAndLargest` that takes a non-empty array of integers (using a `std::vector<int>` for safety and flexibility) and returns a `std::pair<int, int>` where the first element is the second smallest distinct value and the second element is the second largest distinct value. If the array contains fewer than two distinct values, the function should return `std::make_pair(std::numeric_limits<int>::max(), std::numeric_limits<int>::min())` to indicate that the values do not exist. The function must handle duplicate values correctly, meaning that if all elements are equal, there is no distinct second smallest or second largest, and the special pair should be returned. The function should not mutate the input, work for any size ≥ 1, and handle negative numbers and extreme values (e.g., `INT_MIN` and `INT_MAX`). You must implement the logic in a single pass, avoiding sorting or extra containers beyond a few scalar variables.

The solution approach is to traverse the array once while maintaining four variables: `smallest`, `secondSmallest`, `largest`, `secondLargest`. Initialize `smallest = secondSmallest = INT_MAX` and `largest = secondLargest = INT_MIN`. For each element `x`:
- **Largest tracking:** If `x > largest`, then shift the old `largest` into `secondLargest` and update `largest = x`. Else if `x > secondLargest` and `x != largest`, update `secondLargest = x`.
- **Smallest tracking:** If `x < smallest`, then shift the old `smallest` into `secondSmallest` and update `smallest = x`. Else if `x < secondSmallest` and `x != smallest`, update `secondSmallest = x`.

After the loop, if `secondSmallest` is still `INT_MAX` or `secondLargest` is still `INT_MIN`, it means there are fewer than two distinct values, so return the sentinel pair. Otherwise return `{secondSmallest, secondLargest}`.

Edge cases: arrays of length 1, arrays with all identical elements, arrays with exactly two distinct values (then those two are the smallest and second smallest, and largest and second largest), negative numbers, and values equal to `INT_MAX` or `INT_MIN` (since we use `!=` comparison, duplicates of extremes are handled correctly). Time complexity is O(n) with a single pass, space complexity is O(1) auxiliary (excluding input storage).

#include <vector>
#include <utility>
#include <limits>

// Returns a pair {secondSmallest, secondLargest} from the array.
// If fewer than two distinct values exist, returns {INT_MAX, INT_MIN}.
std::pair<int, int> findSecondSmallestAndLargest(const std::vector<int>& arr) {
    int smallest = std::numeric_limits<int>::max();
    int secondSmallest = std::numeric_limits<int>::max();
    int largest = std::numeric_limits<int>::min();
    int secondLargest = std::numeric_limits<int>::min();

    for (int x : arr) {
        // Update largest and second largest
        if (x > largest) {
            secondLargest = largest;
            largest = x;
        } else if (x > secondLargest && x != largest) {
            secondLargest = x;
        }

        // Update smallest and second smallest
        if (x < smallest) {
            secondSmallest = smallest;
            smallest = x;
        } else if (x < secondSmallest && x != smallest) {
            secondSmallest = x;
        }
    }

    if (secondSmallest == std::numeric_limits<int>::max() ||
        secondLargest == std::numeric_limits<int>::min()) {
        return {std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
    }
    return {secondSmallest, secondLargest};
}

#include <cassert>
#include <vector>
#include <limits>

// The function declaration (ensure it matches the solution above)
std::pair<int, int> findSecondSmallestAndLargest(const std::vector<int>& arr);

int main() {
    // Basic case with distinct values
    auto result1 = findSecondSmallestAndLargest({8, 4, 3, 2, 1});
    assert(result1.first == 2 && result1.second == 4);

    // Negative and duplicate values
    auto result2 = findSecondSmallestAndLargest({-5, -1, -5, -10, -1});
    assert(result2.first == -5 && result2.second == -1);

    // All equal -> sentinel
    auto result3 = findSecondSmallestAndLargest({7, 7, 7});
    assert(result3.first == std::numeric_limits<int>::max() &&
           result3.second == std::numeric_limits<int>::min());

    // Only one element -> sentinel
    auto result4 = findSecondSmallestAndLargest({42});
    assert(result4.first == std::numeric_limits<int>::max() &&
           result4.second == std::numeric_limits<int>::min());

    // Two distinct values, both extremes
    auto result5 = findSecondSmallestAndLargest({-100, 100});
    assert(result5.first == -100 && result5.second == 100);

    // Mix with INT_MAX and INT_MIN (duplicates)
    auto result6 = findSecondSmallestAndLargest({std::numeric_limits<int>::max(),
                                                 std::numeric_limits<int>::max(),
                                                 std::numeric_limits<int>::min(),
                                                 std::numeric_limits<int>::min(),
                                                 0});
    assert(result6.first == std::numeric_limits<int>::min() &&
           result6.second == 0);

    // Duplicate smallest and largest
    auto result7 = findSecondSmallestAndLargest({1, 1, 2, 2, 3, 3});
    assert(result7.first == 2 && result7.second == 2);

    // Unsorted and large array
    auto result8 = findSecondSmallestAndLargest({100, 1, 50, 2, 3, 99, -1});
    assert(result8.first == 1 && result8.second == 99);

    // All same negative values
    auto result9 = findSecondSmallestAndLargest({-3, -3, -3});
    assert(result9.first == std::numeric_limits<int>::max() &&
           result9.second == std::numeric_limits<int>::min());

    return 0;
}
