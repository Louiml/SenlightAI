/*
Write a C++ function that reads an integer `n` (greater than 0), then reads exactly `n` integers into a vector, reads a scalar multiplier `k`, and returns a new vector where each element is the corresponding input element multiplied by `k`. The function should handle any `n ≥ 1`, and should not modify the original input vector. The returned vector must be the same size as the input vector. The function should be named `multiplyVectorByK` and must be const-correct, taking the input vector by const reference and returning a new vector by value. The input reading and output printing should not be part of the function; the function should work purely on already-provided data.
*/
#include <vector>

// Return a new vector where each element is the input element multiplied by k.
std::vector<int> multiplyVectorByK(const std::vector<int>& input, int k) {
    std::vector<int> result(input.size());
    for (std::size_t i = 0; i < input.size(); ++i) {
        result[i] = input[i] * k;
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Basic case
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> r1 = multiplyVectorByK(v1, 3);
    assert(r1.size() == 4);
    assert(r1[0] == 3 && r1[1] == 6 && r1[2] == 9 && r1[3] == 12);

    // Negative multiplier
    std::vector<int> v2 = {5, -7, 0};
    std::vector<int> r2 = multiplyVectorByK(v2, -2);
    assert(r2.size() == 3);
    assert(r2[0] == -10 && r2[1] == 14 && r2[2] == 0);

    // Multiplier of 0
    std::vector<int> v3 = {4, 8, 15, 16, 23, 42};
    std::vector<int> r3 = multiplyVectorByK(v3, 0);
    for (std::size_t i = 0; i < r3.size(); ++i) {
        assert(r3[i] == 0);
    }

    // Single element
    std::vector<int> v4 = {9};
    std::vector<int> r4 = multiplyVectorByK(v4, 1);
    assert(r4.size() == 1 && r4[0] == 9);

    // Multiplier of 1 (copy)
    std::vector<int> v5 = {1, 2, 3};
    std::vector<int> r5 = multiplyVectorByK(v5, 1);
    assert(r5 == v5); // Direct equality works for vectors

    // Input not modified (const reference)
    std::vector<int> v6 = {2, 4, 6};
    std::vector<int> original = v6;
    std::vector<int> r6 = multiplyVectorByK(v6, 2);
    assert(v6 == original); // Original unchanged
    assert(r6[0] == 4 && r6[1] == 8 && r6[2] == 12);

    // Large multiplier
    std::vector<int> v7 = {-1, 1};
    std::vector<int> r7 = multiplyVectorByK(v7, 1000);
    assert(r7[0] == -1000 && r7[1] == 1000);

    // Zero-size vector (edge case, though n>=1 in task)
    std::vector<int> v8 = {};
    std::vector<int> r8 = multiplyVectorByK(v8, 5);
    assert(r8.empty());

    // Multiplier with large positive values
    std::vector<int> v9 = {3, 7, 2};
    std::vector<int> r9 = multiplyVectorByK(v9, 4);
    assert(r9[0] == 12 && r9[1] == 28 && r9[2] == 8);

    return 0;
}
// The solution uses a simple loop over the input vector elements. For each index `i` from 0 to `size-1`, compute `input[i] * k` and store the result in a new vector of the same size. This avoids issues with 1-based indexing present in the original snippet (which used arrays starting at index 1, causing out-of-bounds access at index 0). Edge cases: an empty vector should not be passed (since `n ≥ 1` per problem), but the function still handles size 0 gracefully by returning an empty vector. Time complexity is O(n) because we iterate through all elements once. Space complexity is O(n) for the output vector, plus O(n) extra space for the input vector (which already exists). The function does not handle reading from standard input; that is left to the caller.
