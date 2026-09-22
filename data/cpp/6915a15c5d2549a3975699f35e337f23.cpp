Write a C++ function `eraseEveryOther` that takes a reference to a `std::vector<int>` and modifies it in-place so that it contains only the elements that were originally at even indices (0-based) of the input vector, preserving their relative order. For example, given `{90, 34, 56, 32, 89}`, the function should leave the vector as `{90, 56, 89}` (original indices 0, 2, 4). The function should handle empty vectors, vectors with one element, and any vector length. Do not use a second vector or any extra storage; only modify the given vector. You may use `erase` or a two-pointer overwrite method. The function must be const-correct where appropriate (i.e., the vector parameter is non-const because it is modified), but the function itself should not be `const`.

#include <cassert>
#include <vector>

// Function declaration from solution (not included here to avoid duplication)
void eraseEveryOther(std::vector<int>& v);

int main() {
    // Test 1: Original example
    std::vector<int> v1 = {90, 34, 56, 32, 89};
    eraseEveryOther(v1);
    assert((v1 == std::vector<int>{90, 56, 89}));

    // Test 2: Empty vector
    std::vector<int> v2;
    eraseEveryOther(v2);
    assert(v2.empty());

    // Test 3: Single element
    std::vector<int> v3 = {42};
    eraseEveryOther(v3);
    assert((v3 == std::vector<int>{42}));

    // Test 4: Even length
    std::vector<int> v4 = {1, 2, 3, 4};
    eraseEveryOther(v4);
    assert((v4 == std::vector<int>{1, 3}));

    // Test 5: All even indices are zero or negative numbers
    std::vector<int> v5 = {0, -1, -2, -3, -4};
    eraseEveryOther(v5);
    assert((v5 == std::vector<int>{0, -2, -4}));

    // Test 6: Large vector, only odd-sized
    std::vector<int> v6 = {5, 10, 15, 20, 25, 30, 35};
    eraseEveryOther(v6);
    assert((v6 == std::vector<int>{5, 15, 25, 35}));

    // Test 7: Duplicate values
    std::vector<int> v7 = {7, 7, 7, 7, 7};
    eraseEveryOther(v7);
    assert((v7 == std::vector<int>{7, 7, 7}));

    // Test 8: Two elements
    std::vector<int> v8 = {100, 200};
    eraseEveryOther(v8);
    assert((v8 == std::vector<int>{100}));

    // Test 9: Three elements
    std::vector<int> v9 = {1, 2, 3};
    eraseEveryOther(v9);
    assert((v9 == std::vector<int>{1, 3}));

    // Test 10: Six elements
    std::vector<int> v10 = {0, 1, 2, 3, 4, 5};
    eraseEveryOther(v10);
    assert((v10 == std::vector<int>{0, 2, 4}));

    return 0;
}

#include <vector>

// Modify the vector in-place so that only elements originally at even indices remain.
void eraseEveryOther(std::vector<int>& v) {
    size_t write = 0;
    for (size_t read = 0; read < v.size(); ++read) {
        if (read % 2 == 0) {
            v[write] = v[read];
            ++write;
        }
    }
    v.resize(write);
}

// The main algorithm is straightforward: we need to remove all elements at odd indices from the vector. Since erasing elements one by one with `erase` is inefficient (O(n²) in the worst case due to shifting), the optimal approach uses a two-pointer overwrite technique: maintain a write index `w` starting at 0, and iterate over the vector with a read index `r` from 0 to `size-1`. For each `r`, if `r` is even, copy `v[r]` to `v[w]` and increment `w`. After the loop, resize the vector to `w`. This yields O(n) time and O(1) auxiliary space. Edge cases: empty vector (w stays 0, resize to 0), single element (r=0 even, copies it, w=1, resize to 1), and vectors with odd length – the last element with index even is kept. Also, note that overwriting in-place is safe because `w <= r` always (since we only write on even indices, and we skip odd indices). The final `resize` truncates the tail. This avoids any reallocation or extra memory.
