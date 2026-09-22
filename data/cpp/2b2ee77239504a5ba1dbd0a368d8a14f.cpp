Write a C++ function named `rotateArrayLeftByD` that takes a `std::vector<int>` by const reference and an integer `d` (the number of positions to rotate left), and returns a new `std::vector<int>` with the elements rotated left by `d` positions. The rotation should be performed using the Juggling Algorithm (based on GCD cycles) as shown in the snippet. The function must handle `d` values larger than the vector size (i.e., use `d % n` first), handle `d = 0` (return a copy), handle empty vectors (return an empty vector), and preserve the order of elements after rotation. Do not modify the input vector. The solution must not use the STL `std::rotate` or any other built-in rotation functions.

// The Juggling Algorithm rotates the array by breaking it into `gcd(n, d)` independent cycles. Each cycle starts at an index `i` (from 0 to gcd-1), stores the element at `i` in a temporary variable, then moves elements along the cycle: for each step, the element at index `j` is replaced by the element at index `(j + d) % n`, continuing until we return to the starting index `i`, then place the temporary value at the final position. This avoids overwriting unprocessed elements. Key edge cases: (1) if `d` is negative or larger than `n`, we first normalize `d = ((d % n) + n) % n` to handle both negative and overflow; (2) if `n == 0` or `d == 0`, return a copy of the input; (3) the algorithm works for all positive `n` and `d`. Time complexity is O(n) since each element is moved exactly once across all cycles. Auxiliary space is O(1) besides the output vector (which is O(n) by necessity) and the recursion depth for gcd (O(log n) worst case). The reference implementation uses an iterative `gcd` to avoid recursion overhead, but recursion is also fine.

#include <vector>
#include <numeric> // for std::gcd (C++17)

// Rotate vector left by d positions using the Juggling Algorithm.
std::vector<int> rotateArrayLeftByD(const std::vector<int>& arr, int d) {
    int n = static_cast<int>(arr.size());
    if (n == 0 || d % n == 0) {
        return arr; // no rotation needed or empty
    }

    // Normalize d to be in [0, n-1], handling negative and large d
    d = ((d % n) + n) % n;
    if (d == 0) return arr;

    int cycles = std::gcd(n, d);

    for (int i = 0; i < cycles; ++i) {
        int temp = arr[i];
        int j = i;
        while (true) {
            int next = j + d;
            if (next >= n) next -= n;

            if (next == i) break;

            arr[j] = arr[next];
            j = next;
        }
        arr[j] = temp;
    }

    return arr;
}

#include <cassert>
#include <vector>
#include <iostream>

// (The solution function is defined above; for test, place it here.)

int main() {
    // Basic case: rotate by 2
    std::vector<int> v1 = {1, 2, 3, 4, 5, 6, 7};
    assert(rotateArrayLeftByD(v1, 2) == std::vector<int>({3, 4, 5, 6, 7, 1, 2}));

    // Rotate by n (no change)
    std::vector<int> v2 = {1, 2, 3, 4};
    assert(rotateArrayLeftByD(v2, 4) == std::vector<int>({1, 2, 3, 4}));

    // Rotate by 0
    assert(rotateArrayLeftByD(v2, 0) == std::vector<int>({1, 2, 3, 4}));

    // Rotate by d > n
    std::vector<int> v3 = {10, 20, 30};
    assert(rotateArrayLeftByD(v3, 5) == std::vector<int>({30, 10, 20})); // 5 % 3 = 2

    // Rotate by negative d (simulate left rotate by negative = right rotate)
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    assert(rotateArrayLeftByD(v4, -1) == std::vector<int>({2, 3, 4, 5, 1})); // -1 % 5 = 4, but normalized? Actually -1 mod 5 = 4, left rotate by 4 = right rotate by 1, result is {2,3,4,5,1}

    // Empty vector
    std::vector<int> v5;
    assert(rotateArrayLeftByD(v5, 3).empty());

    // Single element
    std::vector<int> v6 = {42};
    assert(rotateArrayLeftByD(v6, 1) == std::vector<int>({42}));

    // Verify original unchanged
    std::vector<int> original = {1, 2, 3};
    rotateArrayLeftByD(original, 1);
    assert(original == std::vector<int>({1, 2, 3}));

    // Large d exactly n+1
    std::vector<int> v7 = {1, 2, 3, 4};
    assert(rotateArrayLeftByD(v7, 5) == std::vector<int>({2, 3, 4, 1}));

    std::cout << "All tests passed." << std::endl;
    return 0;
}
