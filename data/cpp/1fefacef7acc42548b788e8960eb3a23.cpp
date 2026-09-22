// Write a C++ function `std::vector<int> findNextGreater(const std::vector<int>& query, const std::vector<int>& reference)` that, for each element in `query`, returns the next greater element as defined in the classic "Next Greater Element I" problem: given that `query` is a subset of `reference` (but not necessarily contiguous), for each element `x` in `query`, find its position in `reference`, then scan to the *right* of that position for the first element strictly larger than `x`. If no such element exists, the result is `-1`. The function must handle duplicate values in `reference`, empty `query`, and cases where `query` contains an element not present in `reference` (in that case, also return `-1`). The function should not modify the inputs and must use `const` references for both parameters. The expected algorithm uses a monotonic decreasing stack to precompute, for every element in `reference`, its next greater element in `O(n)` time, then answer all queries in `O(m)` time, where `n = reference.size()` and `m = query.size()`. Total time complexity is `O(n + m)`, and space complexity is `O(n)` for the stack and hash map.
#include <cassert>
#include <vector>

// Declaration of the tested function.
std::vector<int> findNextGreater(const std::vector<int>& query, const std::vector<int>& reference);

int main() {
    // Basic example from problem statement.
    assert((findNextGreater({4,1,2}, {1,3,4,2}) == std::vector<int>{-1,3,-1}));
    
    // Query is the same as reference.
    assert((findNextGreater({2,4}, {2,4}) == std::vector<int>{4,-1}));
    
    // Empty query.
    assert((findNextGreater({}, {1,2,3}) == std::vector<int>{}));
    
    // Empty reference: all queries return -1.
    assert((findNextGreater({1,2}, {}) == std::vector<int>{-1,-1}));
    
    // Duplicate values in reference: rightmost occurrence determines the answer.
    assert((findNextGreater({2}, {1,2,3,2}) == std::vector<int>{3}));
    
    // Element not present in reference returns -1.
    assert((findNextGreater({10,0}, {1,2,3}) == std::vector<int>{-1,-1}));
    
    // Strictly decreasing reference: no next greater element for any.
    assert((findNextGreater({5,3,1}, {5,3,1}) == std::vector<int>{-1,-1,-1}));
    
    // Strictly increasing reference.
    assert((findNextGreater({1,2,3}, {1,2,3}) == std::vector<int>{2,3,-1}));
    
    // Mixed case with multiple queries.
    assert((findNextGreater({1,5,2,4}, {2,1,5,3,4}) == std::vector<int>{5,-1,3,-1}));
    
    // Large values to ensure no overflow.
    assert((findNextGreater({1000000, 1}, {1, 1000000, 2000000}) == std::vector<int>{2000000,1000000}));
    
    return 0;
}
#include <vector>
#include <stack>
#include <unordered_map>

// For each element in query, return the next greater element as found in reference.
// If no such element exists or the element is absent, return -1.
std::vector<int> findNextGreater(const std::vector<int>& query, const std::vector<int>& reference) {
    std::stack<int> decreasingStack;
    std::unordered_map<int, int> nextGreaterMap;
    
    for (int current : reference) {
        while (!decreasingStack.empty() && decreasingStack.top() < current) {
            nextGreaterMap[decreasingStack.top()] = current;
            decreasingStack.pop();
        }
        decreasingStack.push(current);
    }
    
    // Elements left in stack have no next greater element.
    // No explicit mapping needed; map.count will be 0 for them.
    
    std::vector<int> result;
    result.reserve(query.size());
    for (int value : query) {
        auto it = nextGreaterMap.find(value);
        result.push_back(it != nextGreaterMap.end() ? it->second : -1);
    }
    return result;
}
// The solution processes `reference` from left to right, maintaining a stack that stores indices (or values) for which the next greater element has not yet been found. For each new element `current` in `reference`, we pop all elements from the stack that are strictly smaller than `current`; for each popped element, its next greater element is exactly `current`. We record this mapping in an `unordered_map<int, int>`. After the loop, any remaining elements in the stack have no greater element to their right, so their mapped value is `-1` (implicitly handled by the map's `count` check). Then, for each element in `query`, we simply look it up in the map; if present, return its next greater, otherwise `-1`. Edge cases include: empty `reference` (then every query returns `-1`), duplicate values in `reference` (the map will overwrite with the value from the rightmost occurrence, which is correct because `query` elements are unique and we only care about the first occurrence to the right; however, if a value appears multiple times, the map will store the next greater for the last occurrence, which is still valid because the function semantics only require *any* occurrence? Actually the standard problem guarantees `query` is a subset of `reference` and elements are unique, but to be safe we return `-1` if not found), and large values where `int` overflow is not an issue. Time complexity is linear in the total size of both inputs; space complexity is `O(n)` for the map and stack.
