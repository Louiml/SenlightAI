// Given a sorted array of distinct integers and a target value, write a C++ function that performs a binary search to find the insertion position of the target in the array—the index where the target should be inserted to maintain the sorted order. If the target is already present, return its index. The function should be templated to work with any comparable type and must return the lower bound position (the first position where the target could be inserted without breaking the sort order).

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test with integers: target already present.
    std::vector<int> v1 = {1, 3, 5, 7, 9};
    assert(lower_bound_index(v1, 5) == 2);
    assert(lower_bound_index(v1, 1) == 0);
    assert(lower_bound_index(v1, 9) == 4);

    // Test with integers: target absent (insertion position).
    assert(lower_bound_index(v1, 0) == 0);
    assert(lower_bound_index(v1, 2) == 1);
    assert(lower_bound_index(v1, 6) == 3);
    assert(lower_bound_index(v1, 10) == 5);

    // Test with single element vector.
    std::vector<int> v2 = {42};
    assert(lower_bound_index(v2, 42) == 0);
    assert(lower_bound_index(v2, 41) == 0);
    assert(lower_bound_index(v2, 43) == 1);

    // Test with empty vector.
    std::vector<int> v3;
    assert(lower_bound_index(v3, 0) == 0);

    // Test with strings.
    std::vector<std::string> v4 = {"apple", "banana", "cherry"};
    assert(lower_bound_index(v4, std::string("banana")) == 1);
    assert(lower_bound_index(v4, std::string("blueberry")) == 2);
    assert(lower_bound_index(v4, std::string("apricot")) == 1);

    // Verify against std::lower_bound for random data.
    std::vector<int> v5 = {-10, -3, 0, 4, 8, 15};
    for (int x = -15; x <= 20; ++x) {
        auto expected = std::lower_bound(v5.begin(), v5.end(), x) - v5.begin();
        assert(lower_bound_index(v5, x) == static_cast<size_t>(expected));
    }

    return 0;
}

#include <vector>
#include <iterator>

/**
 * @brief Find the lower bound position of a target value in a sorted range.
 *
 * @tparam RandomIt Random-access iterator type.
 * @tparam T Type of the target value.
 * @param first Iterator to the first element.
 * @param last Iterator to one past the last element.
 * @param target Value to search for.
 * @return Iterator to the first element not less than the target.
 */
template <typename RandomIt, typename T>
RandomIt lower_bound_position(RandomIt first, RandomIt last, const T& target) {
    // Use difference type to handle arithmetic correctly for large ranges.
    using DiffType = typename std::iterator_traits<RandomIt>::difference_type;
    DiffType count = std::distance(first, last);

    while (count > 0) {
        DiffType step = count / 2;
        RandomIt mid = first;
        std::advance(mid, step);

        if (*mid < target) {
            first = std::next(mid);
            count -= step + 1;
        } else {
            count = step;
        }
    }
    return first;
}

/**
 * @brief Convenience overload for std::vector and similar containers.
 *
 * @param container Sorted container with random-access iterators.
 * @param target Value to search for.
 * @return Index of the lower bound position.
 */
template <typename Container, typename T>
typename Container::size_type lower_bound_index(const Container& container, const T& target) {
    auto it = lower_bound_position(std::begin(container), std::end(container), target);
    return static_cast<typename Container::size_type>(std::distance(std::begin(container), it));
}

// The solution implements the standard lower-bound binary search algorithm. The function takes a sorted range defined by two forward iterators (or a container with random-access iterators) and a target value. The main algorithm maintains two pointers, `low` and `high`, representing the current search interval `[low, high)`. At each step, it computes the midpoint `mid` and compares the element at `mid` with the target. If the element is less than the target, the search continues in the right half (`low = mid + 1`); otherwise, the search continues in the left half (`high = mid`). This process continues until `low == high`, at which point `low` is the insertion position. The algorithm handles all edge cases: when the target is smaller than all elements (returns 0), larger than all elements (returns the container size), or already present (returns the index of the first occurrence due to the `>=` comparison logic). The time complexity is \(O(\log n)\) for an array of \(n\) elements, and the space complexity is \(O(1)\) auxiliary space since only a constant number of variables are used.
