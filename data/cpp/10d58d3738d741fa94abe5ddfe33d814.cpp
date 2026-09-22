Write a C++ function that accepts a fixed-size vector of exactly 4 integers (represented as `std::array<int, 4>` or similar) and modifies it in-place so that the last two elements are set to zero, while preserving the first two elements as they originally were. The function should return nothing (void) and operate directly on the input array. The input must always contain exactly 4 integers. The function must be named `zeroTailTwo`. Additionally, write a second overload or the same function should also handle a `std::vector<int>` of any size >= 2, setting only the last two elements to zero; for vectors with exactly 2 elements, both become zero; for vectors with 1 or 0 elements, no change occurs. You are only required to implement the 4-element `std::array` version for the main test, but the solution should include the vector version as an overload.

#include <cassert>
#include <array>
#include <vector>

int main() {
    // Test array version
    std::array<int, 4> a1 = {1, 2, 3, 4};
    zeroTailTwo(a1);
    assert(a1[0] == 1 && a1[1] == 2 && a1[2] == 0 && a1[3] == 0);

    std::array<int, 4> a2 = {-5, 0, 42, 7};
    zeroTailTwo(a2);
    assert(a2[0] == -5 && a2[1] == 0 && a2[2] == 0 && a2[3] == 0);

    // Test vector version
    std::vector<int> v1 = {10, 20, 30, 40, 50};
    zeroTailTwo(v1);
    assert(v1 == std::vector<int>({10, 20, 30, 0, 0}));

    std::vector<int> v2 = {3, 4};
    zeroTailTwo(v2);
    assert(v2 == std::vector<int>({0, 0}));

    std::vector<int> v3 = {5};
    zeroTailTwo(v3);
    assert(v3 == std::vector<int>({5}));

    std::vector<int> v4;
    zeroTailTwo(v4);
    assert(v4.empty());

    std::vector<int> v5 = {1};
    zeroTailTwo(v5);
    assert(v5 == std::vector<int>({1}));

    // Ensure original values preserved for first elements
    std::vector<int> v6 = {7, 8, 9, 10};
    zeroTailTwo(v6);
    assert(v6[0] == 7 && v6[1] == 8 && v6[2] == 0 && v6[3] == 0);
}

#include <array>
#include <vector>

// Zero out the last two elements of a 4-element array.
void zeroTailTwo(std::array<int, 4>& arr) {
    arr[2] = 0;
    arr[3] = 0;
}

// Zero out the last two elements of a vector, if it has at least two elements.
void zeroTailTwo(std::vector<int>& vec) {
    if (vec.size() >= 2) {
        vec[vec.size() - 2] = 0;
        vec[vec.size() - 1] = 0;
    }
}

// The core idea is to directly access the last two positions of the container and assign zero to them. For a `std::array<int, 4>`, this is trivial: set `arr[2] = 0` and `arr[3] = 0`. For a `std::vector<int>`, you need to check its size. If size >= 2, assign zero to `vec[vec.size()-2]` and `vec[vec.size()-1]`. If size == 0 or 1, do nothing because there are not two elements to modify. This direct indexing approach works in O(1) time and O(1) auxiliary space. Edge cases include an empty vector and a vector of size 1 — both are handled by the size check. The array version always has exactly 4 elements so no check is needed. For const correctness, the function modifies the input, so the parameter must be non-const reference. For the vector overload, we also need to include `<vector>` and `<array>` headers.
