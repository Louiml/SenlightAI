Write a C++ function named `reverseVectorInPlace` that takes a non-const reference to a `std::vector<char>` and reverses its elements in place. The function must not allocate additional storage for character data (only constant extra space for indices/temporaries is allowed) and must perform the reversal using a single recursive helper function that processes the vector from both ends toward the middle. The function should correctly handle vectors of any size, including empty vectors (size 0) and vectors with a single element, and must not rely on library algorithms like `std::reverse`. The function should be free (not a member of a class), and its signature should be: `void reverseVectorInPlace(std::vector<char>& vec);`. The implementation must include a recursive helper with parameters for the current left and right indices, and the recursion must stop when the left index is greater than or equal to the right index. For valid non-empty vectors, after calling the function, the order of elements must be exactly reversed (e.g., `{'a','b','c'}` becomes `{'c','b','a'}`).

#include <cassert>
#include <vector>

void reverseHelper(std::vector<char>& vec, int left, int right) {
    if (left >= right) {
        return;
    }
    char temp = vec[left];
    vec[left] = vec[right];
    vec[right] = temp;
    reverseHelper(vec, left + 1, right - 1);
}

void reverseVectorInPlace(std::vector<char>& vec) {
    reverseHelper(vec, 0, static_cast<int>(vec.size()) - 1);
}

int main() {
    std::vector<char> v1 = {'a', 'b', 'c', 'd'};
    reverseVectorInPlace(v1);
    assert(v1 == std::vector<char>({'d', 'c', 'b', 'a'}));

    std::vector<char> v2 = {'x', 'y', 'z'};
    reverseVectorInPlace(v2);
    assert(v2 == std::vector<char>({'z', 'y', 'x'}));

    std::vector<char> v3 = {'s'};
    reverseVectorInPlace(v3);
    assert(v3 == std::vector<char>({'s'}));

    std::vector<char> v4;
    reverseVectorInPlace(v4);
    assert(v4.empty());

    std::vector<char> v5 = {'1', '2', '3', '4', '5', '6'};
    reverseVectorInPlace(v5);
    assert(v5 == std::vector<char>({'6', '5', '4', '3', '2', '1'}));

    std::vector<char> v6 = {'a', 'b'};
    reverseVectorInPlace(v6);
    assert(v6 == std::vector<char>({'b', 'a'}));

    std::vector<char> v7 = {'p', 'q', 'r', 's', 't'};
    reverseVectorInPlace(v7);
    assert(v7 == std::vector<char>({'t', 's', 'r', 'q', 'p'}));

    std::vector<char> v8 = {'m', 'n'};
    reverseVectorInPlace(v8);
    assert(v8 == std::vector<char>({'n', 'm'}));

    std::vector<char> v9 = {'a', 'a', 'b', 'b'};
    reverseVectorInPlace(v9);
    assert(v9 == std::vector<char>({'b', 'b', 'a', 'a'}));

    std::vector<char> v10 = {'z'};
    reverseVectorInPlace(v10);
    assert(v10 == std::vector<char>({'z'}));
}

#include <vector>

// Recursive helper that swaps elements from the ends toward the center.
void reverseHelper(std::vector<char>& vec, int left, int right) {
    if (left >= right) {
        return;
    }
    char temp = vec[left];
    vec[left] = vec[right];
    vec[right] = temp;
    reverseHelper(vec, left + 1, right - 1);
}

// Reverses the order of elements in the given vector in place.
void reverseVectorInPlace(std::vector<char>& vec) {
    reverseHelper(vec, 0, static_cast<int>(vec.size()) - 1);
}

// The core idea is to recursively swap the elements at the two ends of the vector and then move inward. We define a helper function `reverseHelper` that takes the vector, a left index, and a right index. The base case is when `left >= right`, meaning either we have crossed the middle (odd-length vector) or we have processed all pairs (even-length vector), and there is nothing left to do. In the recursive step, we swap `vec[left]` and `vec[right]`, then recursively call `reverseHelper` with `left+1` and `right-1`. This ensures we process each pair exactly once. For an empty vector, we can simply return immediately because the initial call with `left=0` and `right=-1` immediately satisfies `left >= right`. For a single-element vector, `left=0` and `right=0`, which also satisfies the base case. Edge cases include vectors with an even number of elements (the recursion stops exactly when `left > right`) and odd numbers (the middle element remains in place). The time complexity is O(n) where n is the number of elements, because we perform one swap per two elements, i.e., about n/2 swaps and n/2 recursive calls. The space complexity is O(n) in the worst case due to the recursion stack depth (which is about n/2), but we use no additional data structures for the vector contents; only constant auxiliary space for indices and the temporary variable used in swap.
