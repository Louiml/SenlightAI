// Write a C++ function that simulates the core logic of a Towers of Hanoi game using two stacks (source and destination) with a fixed capacity. The function should take as input: the number of disks `n`, a source stack represented as an array of integers (where index 0 is the bottom and index n-1 is the top), and a destination stack also as an array of integers. The function must move a single disk from the top of the source stack to the top of the destination stack, but only if the move is valid: (1) the source stack must not be empty, (2) the destination stack must not be full, and (3) if the destination stack is non-empty, the disk being moved (the top of the source) must be smaller than the current top of the destination. If the move is valid, the function should update both arrays (source top becomes 0 meaning empty slot, destination top becomes the moved disk value) and return `true`. If invalid, it should leave both stacks unchanged and return `false`. Assume all disk sizes are positive integers, and empty stack slots are represented by 0. The function signature should be: `bool moveDisk(int* source, int& sourceTop, int* dest, int& destTop, int capacity)`, where `sourceTop` and `destTop` are indices of the top elements (0 = empty stack, capacity-1 = full). The function must handle edge cases like empty source, full destination, and invalid disk ordering.

The solution requires a stack-like array representation where we track the index of the topmost element. An empty stack has `top = 0` (since index 0 would be the bottom, and if no elements, top index is 0 meaning "no element at index 0"? Actually, we define: if stack is empty, `top = 0` and the array is all zeros; the first element is at index `capacity-1` if we push from bottom? Let's clarify: To make it simple, we treat the array as having the bottom at index 0 and top at index `capacity-1`? That's counterintuitive. Instead, we'll use a conventional stack: array[0] is bottom, array[topIndex] is top, where topIndex starts at -1 for empty. So in the function, we use `sourceTop` and `destTop` as indices of the top element, with -1 meaning empty. The capacity is the maximum number of disks. The function checks: if sourceTop < 0 -> false (empty source). If destTop >= capacity-1 -> false (full destination). Then get disk = source[sourceTop]. If destTop >= 0 and disk >= dest[destTop] -> false (invalid order, since we require smaller disk on top of larger). Otherwise, perform move: dest[destTop+1] = disk; source[sourceTop] = 0 (empty); decrement sourceTop; increment destTop; return true. Edge cases: source has exactly one disk, destination empty, destination has same size (should be invalid), negative tops, etc. Time complexity O(1), space O(1).

#include <cstddef> // for size_t

// Move one disk from source stack to destination stack.
// Returns true if valid move performed, false otherwise.
// source and dest are arrays of size 'capacity', using 0 for empty slots.
// sourceTop and destTop are indices of the topmost element (-1 if empty).
bool moveDisk(int* source, int& sourceTop, int* dest, int& destTop, int capacity) {
    // Source empty?
    if (sourceTop < 0) return false;
    // Destination full?
    if (destTop >= capacity - 1) return false;

    int disk = source[sourceTop];
    // Check disk ordering: destination top (if any) must be larger than disk.
    if (destTop >= 0 && disk >= dest[destTop]) return false;

    // Perform the move.
    dest[destTop + 1] = disk;
    source[sourceTop] = 0; // clear the slot
    --sourceTop;
    ++destTop;
    return true;
}

#include <cassert>

int main() {
    const int cap = 3;
    int src[cap] = {0, 0, 1}; // bottom to top? Actually index 0 is bottom, index 2 is top. So stack has disk 1 on top.
    int srcTop = 2; // top index is 2
    int dst[cap] = {0, 0, 0};
    int dstTop = -1; // empty

    // Move disk 1 to empty dest -> valid
    assert(moveDisk(src, srcTop, dst, dstTop, cap) == true);
    assert(srcTop == 1);
    assert(dstTop == 0);
    assert(dst[0] == 1);

    // Now dest has disk 1, source empty? source has 0s and srcTop=1 meaning no disk? Actually srcTop=1 means index1 is top, but src[1]=0 (empty). So source is empty.
    assert(moveDisk(src, srcTop, dst, dstTop, cap) == false); // empty source

    // Setup a new scenario: source has disk 3, dest has disk 1 (top). Moving 3 onto 1 invalid.
    int src2[cap] = {0, 0, 3};
    int src2Top = 2;
    int dst2[cap] = {1, 0, 0};
    int dst2Top = 0; // top is index0 with disk 1
    assert(moveDisk(src2, src2Top, dst2, dst2Top, cap) == false); // 3>=1

    // Moving 2 onto 1 is valid
    int src3[cap] = {0, 0, 2};
    int src3Top = 2;
    int dst3[cap] = {1, 0, 0};
    int dst3Top = 0;
    assert(moveDisk(src3, src3Top, dst3, dst3Top, cap) == true);
    assert(dst3[1] == 2);
    assert(dst3Top == 1);
    assert(src3Top == 1);

    // Full destination check
    int src4[cap] = {0, 0, 1};
    int src4Top = 2;
    int dst4[cap] = {3, 2, 1}; // full stack
    int dst4Top = 2;
    assert(moveDisk(src4, src4Top, dst4, dst4Top, cap) == false); // dest full

    // Edge: equal disks invalid
    int src5[cap] = {0, 0, 5};
    int src5Top = 2;
    int dst5[cap] = {5, 0, 0};
    int dst5Top = 0;
    assert(moveDisk(src5, src5Top, dst5, dst5Top, cap) == false); // equal

    return 0;
}
