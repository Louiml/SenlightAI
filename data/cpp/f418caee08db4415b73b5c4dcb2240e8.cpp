/*
Write a C++ function named `stablePartitionOddEven` that takes a `std::vector<int>` by reference and reorders its elements so that all odd integers appear before all even integers, while preserving the original relative order among odds and among evens. The function must modify the input vector in place (i.e., it should not return a new vector, but rather rearrange the given vector). The input vector may be empty, contain duplicates, negative numbers, or be already sorted in the desired order; all such cases must be handled correctly. Do not use `std::stable_partition` or any other standard library algorithm that directly solves the partition problem—implement the logic manually using only basic containers and operations. The function should be declared in a header-style free function with appropriate `const` correctness (note: the parameter must be non-const because it is modified).
*/

#include <vector>

// Reorders the vector so that all odd numbers come before all even numbers,
// preserving the original relative order within the odd and even groups.
void stablePartitionOddEven(std::vector<int>& arr) {
    std::vector<int> temp;
    temp.reserve(arr.size());

    // First pass: collect all odd numbers in original order.
    for (int x : arr) {
        if (x % 2 != 0) {
            temp.push_back(x);
        }
    }
    // Second pass: collect all even numbers in original order.
    for (int x : arr) {
        if (x % 2 == 0) {
            temp.push_back(x);
        }
    }

    // Move the result back into the input vector.
    arr.swap(temp);
}

#include <cassert>
#include <vector>

// The solution function is declared above; no main needed here except the test.
int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    stablePartitionOddEven(v1);
    assert((v1 == std::vector<int>{1, 3, 5, 2, 4}));

    std::vector<int> v2 = {2, 4, 6};
    stablePartitionOddEven(v2);
    assert((v2 == std::vector<int>{2, 4, 6}));

    std::vector<int> v3 = {1, 3, 5};
    stablePartitionOddEven(v3);
    assert((v3 == std::vector<int>{1, 3, 5}));

    std::vector<int> v4 = {};
    stablePartitionOddEven(v4);
    assert(v4.empty());

    std::vector<int> v5 = {-3, -2, -1, 0, 1, 2};
    stablePartitionOddEven(v5);
    assert((v5 == std::vector<int>{-3, -1, 1, -2, 0, 2})); // negative odds and evens

    std::vector<int> v6 = {0, -5, 3, -2, 7, 4};
    stablePartitionOddEven(v6);
    assert((v6 == std::vector<int>{-5, 3, 7, 0, -2, 4}));

    std::vector<int> v7 = {1, 1, 2, 2, 3, 3};
    stablePartitionOddEven(v7);
    assert((v7 == std::vector<int>{1, 1, 3, 3, 2, 2}));

    std::vector<int> v8 = {2, 1};
    stablePartitionOddEven(v8);
    assert((v8 == std::vector<int>{1, 2}));
}

// The core requirement is to maintain relative order, which rules out the classic two-pointer swap approach (that only ensures all odds left of evens but breaks stability). A straightforward stable approach is to create a temporary vector, iterate through the original vector twice: first pushing all odd numbers in their original order, then pushing all even numbers in their original order. Finally, assign the temporary vector back to the input vector. This uses O(n) extra space (the temporary vector) and runs in O(n) time because each element is inspected exactly twice. Edge cases include an empty vector (no issues), all odd, all even, and negative numbers—since negativity does not affect odd/even classification (use `x % 2 != 0` for odd, because C++ modulo with negative numbers yields a negative remainder, but `% 2` still gives 0 or ±1; checking `!= 0` works for both positive and negative integers). The solution is simple and meets the requirement of preserving stability.
