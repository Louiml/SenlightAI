// Write a C++ function `flattenNestedList` that takes a vector of `NestedInteger` objects (the interface defined below) and returns a `std::vector<int>` containing all integers in the nested structure in depth-first, left-to-right order. The `NestedInteger` interface has three methods: `isInteger()` returns `true` if the element holds a single integer, `getInteger()` returns that integer (valid only if `isInteger()` is `true`), and `getList()` returns a reference to the vector of `NestedInteger` objects (valid only if `isInteger()` is `false`). The input may contain arbitrarily deep nesting, empty lists, and duplicate integers. The function must not modify the input. It should handle empty input gracefully by returning an empty vector. The solution must use an iterative approach with an explicit stack (no recursion) to avoid stack overflow on deeply nested inputs.
The key idea is to simulate the depth-first traversal of the nested list structure using a stack of `NestedInteger` objects, pushing elements in reverse order so that the leftmost element is processed first. Start by pushing all top-level elements of the input vector onto the stack in reverse order (from last to first). Then, while the stack is not empty, pop the top element. If it is an integer, append its value to the result vector. If it is a list, push all its contained elements onto the stack in reverse order as well (from the last element of that inner list to the first), so that the first element of that inner list will be processed next. This ensures a correct depth-first, left-to-right order. Important edge cases: empty top-level list (return empty vector), empty inner lists (they contribute nothing and are simply skipped), duplicate integers (handled naturally by pushing each element separately), and deeply nested structures (the iterative stack avoids recursion depth limits). The time complexity is O(n) where n is the total number of integers across all nested lists, because each integer is pushed and popped exactly once, and each list element is visited once. The space complexity is O(n) in the worst case, because the stack may hold up to all elements when the structure is flat or when the top-level list is very wide.
#include <vector>
#include <stack>

// Forward declaration of the interface (provided by the problem).
class NestedInteger {
public:
    bool isInteger() const;
    int getInteger() const;
    const std::vector<NestedInteger> &getList() const;
};

// Flatten a nested list of integers into a flat vector of integers.
// Uses an iterative stack approach to avoid recursion depth problems.
std::vector<int> flattenNestedList(const std::vector<NestedInteger>& nestedList) {
    std::vector<int> result;
    std::stack<NestedInteger> workStack;

    // Push top-level elements in reverse order so that first element is on top.
    for (int i = static_cast<int>(nestedList.size()) - 1; i >= 0; --i) {
        workStack.push(nestedList[i]);
    }

    while (!workStack.empty()) {
        NestedInteger current = workStack.top();
        workStack.pop();

        if (current.isInteger()) {
            result.push_back(current.getInteger());
        } else {
            const std::vector<NestedInteger>& innerList = current.getList();
            // Push inner elements in reverse order to preserve left-to-right order.
            for (int i = static_cast<int>(innerList.size()) - 1; i >= 0; --i) {
                workStack.push(innerList[i]);
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Minimal mock implementation of NestedInteger for testing purposes.
// This is just for the test; in a real problem this would be provided.
class MockNestedInteger {
public:
    bool integerFlag;
    int value;
    std::vector<MockNestedInteger> list;

    MockNestedInteger(int v) : integerFlag(true), value(v), list() {}
    MockNestedInteger(const std::vector<MockNestedInteger>& l) : integerFlag(false), value(0), list(l) {}

    bool isInteger() const { return integerFlag; }
    int getInteger() const { return value; }
    const std::vector<MockNestedInteger>& getList() const { return list; }
};

// Since the task requires using the actual NestedInteger interface, we adapt the test
// by defining a conversion wrapper. For brevity, we'll directly test with MockNestedInteger
// but call the same algorithm logic via a helper that works with the mock.
// In a real submission, the function would be tested with the real NestedInteger.
std::vector<int> flattenMock(const std::vector<MockNestedInteger>& nestedList) {
    std::vector<int> result;
    std::stack<MockNestedInteger> workStack;

    for (int i = static_cast<int>(nestedList.size()) - 1; i >= 0; --i) {
        workStack.push(nestedList[i]);
    }

    while (!workStack.empty()) {
        MockNestedInteger current = workStack.top();
        workStack.pop();

        if (current.isInteger()) {
            result.push_back(current.getInteger());
        } else {
            const std::vector<MockNestedInteger>& innerList = current.getList();
            for (int i = static_cast<int>(innerList.size()) - 1; i >= 0; --i) {
                workStack.push(innerList[i]);
            }
        }
    }

    return result;
}

int main() {
    // Test 1: Basic flat list
    std::vector<MockNestedInteger> flat = {MockNestedInteger(1), MockNestedInteger(2), MockNestedInteger(3)};
    assert(flattenMock(flat) == std::vector<int>({1, 2, 3}));

    // Test 2: Single integer
    std::vector<MockNestedInteger> single = {MockNestedInteger(42)};
    assert(flattenMock(single) == std::vector<int>({42}));

    // Test 3: Empty list
    std::vector<MockNestedInteger> empty;
    assert(flattenMock(empty) == std::vector<int>());

    // Test 4: One nested list
    std::vector<MockNestedInteger> nested1 = {MockNestedInteger(std::vector<MockNestedInteger>{MockNestedInteger(5), MockNestedInteger(6)})};
    assert(flattenMock(nested1) == std::vector<int>({5, 6}));

    // Test 5: Mixed nesting and duplicates
    std::vector<MockNestedInteger> mixed = {
        MockNestedInteger(1),
        MockNestedInteger(std::vector<MockNestedInteger>{
            MockNestedInteger(2),
            MockNestedInteger(std::vector<MockNestedInteger>{MockNestedInteger(3), MockNestedInteger(3)}),
            MockNestedInteger(4)
        }),
        MockNestedInteger(5)
    };
    assert(flattenMock(mixed) == std::vector<int>({1, 2, 3, 3, 4, 5}));

    // Test 6: Empty inner lists
    std::vector<MockNestedInteger> withEmpty = {
        MockNestedInteger(std::vector<MockNestedInteger>{}),
        MockNestedInteger(7),
        MockNestedInteger(std::vector<MockNestedInteger>{MockNestedInteger(std::vector<MockNestedInteger>{}), MockNestedInteger(8)})
    };
    assert(flattenMock(withEmpty) == std::vector<int>({7, 8}));

    return 0;
}
