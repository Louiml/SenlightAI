Write a C++ function `StackSimulator` that simulates a fixed-capacity stack using a dynamic array (or `std::vector`) internally, but exposes only four operations: `push`, `pop`, `top`, and `isEmpty`. The function should take a vector of integers representing a sequence of commands encoded as integers: positive numbers (including 0?) are pushed onto the stack; a special sentinel value `-1` means "pop" (and if pop is called on an empty stack, record a failure); `-2` means "return the current top without removing it" (if empty, record failure); `-3` means "check if empty" (record 1 if empty, 0 otherwise). The function should return a `std::vector<int>` containing the results of the `-2` and `-3` operations in the order they occur, plus a trailing `-99` if any push/pop operation failed (i.e., push when full, or pop when empty). Capacity is fixed at 5. The input vector is non-empty and only contains integers between -3 and 100 (inclusive), where positive numbers (including 0) are pushes. The function must be const-correct for the input (accept by const reference) and must not modify the input. Edge cases: pushing when the stack is full should not modify the stack, and popping when empty should not modify the stack. The function should be self-contained, include necessary headers, and not rely on global variables.
The solution maintains an internal `std::vector<int>` as the stack storage, with a size counter (or use `std::vector::size()`). Process each command sequentially: if the command is >= 0, push it only if size < 5; if push fails, record the failure flag. If command == -1, pop only if size > 0; if pop fails, record the failure flag. If command == -2, if size > 0, record `top` value; else record failure flag. If command == -3, record `size == 0 ? 1 : 0`. After processing all commands, if any failure occurred (push or pop on full/empty), append `-99` to the result vector. Important edge cases: multiple failures should still only produce one `-99` at the end; pushes and pops should be validated before modifying state; the input vector may contain large positive numbers, but capacity is fixed at 5. Time complexity: O(n) where n is the number of commands, because each command does O(1) work. Space complexity: O(1) auxiliary space for the stack storage (max 5 elements) plus the output vector, which is O(m) where m is the number of results.
#include <vector>

// Simulate a fixed-capacity stack (max 5) based on command sequence.
// Commands: >=0 push, -1 pop, -2 top, -3 isEmpty.
// Returns results of top/isEmpty commands, plus -99 if any push/pop failed.
std::vector<int> StackSimulator(const std::vector<int>& commands) {
    std::vector<int> stack;
    stack.reserve(5);
    std::vector<int> results;
    bool failure = false;

    for (int cmd : commands) {
        if (cmd >= 0) {
            if (stack.size() < 5) {
                stack.push_back(cmd);
            } else {
                failure = true;
            }
        } else if (cmd == -1) {
            if (!stack.empty()) {
                stack.pop_back();
            } else {
                failure = true;
            }
        } else if (cmd == -2) {
            if (!stack.empty()) {
                results.push_back(stack.back());
            } else {
                results.push_back(-11); // sentinel for top on empty
            }
        } else if (cmd == -3) {
            results.push_back(stack.empty() ? 1 : 0);
        }
    }

    if (failure) {
        results.push_back(-99);
    }
    return results;
}
#include <cassert>
#include <vector>

// The solution function is expected to be defined above in the same translation unit.

int main() {
    // Basic push and top
    std::vector<int> c1 = {5, -2, -3};
    std::vector<int> r1 = StackSimulator(c1);
    assert(r1.size() == 2 && r1[0] == 5 && r1[1] == 0);

    // Pop on empty triggers failure
    std::vector<int> c2 = {-1, -3};
    std::vector<int> r2 = StackSimulator(c2);
    assert(r2.size() == 2 && r2[0] == 1 && r2[1] == -99);

    // Push beyond capacity triggers failure
    std::vector<int> c3 = {1, 2, 3, 4, 5, 6, -3};
    std::vector<int> r3 = StackSimulator(c3);
    assert(r3.size() == 2 && r3[0] == 0 && r3[1] == -99);

    // Top on empty returns sentinel -11 but no failure
    std::vector<int> c4 = {-2, -3};
    std::vector<int> r4 = StackSimulator(c4);
    assert(r4.size() == 2 && r4[0] == -11 && r4[1] == 1);

    // Mixed operations with pushes/pops/top/empty checks
    std::vector<int> c5 = {10, 20, -2, -1, -2, -3};
    std::vector<int> r5 = StackSimulator(c5);
    assert(r5.size() == 4 && r5[0] == 20 && r5[1] == 10 && r5[2] == 0 && r5[3] == -99);

    // Multiple top calls and isEmpty
    std::vector<int> c6 = {7, -3, -3, -2, -2};
    std::vector<int> r6 = StackSimulator(c6);
    assert(r6.size() == 5 && r6[0] == 0 && r6[1] == 0 && r6[2] == 7 && r6[3] == 7);

    // Zero is a valid push value
    std::vector<int> c7 = {0, -2, -3};
    std::vector<int> r7 = StackSimulator(c7);
    assert(r7.size() == 2 && r7[0] == 0 && r7[1] == 0);

    // Pop till empty without failure, then top on empty
    std::vector<int> c8 = {1, -1, -2, -3};
    std::vector<int> r8 = StackSimulator(c8);
    assert(r8.size() == 3 && r8[0] == -11 && r8[1] == 1 && r8[2] == -99);

    // No commands except empty-check
    std::vector<int> c9 = {-3};
    std::vector<int> r9 = StackSimulator(c9);
    assert(r9.size() == 1 && r9[0] == 1);

    // Push exactly to capacity then top and tok
    std::vector<int> c10 = {1,2,3,4,5,-2,-1,-2,-3};
    std::vector<int> r10 = StackSimulator(c10);
    assert(r10.size() == 4 && r10[0] == 5 && r10[1] == 4 && r10[2] == 0 && r10[3] == -99);

    return 0;
}
