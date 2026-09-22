Write a C++ function that takes a `std::forward_list<int>` as input and returns a new `std::forward_list<int>` containing the same elements in reverse order. The function must not modify the original list, must handle empty lists and lists with a single element correctly, and must preserve any duplicate values. The solution should use only standard library facilities and demonstrate proper `const` correctness by taking the input list by `const` reference.
// The solution creates a new `std::forward_list<int>` and iterates through the original list from beginning to end, inserting each element at the front of the new list using the `push_front` method. Since forward lists are singly linked, inserting at the front is an `O(1)` operation per element, resulting in an overall `O(n)` time complexity where `n` is the number of elements. The space complexity is `O(n)` for the new list, plus constant auxiliary space for the iterator. For an empty input list, the function simply returns an empty list. For a list with one element, the element is copied once, and the returned list contains that single element. No special handling is needed for duplicates because each element is independently pushed to the front, preserving all occurrences. The function takes the input by `const std::forward_list<int>&` to avoid copying, and returns a new list by value.
#include <forward_list>

// Return a new forward_list containing the elements of `input` in reverse order.
// The original list is left unmodified.
std::forward_list<int> reverseForwardList(const std::forward_list<int>& input) {
    std::forward_list<int> result;
    for (int value : input) {
        result.push_front(value);
    }
    return result;
}
#include <cassert>
#include <forward_list>
#include <initializer_list>

int main() {
    // Helper to compare two forward_lists
    auto listsEqual = [](const std::forward_list<int>& a, const std::forward_list<int>& b) {
        return std::equal(a.begin(), a.end(), b.begin(), b.end());
    };

    // Test 1: Normal case
    std::forward_list<int> l1{1, 2, 3, 4, 5};
    std::forward_list<int> r1 = reverseForwardList(l1);
    assert(listsEqual(r1, std::forward_list<int>({5, 4, 3, 2, 1})));
    // Original unchanged
    assert(listsEqual(l1, std::forward_list<int>({1, 2, 3, 4, 5})));

    // Test 2: Empty list
    std::forward_list<int> l2;
    std::forward_list<int> r2 = reverseForwardList(l2);
    assert(r2.empty());

    // Test 3: Single element
    std::forward_list<int> l3{42};
    std::forward_list<int> r3 = reverseForwardList(l3);
    assert(listsEqual(r3, std::forward_list<int>({42})));

    // Test 4: Duplicate values
    std::forward_list<int> l4{7, 7, 8, 7};
    std::forward_list<int> r4 = reverseForwardList(l4);
    assert(listsEqual(r4, std::forward_list<int>({7, 8, 7, 7})));

    // Test 5: Negative and zero values
    std::forward_list<int> l5{-1, 0, -5, 3};
    std::forward_list<int> r5 = reverseForwardList(l5);
    assert(listsEqual(r5, std::forward_list<int>({3, -5, 0, -1})));

    // Note: after all assertions, original lists remain unchanged
    assert(listsEqual(l5, std::forward_list<int>({-1, 0, -5, 3})));

    return 0;
}
