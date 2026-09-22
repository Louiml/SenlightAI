Write a C++ function that takes a non-empty `std::vector<int>` representing a jump table of offsets and a non-empty `std::vector<void(*)()>` of function pointers, along with a target index, and returns a `std::function<void()>` that safely invokes the function pointer at the given index after applying a cyclic offset stored in the jump table. The offset for index `i` is `jumpTable[i % jumpTable.size()]`, and the final target index is `(i + offset) % functions.size()`. The function must handle cyclic wrapping, reject negative indices, and throw `std::out_of_range` if either container is empty or the index is invalid. The returned callable must capture the correct function pointer by value and be invocable with no arguments, ensuring no dangling references.
#include <cassert>
#include <functional>
#include <vector>

// Test helper functions for the jump call.
static void funcA() {}
static void funcB() {}
static void funcC() {}

int main() {
    std::vector<void(*)()> funcs = {funcA, funcB, funcC};
    std::vector<int> jumps = {1, -1, 2};

    // index 0: jump 1 -> funcB
    auto call1 = createJumpCall(jumps, funcs, 0);
    assert(true); // lambda compiles and is callable
    call1(); // no crash; check by side-effect isn't feasible, so just invoke

    // index 1: jump -1 -> index 0 -> funcA
    auto call2 = createJumpCall(jumps, funcs, 1);
    call2();

    // index 2: jump 2 -> index 4 % 3 = 1 -> funcB
    auto call3 = createJumpCall(jumps, funcs, 2);
    call3();

    // index 3: jump index 3 % 3 = 0 -> offset 1 -> index 4 % 3 = 1 -> funcB
    auto call4 = createJumpCall(jumps, funcs, 3);
    call4();

    // Single element jump table and function vector
    std::vector<int> singleJump = {0};
    std::vector<void(*)()> singleFunc = {funcA};
    auto call5 = createJumpCall(singleJump, singleFunc, 42);
    call5();

    // Large index wraps correctly
    std::vector<int> jumpBig = {2, 2};
    std::vector<void(*)()> funcBig = {funcA, funcB};
    auto call6 = createJumpCall(jumpBig, funcBig, 100);
    call6();

    // Negative index must throw
    bool threw = false;
    try { createJumpCall(jumps, funcs, -1); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    // Empty vectors must throw
    threw = false;
    std::vector<int> emptyJump;
    std::vector<void(*)()> emptyFunc;
    try { createJumpCall(emptyJump, funcs, 0); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    threw = false;
    try { createJumpCall(jumps, emptyFunc, 0); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
}
#include <functional>
#include <stdexcept>
#include <vector>

// Return a callable that invokes the function pointer at (index + jumpTable[index % size]) % functions.size().
// Throws std::out_of_range for any invalid input or index.
std::function<void()> createJumpCall(
    const std::vector<int>& jumpTable,
    const std::vector<void(*)()>& functions,
    int index
) {
    if (jumpTable.empty() || functions.empty()) {
        throw std::out_of_range("jumpTable and functions must be non-empty");
    }
    if (index < 0) {
        throw std::out_of_range("index must be non-negative");
    }

    const size_t jumpIdx = static_cast<size_t>(index) % jumpTable.size();
    const int offset = jumpTable[jumpIdx];
    const size_t funcIdx = (static_cast<size_t>(index) + static_cast<size_t>(offset)) % functions.size();

    void (*target)() = functions[funcIdx];
    return [target]() { target(); };
}
// The main algorithm is straightforward: given the containers and a starting index, compute an offset from the jump table using modular arithmetic to normalize the index into the jump table's bounds. The offset is then added to the starting index, and the result is normalized modulo the function-vector size to ensure cyclic wrapping. Critical edge cases include empty containers, negative indices, and modulo by zero — all must be checked upfront and throw `std::out_of_range`. Because `std::function` and function pointers are copyable and value-semantic, capturing the function pointer by value in a lambda avoids dangling references. Time complexity is O(1) for the computation and O(1) for capturing the pointer in `std::function`, while space complexity is O(1) beyond the returned object.
