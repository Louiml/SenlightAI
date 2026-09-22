// Write a C++ function `bool safeToProduce(int mutex, int empty, int full)` that implements the mutex-and-semaphore check for the producer step in the classic bounded-buffer Producer–Consumer problem. The function should return `true` if and only if the mutex is available (`mutex == 1`) and there is at least one empty slot (`empty > 0`), regardless of the `full` parameter. The function must be `const`-correct and take parameters by value. This is a pure Boolean predicate with no side effects; it should not simulate the state changes of the actual producer/consumer operations. The task is to extract and generalize the condition used in the original code’s `case 1` branch.

int main() {
    // Test true cases: mutex free and at least one empty slot
    assert(safeToProduce(1, 1, 0) == true);
    assert(safeToProduce(1, 5, 0) == true);
    assert(safeToProduce(1, 10, 0) == true);
    // Test mutex not free
    assert(safeToProduce(0, 1, 0) == false);
    assert(safeToProduce(2, 1, 0) == false);
    assert(safeToProduce(-1, 1, 0) == false);
    // Test empty slot count zero or negative
    assert(safeToProduce(1, 0, 0) == false);
    assert(safeToProduce(1, -1, 0) == false);
    // Test full parameter is ignored
    assert(safeToProduce(1, 1, 100) == true);
    assert(safeToProduce(0, 5, 0) == false);
    return 0;
}

#include <cstddef> // for std::size_t if needed, but not necessary here

// Predicate: can a producer proceed given current semaphore states?
// Returns true only if mutex is free (==1) and there is at least one empty slot (>0).
bool safeToProduce(int mutex, int empty, int full) {
    (void)full; // explicitly unused parameter, suppress compiler warnings
    return (mutex == 1) && (empty > 0);
}

// The original code snippet models a producer–consumer with three global variables: mutex `s`, count of full slots `f`, and count of empty slots `empty`. The producer’s action is guarded by `if (s == 1 && empty != 0)`. The task isolates this guard into a reusable function that takes the current values of these three parameters. The algorithm is trivial: return `(mutex == 1) && (empty > 0)`. Edge cases include zero or negative `empty` values (both should fail), `mutex` values other than `1` (e.g., `0` when locked, or negative as a corruption) should fail, and the `full` parameter is ignored entirely because the producer only checks empty space and mutex, not how many items already exist. The function must not mutate inputs, so pass by value and mark the function `const`-qualified (though a free function cannot be `const`; use a `const` lambda or a static function? As a free function, we can simply not modify parameters). Time complexity is O(1), space complexity O(1).
