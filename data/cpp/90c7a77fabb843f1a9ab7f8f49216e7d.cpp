// Write a C++ function `template<typename T> void RemoveDuplicates(MyStack<T>& s)` that takes a stack implemented as a dynamic array (with fields `T* stack`, `int top`, `int maxSize`) and removes all duplicate elements from the stack, preserving the relative order of the first occurrences of each unique value (i.e., the bottom-most occurrence of each distinct value is kept, and any later occurrences above it are removed). The function must not use any standard library containers (e.g., `std::vector`, `std::set`, `std::unordered_set`) and must operate in-place on the stack's internal array. After removal, the stack's `top` must be updated to reflect the new size, and the remaining elements should be compacted toward the bottom (index 0). The function should handle empty stacks (do nothing), stacks with one element (do nothing), and stacks containing duplicate values at any positions. Do not modify `maxSize` or reallocate the underlying array. For custom types like `NewStruct`, define equality as equality of all member fields; the provided `NewStruct` has a single `int data` field, so `==` works by default. The function should be `const`-correct where applicable (i.e., it may read `maxSize` and `top` as needed but modifies `stack` and `top`). Do not include a `main` function in the solution; only provide the function definition and any necessary supporting code (e.g., the existing `MyStack` struct definition and `NewStruct` if referenced) in the solution block. The function must work for any `T` that supports `==` comparison.

The core challenge is to remove duplicates in-place without extra storage. A brute-force approach would compare each element from bottom to top against all previously kept elements, but that is O(n²) in time and O(1) in space. To improve time without violating the "no standard containers" rule, we can use an O(n²) algorithm that is simple to reason about: iterate from the bottom of the stack (index 0) upward, and for each element at index `i`, scan all earlier kept positions (0..i-1) to see if the same value already exists. If it does, mark the current position for removal; otherwise, it is unique. After scanning, compact the array by shifting kept elements down. Since the stack is a simple array, we can do this in a single pass with an auxiliary "write" index: for each element from bottom to top, if it is not a duplicate of any earlier kept element, copy it to the `write` position and increment `write`. At the end, set `top = write - 1`. This is O(n²) time in the worst case (when all elements are unique, each element scans all previous), and O(1) auxiliary space (only loop variables). Edge cases: empty stack (top = -1) and single-element stack (top = 0) are trivially handled because the loops do nothing or the single element is automatically kept. Duplicates of the same value appearing multiple times are all removed except the first occurrence. The function must be a template, and for `NewStruct`, the default `operator==` correctly compares `data`. The time complexity is O(n²) for uniqueness checks, and space is O(1) extra, making it acceptable for moderate stack sizes. If a stack is at full capacity (`top == maxSize - 1`), duplicates are removed but `maxSize` remains unchanged, so the stack's capacity is not reduced.

#include <iostream>

// Support struct for the stack template
struct NewStruct {
    int data;
    // Default operator== works because data is the only field
};

// The stack struct as given in the problem
template <typename T>
struct MyStack {
    T* stack;
    int top;
    int maxSize;
};

// Function to remove duplicates from the stack, preserving the first (bottom-most) occurrence.
// Works in-place, O(n^2) time, O(1) extra space.
template <typename T>
void RemoveDuplicates(MyStack<T>& s) {
    if (s.top < 0) return; // empty stack

    int writeIndex = 0; // where to place the next unique element

    for (int i = 0; i <= s.top; ++i) {
        bool isDuplicate = false;
        // Check if current element matches any previously kept element
        for (int j = 0; j < writeIndex; ++j) {
            if (s.stack[j] == s.stack[i]) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            // Keep this element by copying it to the write position
            if (writeIndex != i) {
                s.stack[writeIndex] = s.stack[i];
            }
            ++writeIndex;
        }
    }

    s.top = writeIndex - 1; // update the new top index
}

#include <cassert>

// Include the solution code above (or paste here) — for completeness, we repeat the required definitions.
// (In a real test, the solution would be included from the previous block.)
template <typename T>
struct MyStack {
    T* stack;
    int top;
    int maxSize;
};

template <typename T>
void Initialize(MyStack<T>& s, int maxSize) {
    s.stack = new T[maxSize];
    s.top = -1;
    s.maxSize = maxSize;
}

template <typename T>
void Push(MyStack<T>& s, T element) {
    if (s.top == s.maxSize - 1) {
        // Should not happen in tests; we assume enough capacity.
    } else {
        s.stack[++s.top] = element;
    }
}

template <typename T>
void RemoveDuplicates(MyStack<T>& s) {
    if (s.top < 0) return;
    int writeIndex = 0;
    for (int i = 0; i <= s.top; ++i) {
        bool isDuplicate = false;
        for (int j = 0; j < writeIndex; ++j) {
            if (s.stack[j] == s.stack[i]) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            if (writeIndex != i) {
                s.stack[writeIndex] = s.stack[i];
            }
            ++writeIndex;
        }
    }
    s.top = writeIndex - 1;
}

int main() {
    // Test 1: int stack with duplicates
    {
        MyStack<int> s;
        Initialize(s, 10);
        Push(s, 1); Push(s, 2); Push(s, 1); Push(s, 3); Push(s, 2); Push(s, 4);
        RemoveDuplicates(s);
        assert(s.top == 3); // 1,2,3,4 -> indices 0..3
        assert(s.stack[0] == 1);
        assert(s.stack[1] == 2);
        assert(s.stack[2] == 3);
        assert(s.stack[3] == 4);
        delete[] s.stack;
    }

    // Test 2: all duplicates
    {
        MyStack<int> s;
        Initialize(s, 5);
        Push(s, 7); Push(s, 7); Push(s, 7);
        RemoveDuplicates(s);
        assert(s.top == 0);
        assert(s.stack[0] == 7);
        delete[] s.stack;
    }

    // Test 3: empty stack
    {
        MyStack<int> s;
        Initialize(s, 5);
        RemoveDuplicates(s);
        assert(s.top == -1);
        delete[] s.stack;
    }

    // Test 4: single element
    {
        MyStack<int> s;
        Initialize(s, 5);
        Push(s, 42);
        RemoveDuplicates(s);
        assert(s.top == 0);
        assert(s.stack[0] == 42);
        delete[] s.stack;
    }

    // Test 5: no duplicates
    {
        MyStack<int> s;
        Initialize(s, 5);
        Push(s, 10); Push(s, 20); Push(s, 30);
        RemoveDuplicates(s);
        assert(s.top == 2);
        assert(s.stack[0] == 10);
        assert(s.stack[1] == 20);
        assert(s.stack[2] == 30);
        delete[] s.stack;
    }

    // Test 6: duplicates at non-consecutive positions
    {
        MyStack<int> s;
        Initialize(s, 10);
        Push(s, 5); Push(s, 3); Push(s, 5); Push(s, 7); Push(s, 3); Push(s, 9);
        RemoveDuplicates(s);
        assert(s.top == 3); // 5,3,7,9
        assert(s.stack[0] == 5);
        assert(s.stack[1] == 3);
        assert(s.stack[2] == 7);
        assert(s.stack[3] == 9);
        delete[] s.stack;
    }

    // Test 7: custom struct type
    {
        MyStack<NewStruct> s;
        Initialize(s, 10);
        NewStruct a{1}, b{2}, c{1}, d{3};
        Push(s, a); Push(s, b); Push(s, c); Push(s, d);
        RemoveDuplicates(s);
        assert(s.top == 2); // a,b,d
        assert(s.stack[0].data == 1);
        assert(s.stack[1].data == 2);
        assert(s.stack[2].data == 3);
        delete[] s.stack;
    }

    return 0;
}
