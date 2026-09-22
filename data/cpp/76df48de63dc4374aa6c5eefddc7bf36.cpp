/*
Write a C++ function named `binarySearchCost` that takes a sorted vector of strings (`words`), a vector of corresponding integer costs (`costs`), an integer default cost (`defaultCost`), and a query string (`key`). The function must perform a binary search on `words` to find `key`. If found, return the cost at the matching index; otherwise, return `defaultCost`. The input vectors are guaranteed to be non-empty and of equal length, and `words` is sorted in ascending lexicographic order. Handle edge cases such as an empty `key` string (return `defaultCost`) and ensure the function uses `const` references for read-only access.
*/
#include <string>
#include <vector>

// Binary search for a key in a sorted vector of strings with associated costs.
// Returns the cost if found; otherwise returns defaultCost.
int binarySearchCost(
    const std::vector<std::string>& words,
    const std::vector<int>& costs,
    int defaultCost,
    const std::string& key
) {
    if (key.empty()) {
        return defaultCost;
    }

    int lo = 0;
    int hi = static_cast<int>(words.size()) - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (words[mid] == key) {
            return costs[mid];
        }
        if (words[mid] < key) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return defaultCost;
}
#include <cassert>
#include <string>
#include <vector>

// Include the solution function declaration here (or copy the function above)

int main() {
    std::vector<std::string> words = {"apple", "banana", "cherry", "date"};
    std::vector<int> costs = {10, 20, 30, 40};

    // Found cases
    assert(binarySearchCost(words, costs, 99, "apple") == 10);
    assert(binarySearchCost(words, costs, 99, "date") == 40);
    assert(binarySearchCost(words, costs, 99, "cherry") == 30);

    // Not found cases
    assert(binarySearchCost(words, costs, 99, "blueberry") == 99);
    assert(binarySearchCost(words, costs, 99, "zzz") == 99);
    assert(binarySearchCost(words, costs, 99, "aardvark") == 99);

    // Empty key
    assert(binarySearchCost(words, costs, 99, "") == 99);

    // Single-element vector
    std::vector<std::string> singleWords = {"only"};
    std::vector<int> singleCosts = {7};
    assert(binarySearchCost(singleWords, singleCosts, 0, "only") == 7);
    assert(binarySearchCost(singleWords, singleCosts, 0, "nope") == 0);

    // Duplicate words (though input should be sorted, duplicates would break
    // binary search, but we test that first occurrence or any match works)
    std::vector<std::string> dupWords = {"a", "a", "b"};
    std::vector<int> dupCosts = {1, 2, 3};
    int cost = binarySearchCost(dupWords, dupCosts, -1, "a");
    assert(cost == 1 || cost == 2);

    return 0;
}
// The solution uses a classic binary search algorithm on a sorted array. Initialize `lo = 0` and `hi = words.size() - 1`. In each iteration, compute `mid = (lo + hi) / 2` and compare `words[mid]` with `key` using `std::string::compare` or `==`/`<`. If equal, return `costs[mid]`. If `words[mid]` is lexicographically less than `key`, move `lo = mid + 1`; otherwise, move `hi = mid - 1`. Repeat until `lo > hi`. If the loop exits without a match, return `defaultCost`. The empty-string case is trivially handled because binary search will not find it unless the vector contains an empty string, but the specification explicitly says to return `defaultCost` for empty keys, so we check that upfront. Time complexity is `O(log n)` comparisons, each `O(k)` where `k` is the average string length, giving `O(k log n)`. Space complexity is `O(1)` auxiliary.
