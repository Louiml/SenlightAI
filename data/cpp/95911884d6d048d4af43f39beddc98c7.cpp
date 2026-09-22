Write a C++ function named `pointerValueAfterOperations` that simulates the pointer behavior shown in the provided code snippet. The function should accept an integer parameter `initialValue` and return a string describing the final values of two dynamically allocated integer pointers after performing the exact same sequence of operations as in the snippet: allocate `p` and set it to `initialValue`; then have `q` point to the same memory as `p` and set its value to 8; then reassign `q` to a new allocation and set that to 9. The function must return a string of the form `"p: 8, q: 9"` (with the actual integer values). Ensure proper memory management by deleting all heap allocations before returning. The function should not print anything; it must only return the formatted string using `std::ostringstream` or `std::to_string`.
// The core idea is to mirror the pointer semantics from the snippet exactly. Start by allocating a new integer on the heap and assigning its address to pointer `p`, then set its value to the `initialValue` parameter. Next, assign `q = p` so both pointers share the same heap block. Then set `*q = 8`, which mutates the value seen through `p` as well, so `*p` becomes 8. After that, allocate a new integer for `q` (so `q` now points to a second heap block) and set `*q = 9`. At this point, `*p` remains 8 and `*q` is 9. Before returning, we must delete the memory for `q` (the second allocation) and then delete `p` (the first allocation) to avoid leaks. Note that we must not delete `q` before reassigning it, because that would delete the memory that `p` still points to. The function returns a string built from the values, e.g., using `std::ostringstream` to format `"p: 8, q: 9"`. Edge cases: input value could be any integer (positive, negative, zero) but it is overwritten by 8, so it only affects the initial value of `*p` before being overwritten. Time complexity is O(1) and space complexity is O(1) aside from the returned string.
#include <string>
#include <sstream>

// Simulates pointer operations from the snippet and returns a formatted string.
// Accepts an initial value that is overwritten to 8 in the first heap block.
// Returns "p: 8, q: 9" where the numbers are the final values.
std::string pointerValueAfterOperations(int initialValue) {
    int* p = new int;
    *p = initialValue;
    int* q = p; // q points to same memory as p
    *q = 8;     // overwrite to 8

    q = new int; // q now points to a new heap block
    *q = 9;      // set that block to 9

    // Capture the final values before freeing memory
    int pValue = *p;
    int qValue = *q;

    // Free both allocations. q's memory is freed first, then p's.
    delete q;
    delete p;

    std::ostringstream out;
    out << "p: " << pValue << ", q: " << qValue;
    return out.str();
}
#include <cassert>
#include <string>

// The solution function is declared here (or included from above).
// Assume the function is defined above this main in a real compilation unit.

int main() {
    assert(pointerValueAfterOperations(0) == "p: 8, q: 9");
    assert(pointerValueAfterOperations(42) == "p: 8, q: 9");
    assert(pointerValueAfterOperations(-5) == "p: 8, q: 9");
    assert(pointerValueAfterOperations(100) == "p: 8, q: 9");
    assert(pointerValueAfterOperations(1) == "p: 8, q: 9");
    assert(pointerValueAfterOperations(123456789) == "p: 8, q: 9");
    return 0;
}
