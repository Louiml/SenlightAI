// Write a C++ function `removeDuplicatesFromBag` that takes a constant reference to a `Bag` object (as defined in the provided code snippet) and removes all duplicate elements from the bag, keeping only one occurrence of each distinct element. The function should return a new `Bag` containing exactly one copy of each distinct element from the original bag. The original bag must remain unchanged. You may assume the `Bag` class and its `BagIterator` are fully implemented with the same interface as shown in the snippet (including `add`, `remove`, `size`, `search`, `nrOccurrences`, `isEmpty`, `iterator`, and a copy-accessible structure via iterators). The solution should not modify the input bag and must handle empty bags gracefully. The order of elements in the returned bag does not matter.

#include <cassert>

int main() {
    // Test 1: Empty bag
    Bag emptyBag;
    Bag result1 = removeDuplicatesFromBag(emptyBag);
    assert(result1.isEmpty());
    assert(result1.size() == 0);

    // Test 2: Bag with all distinct elements
    Bag distinctBag;
    distinctBag.add(1);
    distinctBag.add(2);
    distinctBag.add(3);
    Bag result2 = removeDuplicatesFromBag(distinctBag);
    assert(result2.size() == 3);
    assert(result2.search(1) && result2.search(2) && result2.search(3));
    assert(result2.nrOccurrences(1) == 1);
    assert(result2.nrOccurrences(2) == 1);
    assert(result2.nrOccurrences(3) == 1);

    // Test 3: Bag with duplicates
    Bag dupBag;
    dupBag.add(5);
    dupBag.add(5);
    dupBag.add(5);
    dupBag.add(7);
    dupBag.add(7);
    dupBag.add(9);
    Bag result3 = removeDuplicatesFromBag(dupBag);
    assert(result3.size() == 3);
    assert(result3.nrOccurrences(5) == 1);
    assert(result3.nrOccurrences(7) == 1);
    assert(result3.nrOccurrences(9) == 1);

    // Test 4: Original bag unchanged
    Bag original;
    original.add(2);
    original.add(2);
    original.add(4);
    original.add(6);
    original.add(6);
    removeDuplicatesFromBag(original);
    assert(original.size() == 5);
    assert(original.nrOccurrences(2) == 2);
    assert(original.nrOccurrences(6) == 2);
    assert(original.nrOccurrences(4) == 1);

    // Test 5: Bag with single element repeated many times
    Bag singleRepeated;
    for (int i = 0; i < 100; ++i) {
        singleRepeated.add(42);
    }
    Bag result5 = removeDuplicatesFromBag(singleRepeated);
    assert(result5.size() == 1);
    assert(result5.nrOccurrences(42) == 1);

    // Test 6: Large bag with many duplicates
    Bag largeBag;
    for (int i = 0; i < 1000; ++i) {
        largeBag.add(i % 10); // 100 copies of each 0..9
    }
    Bag result6 = removeDuplicatesFromBag(largeBag);
    assert(result6.size() == 10);
    for (int i = 0; i < 10; ++i) {
        assert(result6.nrOccurrences(i) == 1);
    }
    assert(largeBag.size() == 1000); // unchanged

    return 0;
}

#include "Bag.h"
#include "BagIterator.h"
#include <unordered_set>

// Return a new Bag containing exactly one copy of each distinct element from the input bag.
// The input bag remains unchanged.
Bag removeDuplicatesFromBag(const Bag& bag) {
    Bag result;
    std::unordered_set<TElem> seen;
    BagIterator it = bag.iterator();
    while (it.valid()) {
        TElem elem = it.getCurrent();
        if (seen.find(elem) == seen.end()) {
            seen.insert(elem);
            result.add(elem);
        }
        it.next();
    }
    return result;
}

// The main challenge is to iterate over all distinct elements of the input bag without modifying it, and build a new bag containing just one copy of each. Since the `Bag` stores elements as `std::pair<TElem, int>` where `first` is the element and `second` is its frequency, we can use the `BagIterator` to traverse all elements. However, an iterator returns each element as many times as its frequency (since the bag stores multiplicities). To avoid duplicates in the output, we must track which elements we have already added. A simple approach: use a `std::unordered_set<TElem>` (or `std::set` if `TElem` is not hashable; but we assume `TElem` is `int` for simplicity) to keep track of seen elements. Iterate through the original bag using its iterator, and for each element, if it has not been seen before, add it to the new bag and mark it as seen. This ensures each distinct element is added only once. The time complexity is O(n) where n is the total number of elements in the bag (since each element is processed once and each `add` operation is O(1) amortized because of the free-list and head insertion). Space complexity is O(d) for the set and O(capacity) for the new bag, where d is the number of distinct elements. Edge cases: empty bag returns an empty bag; bag with all duplicates returns a single-element bag.
