Write a C++ function named `reuseMemory` that takes no arguments and returns an integer representing the number of successful memory allocations performed. The function must simulate a custom memory pool: maintain a static linked list of freed nodes (each node having a single `next` pointer), and implement custom `operator new` and `operator delete` for a class named `PoolNode` so that when nodes are deleted, they are added to the freelist and when new nodes are requested, the freelist is reused first. Inside `reuseMemory`, perform the following cycle exactly: allocate 10 `PoolNode` objects into an array of 10 pointers, delete the first 5 of those pointers, allocate 5 new `PoolNode` objects (reusing the freelist), and finally delete all 10 pointers. The function must return the total number of times `operator new` was called (i.e., total allocations, counting each allocation individually, whether from freelist or `malloc`). Ensure that `operator delete` never calls the global `free`; it only links nodes onto the freelist. Handle the case where the freelist is empty by calling `malloc` directly. The class must have a default constructor that initializes its data member to 0.
// The solution defines a class `PoolNode` with a static `PoolNode* iFreeList` (initialized to `nullptr`), a `next` pointer for the freelist, and an integer data member. The custom `operator new` checks if `iFreeList` is non-null; if so, it removes the head node from the freelist and returns it. Otherwise, it calls `malloc` with the given size. The custom `operator delete` casts the pointer to `PoolNode*`, links the node onto the freelist by setting its `next` to the current head and updating `iFreeList` to that node. The `reuseMemory` function declares an array of 10 `PoolNode*` pointers. It first allocates 10 nodes using `new`, then deletes indices 0–4 (which adds 5 nodes to the freelist), then allocates 5 new nodes — these 5 will be served from the freelist (since it has exactly 5 nodes), so the second round uses no `malloc`. Finally, it deletes all 10 pointers. The total number of `operator new` calls is exactly 15 (10 + 5), because the second batch reuses the freelist; note that `delete` calls do not invoke `operator new`. Edge cases to consider: the freelist may have fewer nodes than requested, but here the counts match. The function must count every call to `new` inside the loop, so it increments a counter after each `new` expression. Time complexity is O(1) per operation, so O(15) overall. Space complexity is O(1) auxiliary (the freelist stores up to 5 nodes at peak, but that's part of the object memory).
#include <cstdlib>

class PoolNode {
public:
    PoolNode() : data(0), next(nullptr) {}

    void* operator new(size_t size) {
        if (iFreeList) {
            PoolNode* ptr = iFreeList;
            iFreeList = ptr->next;
            return ptr;
        }
        return std::malloc(size);
    }

    void operator delete(void* ptr) {
        PoolNode* node = static_cast<PoolNode*>(ptr);
        node->next = iFreeList;
        iFreeList = node;
    }

    int getData() const { return data; }

private:
    int data;
    PoolNode* next;
    static PoolNode* iFreeList;
};

PoolNode* PoolNode::iFreeList = nullptr;

// Simulate the allocation/deletion cycle and return total allocations.
int reuseMemory() {
    const int fullList = 10;
    const int halfList = 5;
    PoolNode* pool[fullList];
    int allocationCount = 0;

    for (int j = 0; j < fullList; ++j) {
        pool[j] = new PoolNode();
        ++allocationCount;
    }

    for (int j = 0; j < halfList; ++j) {
        delete pool[j];
    }

    for (int j = 0; j < halfList; ++j) {
        pool[j] = new PoolNode();
        ++allocationCount;
    }

    for (int j = 0; j < fullList; ++j) {
        delete pool[j];
    }

    return allocationCount;
}
#include <cassert>

// Declare the function prototype (if not included via header).
int reuseMemory();

int main() {
    // The freelist starts empty, so all 10 first allocations use malloc.
    // Then 5 deletes add 5 nodes to freelist.
    // The next 5 allocations reuse the freelist, so no new malloc calls.
    // Total allocations = 10 + 5 = 15.
    assert(reuseMemory() == 15);

    // Ensure the function can be called multiple times; each call starts fresh
    // because the freelist is static but the function clears it by using all
    // freed nodes in the second allocation phase. However, the freelist may
    // retain leftover nodes after the final deletes, but since we never call
    // free, the freelist persists. Calling again would reuse those leftover
    // nodes, but the allocation count is still 15 because each allocation
    // counts regardless of source. Test again for consistency.
    assert(reuseMemory() == 15);

    // Verify that the freelist allows reuse: after the first call, the
    // freelist contains 5 nodes (from the final deletes). In the second call,
    // the first 5 allocations reuse those, and the next 5 use new malloc.
    // Still 15 total allocations. No further assertion needed beyond count.

    // Additional check: the data member is zero-initialized in constructor.
    // We can't access private members here, but the constructor sets data=0.
    // This is implicitly tested by the class definition.

    return 0;
}
