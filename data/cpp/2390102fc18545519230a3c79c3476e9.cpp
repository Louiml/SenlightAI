// Write a C++ function `partitionNegatives` that takes a `std::vector<int>&` and rearranges its elements in-place so that all negative numbers appear before all non-negative numbers (zero and positive). The relative order of elements within each group is not required to be preserved. The function must handle vectors of any size, including empty and single-element vectors, and must modify the original vector without using any extra container (only O(1) auxiliary space beyond a few index variables). It should not return anything; the result is the modified vector itself.
// The algorithm uses a two-pointer technique: one pointer `left` starting at the beginning (index 0) and another `right` starting at the end (index size-1). While `left` is less than or equal to `right`, we inspect `arr[left]`. If `arr[left]` is negative, it is already in the correct partition, so we simply increment `left`. If `arr[left]` is non-negative (≥0), we swap it with `arr[right]` and decrement `right`, because the swapped-in element from the right might be non-negative too, so we must re-check it on the next iteration (but `left` is not incremented after a swap). This guarantees that all negatives end up on the left side and non-negatives on the right. Edge cases: empty vector (loop does not execute, fine), single element (if negative, left increments past right; if non-negative, swap with itself then right decrements below left, fine), all negatives (left moves to end, right stays at end, no swaps needed), all non-negatives (each swap brings another non-negative from right, but left never advances until the right pointer moves left; eventually left and right meet and the loop ends). Time complexity is O(n) because each element is examined at most once and each swap moves an element to its final position. Space complexity is O(1) auxiliary, only a few integer variables.
#include <vector>
#include <utility> // for std::swap

// Rearranges the vector in-place so that all negative numbers come before non-negative ones.
// Relative order within groups is not preserved.
void partitionNegatives(std::vector<int>& arr) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;

    while (left <= right) {
        if (arr[left] < 0) {
            // Already in the negative section.
            ++left;
        } else {
            // Swap the non-negative element to the right side.
            std::swap(arr[left], arr[right]);
            --right;
        }
    }
}
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Helper to check if all negatives come before non-negatives
    auto isPartitioned = [](const std::vector<int>& v) {
        bool seenNonNegative = false;
        for (int x : v) {
            if (x >= 0) seenNonNegative = true;
            else if (seenNonNegative) return false;
        }
        return true;
    };

    // Test 1: Given example
    std::vector<int> v1{-1, 7, 5, -3, 2, 1, -6};
    partitionNegatives(v1);
    assert(isPartitioned(v1));
    assert(v1.size() == 7);
    assert(std::count(v1.begin(), v1.end(), -1) == 1);
    assert(std::count(v1.begin(), v1.end(), -3) == 1);
    assert(std::count(v1.begin(), v1.end(), -6) == 1);
    assert(std::count(v1.begin(), v1.end(), 7) == 1);
    assert(std::count(v1.begin(), v1.end(), 5) == 1);
    assert(std::count(v1.begin(), v1.end(), 2) == 1);
    assert(std::count(v1.begin(), v1.end(), 1) == 1);

    // Test 2: Empty vector
    std::vector<int> v2;
    partitionNegatives(v2);
    assert(v2.empty());

    // Test 3: Single negative
    std::vector<int> v3{-5};
    partitionNegatives(v3);
    assert(v3 == std::vector<int>{-5});

    // Test 4: Single non-negative
    std::vector<int> v4{3};
    partitionNegatives(v4);
    assert(v4 == std::vector<int>{3});

    // Test 5: All negatives
    std::vector<int> v5{-2, -4, -1};
    partitionNegatives(v5);
    assert(isPartitioned(v5));
    assert(v5.size() == 3);

    // Test 6: All non-negative with zeros
    std::vector<int> v6{0, 2, 4};
    partitionNegatives(v6);
    assert(isPartitioned(v6));
    assert(v6.size() == 3);

    // Test 7: Mixed with zeros and negatives
    std::vector<int> v7{-1, 0, -2, 3, -4};
    partitionNegatives(v7);
    assert(isPartitioned(v7));
    assert(v7.size() == 5);
    assert(std::count(v7.begin(), v7.end(), -1) == 1);
    assert(std::count(v7.begin(), v7.end(), -2) == 1);
    assert(std::count(v7.begin(), v7.end(), -4) == 1);

    // Test 8: Already partitioned
    std::vector<int> v8{-3, -1, 0, 2, 5};
    partitionNegatives(v8);
    assert(isPartitioned(v8));
    assert(v8 == std::vector<int>({-3, -1, 0, 2, 5}));

    // Test 9: Reverse order
    std::vector<int> v9{5, 2, 0, -1, -3};
    partitionNegatives(v9);
    assert(isPartitioned(v9));
    assert(v9.size() == 5);

    // Test 10: Large vector alternating
    std::vector<int> v10;
    for (int i = 0; i < 100; ++i) {
        v10.push_back((i % 2 == 0) ? -i : i);
    }
    partitionNegatives(v10);
    assert(isPartitioned(v10));
    assert(v10.size() == 100);
    assert(std::count_if(v10.begin(), v10.end(), [](int x){ return x < 0; }) == 50);
    assert(std::count_if(v10.begin(), v10.end(), [](int x){ return x >= 0; }) == 50);

    return 0;
}
