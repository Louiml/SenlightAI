Write a C++ function `sumSlice(const int arr[], int start, int count)` that returns the sum of `count` consecutive elements from the array `arr`, beginning at index `start`. The function must work for any valid subrange (including the whole array or a single element) and must not modify the array. Assume the caller guarantees `0 <= start` and `count >= 0`, and that `start + count` does not exceed the array length. Your function should be reusable in other programs; it must not print anything and must be `const`‑correct. Provide the function signature and implementation only, with no `main` function in the solution itself.
// The solution uses a simple loop that iterates exactly `count` times, starting from index `start` and adding each element to an accumulator. Because the input array is passed as a pointer to `const int`, the data is protected against modification, satisfying const‑correctness. Edge cases: if `count == 0`, the loop does not run and the function returns 0; if `count == 1`, it returns `arr[start]`; if the slice covers the entire array (start = 0, count = array length), it sums all elements. No special handling is needed for negative indices or overflow (the task assumes valid input). Time complexity is O(count) because we visit each element in the slice exactly once. Space complexity is O(1) because we only use a single integer accumulator.
// Returns the sum of `count` elements from `arr` starting at index `start`.
// Precondition: start >= 0, count >= 0, and start + count <= array length.
int sumSlice(const int arr[], int start, int count) {
    int total = 0;
    for (int i = 0; i < count; ++i) {
        total += arr[start + i];
    }
    return total;
}
int main() {
    int cookies[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    // Whole array
    assert(sumSlice(cookies, 0, 8) == 255);
    // First three
    assert(sumSlice(cookies, 0, 3) == 7);
    // Last four
    assert(sumSlice(cookies, 4, 4) == 240);
    // Single element
    assert(sumSlice(cookies, 2, 1) == 4);
    // Zero-length slice
    assert(sumSlice(cookies, 5, 0) == 0);
    // Middle slice: indices 2..5 => 4+8+16+32 = 60
    assert(sumSlice(cookies, 2, 4) == 60);
    // Negative numbers
    int negatives[3] = {-5, -10, -3};
    assert(sumSlice(negatives, 0, 3) == -18);
    assert(sumSlice(negatives, 1, 2) == -13);

    // Const correctness: passing a const array should work
    const int constArr[2] = {7, 9};
    assert(sumSlice(constArr, 0, 2) == 16);
}
