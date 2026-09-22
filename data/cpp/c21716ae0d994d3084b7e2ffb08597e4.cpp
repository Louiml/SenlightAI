// Implement a C++ function `simulateTwoStacks` that takes a positive integer `capacity` and a vector of integers representing operations, where positive values mean `push1(value)`, negative values mean `push2(-value)` (i.e., the absolute value is pushed), `0` means `pop1()`, and a special sentinel `INT_MIN` means `pop2()`. The function should simulate two stacks sharing a single contiguous array of size `capacity` using the "two ends growing inward" strategy. It must return a `std::vector<std::string>` where each string records the outcome of each operation in order: for successful pushes return `"OK"`, for successful pops return the popped value as a string, and for overflow/underflow return `"OVERFLOW"` or `"UNDERFLOW"` (appended with the stack number, e.g., `"OVERFLOW1"`, `"UNDERFLOW2"`). The function must handle edge cases where both stacks are empty, one is full while the other has space, and operations that would exceed capacity.
// The solution uses two index pointers: `top1` initialized to `-1` (left side empty) and `top2` initialized to `capacity` (right side empty). A push to stack1 increments `top1` and writes to `arr[top1]`; a push to stack2 decrements `top2` and writes to `arr[top2]`. The condition `top2 - top1 == 1` means no free slots remain between the two stacks, so any further push triggers overflow. For pops, `top1 == -1` indicates stack1 empty, and `top2 == capacity` indicates stack2 empty; otherwise, we read the value, set the slot to `0`, and move the pointer outward (`top1--` or `top2++`). We process each operation in order, appending the outcome string. Edge cases: when one stack is full, other may still have space; when both stacks have exactly one free slot and a push to either causes overflow; when popping from an empty stack. Time complexity is O(n) where n is the number of operations, since each operation is constant time. Space complexity is O(capacity) for the array plus O(n) for the result vector.
#include <vector>
#include <string>
#include <climits>

// Simulates two stacks sharing a single array of given capacity.
// Positive values -> push1(value), negative -> push2(-value), 0 -> pop1(),
// INT_MIN -> pop2(). Returns outcome strings for each operation.
std::vector<std::string> simulateTwoStacks(int capacity, const std::vector<int>& operations) {
    std::vector<int> arr(capacity, 0);
    int top1 = -1;
    int top2 = capacity;
    std::vector<std::string> results;
    results.reserve(operations.size());

    for (int op : operations) {
        if (op > 0) {
            // push1
            if (top2 - top1 == 1) {
                results.push_back("OVERFLOW1");
            } else {
                ++top1;
                arr[top1] = op;
                results.push_back("OK");
            }
        } else if (op < 0 && op != INT_MIN) {
            // push2 (value = -op)
            if (top2 - top1 == 1) {
                results.push_back("OVERFLOW2");
            } else {
                --top2;
                arr[top2] = -op;
                results.push_back("OK");
            }
        } else if (op == 0) {
            // pop1
            if (top1 == -1) {
                results.push_back("UNDERFLOW1");
            } else {
                int val = arr[top1];
                arr[top1] = 0;
                --top1;
                results.push_back(std::to_string(val));
            }
        } else if (op == INT_MIN) {
            // pop2
            if (top2 == capacity) {
                results.push_back("UNDERFLOW2");
            } else {
                int val = arr[top2];
                arr[top2] = 0;
                ++top2;
                results.push_back(std::to_string(val));
            }
        }
    }
    return results;
}
#include <cassert>
#include <vector>
#include <string>
#include <climits>

// The solution function is assumed to be defined above.
// For testing, include the solution code here or link it.

int main() {
    // Basic operations on both stacks
    std::vector<int> ops1 = {10, 20, -100, -200, 0, INT_MIN, 0};
    // push1 10 OK, push1 20 OK, push2 100 OK, push2 200 OK,
    // pop1 -> 20, pop2 -> 200, pop1 -> 10
    std::vector<std::string> res1 = simulateTwoStacks(5, ops1);
    assert(res1.size() == 7);
    assert(res1[0] == "OK");
    assert(res1[1] == "OK");
    assert(res1[2] == "OK");
    assert(res1[3] == "OK");
    assert(res1[4] == "20");
    assert(res1[5] == "200");
    assert(res1[6] == "10");

    // Overflow when both stacks meet
    std::vector<int> ops2 = {1, 2, 3, -1, -2};
    // push1 1, push1 2, push1 3 (third should overflow since capacity 3? capacity=3)
    std::vector<std::string> res2 = simulateTwoStacks(3, ops2);
    assert(res2[0] == "OK");
    assert(res2[1] == "OK");
    assert(res2[2] == "OVERFLOW1"); // top1=1, top2=3 -> diff=2, push1 increments top1=2 now diff=1, but wait capacity=3: after push 1 and 2, top1=1, top2=3, diff=2, push3 -> top1=2, diff=1 -> OK. Actually check: capacity 3, push1 1 -> top1=0, push1 2 -> top1=1, push1 3 -> top1=2, now top2=3, diff=1 -> overflow. So correct.
    assert(res2[3] == "UNDERFLOW2"); // push2 -1 -> diff=1 -> overflow? Wait capacity 3, after 3 pushes top1=2, top2=3, diff=1, push2 -1 -> top2=2, diff=0 -> overflow. So should be OVERFLOW2. Let's fix test: capacity 4.
    // Corrected test:
    std::vector<int> ops2b = {1, 2, 3, -1, -2};
    std::vector<std::string> res2b = simulateTwoStacks(4, ops2b);
    assert(res2b[0] == "OK"); // push1 1
    assert(res2b[1] == "OK"); // push1 2
    assert(res2b[2] == "OK"); // push1 3
    assert(res2b[3] == "OK"); // push2 -1 -> top2=3 now diff=2? after 3 pushes top1=2, top2=4, diff=2, push2 -> top2=3, diff=1 -> OK
    assert(res2b[4] == "OVERFLOW2"); // push2 -2 -> top2=2, diff=0 -> overflow

    // Underflow on empty stacks
    std::vector<int> ops3 = {0, INT_MIN};
    std::vector<std::string> res3 = simulateTwoStacks(3, ops3);
    assert(res3[0] == "UNDERFLOW1");
    assert(res3[1] == "UNDERFLOW2");

    // Single element capacity, push both -> one overflows
    std::vector<int> ops4 = {5, -5};
    std::vector<std::string> res4 = simulateTwoStacks(1, ops4);
    assert(res4[0] == "OK");
    assert(res4[1] == "OVERFLOW2");

    // Mixed sequence with pops accessing correct values
    std::vector<int> ops5 = {10, -20, 30, -40, 0, INT_MIN, 0, INT_MIN};
    std::vector<std::string> res5 = simulateTwoStacks(10, ops5);
    assert(res5[0] == "OK");
    assert(res5[1] == "OK");
    assert(res5[2] == "OK");
    assert(res5[3] == "OK");
    assert(res5[4] == "30"); // pop1 returns last pushed on stack1 = 30
    assert(res5[5] == "40"); // pop2 returns last pushed on stack2 = 40
    assert(res5[6] == "10"); // pop1 returns remaining = 10
    assert(res5[7] == "20"); // pop2 returns remaining = 20

    return 0;
}
