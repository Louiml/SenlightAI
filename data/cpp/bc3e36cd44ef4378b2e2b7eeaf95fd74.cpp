// Write a C++ function named `sortAndDescribeList` that takes a `std::list<int>` by value, sorts it in descending order using a lambda expression as the comparison function (similar to the provided snippet), pushes the value `0` to the front and `1000` to the back after sorting, and then returns a `std::string` containing: first the front element, then a space, the back element, a space, and the middle element (the element at index `size()/2` if the list size is odd, or the average (as integer division) of the two middle elements if even), all separated by spaces. The function must be `const`-correct where applicable and handle edge cases like an empty list or a single-element list gracefully (returning `"0 0 0"` for empty or `"0 0 0"` for single element after modifications). For example, given `{3, 1, 5, 4, 2}`, after sorting descending it becomes `{5,4,3,2,1}`, then push 0 front → `{0,5,4,3,2,1}` and push 1000 back → `{0,5,4,3,2,1,1000}`. Size is 7, middle index is 3, middle element is 3. The returned string would be `"0 1000 3"`.

The solution approach: First, copy the input list into a local `std::list<int>` (since we take by value, we can modify directly). If the list is empty, return `"0 0 0"` immediately. Then sort the list in descending order using `list.sort([](int a, int b){ return a > b; })` – note that the provided snippet uses `two < one` which also sorts descending. After sorting, push `0` to the front and `1000` to the back. Then compute the middle index as `size/2` (integer division). If the list size is odd, the middle element is at that index; if even, take the average of the two middle elements (at indices `size/2 - 1` and `size/2`). Use `std::next` to get iterators to those positions, since `std::list` does not support random access. Build the result string using `std::to_string` for the front, back, and middle value. Edge cases: empty list (return default string), single element after operations (the middle logic still works if size is 1). Time complexity is O(n log n) for the sort, O(n) for `std::next` if we advance iterators, but we can use `std::advance` which is O(n) worst-case for a linked list. Space complexity is O(n) due to the list copy (though we modify in place).

#include <list>
#include <string>
#include <iterator>  // for std::next
#include <numeric>   // for std::accumulate (not strictly needed)

// Sorts a list in descending order, adds 0 to front and 1000 to back,
// then returns a string "front back middle" (or "0 0 0" for empty input).
std::string sortAndDescribeList(std::list<int> l) {
    if (l.empty()) {
        return "0 0 0";
    }

    // Sort descending using a lambda (same as `two < one`).
    l.sort([](int one, int two) { return two < one; });

    // Add sentinel values after sorting.
    l.push_front(0);
    l.push_back(1000);

    // Compute middle value.
    size_t size = l.size();
    int middle;
    if (size % 2 == 1) {
        auto it = l.begin();
        std::advance(it, size / 2);
        middle = *it;
    } else {
        auto it1 = l.begin();
        std::advance(it1, size / 2 - 1);
        auto it2 = l.begin();
        std::advance(it2, size / 2);
        middle = (*it1 + *it2) / 2; // integer division
    }

    return std::to_string(l.front()) + " " + std::to_string(l.back()) + " " + std::to_string(middle);
}

#include <cassert>
#include <list>
#include <string>

int main() {
    // Regular case: {3,1,5,4,2} -> sorted desc {5,4,3,2,1} -> push 0 front, 1000 back -> {0,5,4,3,2,1,1000}
    std::list<int> l1{3,1,5,4,2};
    assert(sortAndDescribeList(l1) == "0 1000 3");

    // Even size: {1,2} -> sorted desc {2,1} -> push 0 front, 1000 back -> {0,2,1,1000}
    // size=4, middle indices 1 and 2 → average (2+1)/2 = 1
    std::list<int> l2{1,2};
    assert(sortAndDescribeList(l2) == "0 1000 1");

    // Single element: {7} -> sorted {7} -> push 0 front, 1000 back -> {0,7,1000} size=3, middle index 1 → 7
    std::list<int> l3{7};
    assert(sortAndDescribeList(l3) == "0 1000 7");

    // Empty list → default string
    std::list<int> l4;
    assert(sortAndDescribeList(l4) == "0 0 0");

    // Duplicate values: {5,5,5} -> sorted {5,5,5} -> push → {0,5,5,5,1000} size=5, middle index 2 → 5
    std::list<int> l5{5,5,5};
    assert(sortAndDescribeList(l5) == "0 1000 5");

    // Negative numbers: {-3,-1,-2} -> sorted descending { -1, -2, -3 } (since -1 > -2 > -3) -> push → {0,-1,-2,-3,1000} size=5 middle index 2 → -2
    std::list<int> l6{-3,-1,-2};
    assert(sortAndDescribeList(l6) == "0 1000 -2");

    return 0;
}
