/*
Write a C++ function named `removeValueFromList` that takes a reference to a `std::forward_list<int>` and an integer value, and removes the first occurrence of that value from the list. If the value is not found, the list should remain unchanged. The function must not use any external data structures or the `std::list`/`std::vector` containers, and must operate directly on the forward list's iterators. The function should be `const`-correct where appropriate, meaning it should not modify the list if the value is absent, and it should handle the case where the list is empty. The function returns `void`.
*/
#include <forward_list>

// Remove the first occurrence of 'value' from the forward list.
// If 'value' is not present, the list is left unchanged.
void removeValueFromList(std::forward_list<int>& lst, int value) {
    // Handle empty list
    if (lst.empty()) {
        return;
    }
    
    // Use before_begin() to handle removal of the head node.
    auto previous = lst.before_begin();
    auto current = lst.begin();
    
    // Traverse until we find the value or reach the end.
    while (current != lst.end() && *current != value) {
        ++previous;
        ++current;
    }
    
    // If we found the value (current != end), remove it.
    if (current != lst.end()) {
        lst.erase_after(previous);
    }
    // If not found, current == lst.end() and we do nothing.
}
#include <forward_list>
#include <cassert>

void removeValueFromList(std::forward_list<int>& lst, int value);

int main() {
    // Test 1: Remove from middle
    std::forward_list<int> list1 = {1, 2, 3, 4};
    removeValueFromList(list1, 3);
    assert((list1 == std::forward_list<int>{1, 2, 4}));

    // Test 2: Remove head
    std::forward_list<int> list2 = {5, 6, 7};
    removeValueFromList(list2, 5);
    assert((list2 == std::forward_list<int>{6, 7}));

    // Test 3: Value not found
    std::forward_list<int> list3 = {10, 20, 30};
    removeValueFromList(list3, 99);
    assert((list3 == std::forward_list<int>{10, 20, 30}));

    // Test 4: Empty list
    std::forward_list<int> list4;
    removeValueFromList(list4, 1);
    assert(list4.empty());

    // Test 5: Single element found
    std::forward_list<int> list5 = {42};
    removeValueFromList(list5, 42);
    assert(list5.empty());

    // Test 6: Single element not found
    std::forward_list<int> list6 = {42};
    removeValueFromList(list6, 7);
    assert((list6 == std::forward_list<int>{42}));

    // Test 7: Multiple duplicates, only first removed
    std::forward_list<int> list7 = {1, 2, 2, 2, 3};
    removeValueFromList(list7, 2);
    assert((list7 == std::forward_list<int>{1, 2, 2, 3}));

    // Test 8: Remove only element after multiple
    std::forward_list<int> list8 = {1, 2, 3};
    removeValueFromList(list8, 3);
    assert((list8 == std::forward_list<int>{1, 2}));

    // Test 9: Remove first element when list has two elements
    std::forward_list<int> list9 = {7, 8};
    removeValueFromList(list9, 7);
    assert((list9 == std::forward_list<int>{8}));

    // Test 10: Remove second element when first is different
    std::forward_list<int> list10 = {9, 10};
    removeValueFromList(list10, 10);
    assert((list10 == std::forward_list<int>{9}));

    return 0;
}
// The core algorithm is a linear traversal of the singly linked list structure provided by `std::forward_list`. We maintain a "previous" iterator (or a pointer to the node's `next` pointer) to unlink the target node. The challenge is that `std::forward_list` provides only forward iterators and does not allow easy access to the previous node; a common trick is to keep a pointer to the `next` pointer of the previous node (or use `before_begin()`). We traverse using two iterators: one that points to the node before the current, and one for the current. We start with `previous = before_begin()` and `current = begin()`. If we find the value, we link `previous->next = current->next` and erase via `erase_after(previous)` (which does the unlink and deallocation). Edge cases: empty list (simply return), value at the head (must handle because `before_begin()` works for this), value not found (no modification), and duplicates (only first removed). Time complexity is O(n) for n elements, space O(1). Because we only modify the list when needed, `const` correctness: the function takes a non-const reference to allow mutation when the value exists, but we must ensure we don't accidentally mutate on not-found—the logic naturally avoids that.
