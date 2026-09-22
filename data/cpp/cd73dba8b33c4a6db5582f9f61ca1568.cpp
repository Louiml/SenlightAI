/*
Write a C++ function named `rotateLeft` that takes a reference to a `std::vector<int>` and a non-negative integer `d` (the number of positions to rotate). The function should perform a left rotation on the vector in-place: each element at index `i` moves to index `(i - d + n) % n`, where `n` is the vector's size. The function must handle the case where `d` is larger than the vector's size by effectively rotating only `d % n` positions (i.e., if `d` equals `n` or a multiple of `n`, the vector remains unchanged). The function should not return a value or print anything; it must modify the input vector directly. Assume the vector is non-empty for simplicity, but the code should still work correctly if the vector is empty (in which case nothing is done). The solution must avoid using `std::rotate` or any other standard library rotation algorithm; you must implement the rotation using a temporary storage or the reversal method. Also, ensure the function is declared with `const`-correctness where appropriate (i.e., the function itself does not modify constants but takes a non-const reference).
*/

#include <vector>
#include <algorithm> // for std::reverse

// Reverses a subrange of the vector in place.
void reverseSegment(std::vector<int>& arr, int start, int end) {
    while (start < end) {
        std::swap(arr[start], arr[end]);
        ++start;
        --end;
    }
}

// Left-rotates the vector by d positions (d may exceed size).
void rotateLeft(std::vector<int>& arr, int d) {
    const int n = static_cast<int>(arr.size());
    if (n == 0) return;
    d = d % n; // normalize d to [0, n-1]
    if (d == 0) return;

    reverseSegment(arr, 0, n - 1);      // reverse entire vector
    reverseSegment(arr, 0, n - d - 1);  // reverse first n-d elements
    reverseSegment(arr, n - d, n - 1);  // reverse last d elements
}

#include <cassert>
#include <vector>

// Include the solution code here (rotateLeft and reverseSegment)

int main() {
    // Basic rotation
    std::vector<int> a = {1, 2, 3, 4, 5};
    rotateLeft(a, 2);
    assert((a == std::vector<int>{3, 4, 5, 1, 2}));

    // d = 0 -> unchanged
    std::vector<int> b = {10, 20, 30};
    rotateLeft(b, 0);
    assert((b == std::vector<int>{10, 20, 30}));

    // d equals size -> unchanged
    std::vector<int> c = {7, 8, 9};
    rotateLeft(c, 3);
    assert((c == std::vector<int>{7, 8, 9}));

    // d greater than size
    std::vector<int> d = {1, 2, 3, 4};
    rotateLeft(d, 5); // equivalent to 1
    assert((d == std::vector<int>{2, 3, 4, 1}));

    // Single element
    std::vector<int> e = {42};
    rotateLeft(e, 10);
    assert((e == std::vector<int>{42}));

    // Empty vector
    std::vector<int> f;
    rotateLeft(f, 3);
    assert(f.empty());

    // Larger vector with d multiple of n
    std::vector<int> g = {1, 2, 3, 4, 5, 6};
    rotateLeft(g, 12); // 12 % 6 = 0
    assert((g == std::vector<int>{1, 2, 3, 4, 5, 6}));

    // Rotate by n-1 (one to the right)
    std::vector<int> h = {1, 2, 3, 4};
    rotateLeft(h, 3);
    assert((h == std::vector<int>{4, 1, 2, 3}));

    // Rotate by 1
    std::vector<int> i = {9, 8, 7};
    rotateLeft(i, 1);
    assert((i == std::vector<int>{8, 7, 9}));

    return 0;
}

// A straightforward approach is to use a temporary vector to store the first `d % n` elements, then shift the remaining elements left by `d % n` positions, and finally copy the temporary elements back to the end. However, this uses \(O(d)\) auxiliary space. A more memory-efficient method uses array reversal: first reverse the entire vector, then reverse the first `n - d` elements, then reverse the last `d` elements (with `d` reduced modulo `n`). This achieves the same result in \(O(n)\) time and \(O(1)\) auxiliary space. Edge cases: if the vector is empty, do nothing. If `d` is zero or a multiple of `n`, the vector remains unchanged. The time complexity is \(O(n)\) for both methods; the reversal method has better space complexity. For clarity and robustness, I'll implement the reversal method. I'll include a helper `reverse` function that swaps elements using `std::swap`.
