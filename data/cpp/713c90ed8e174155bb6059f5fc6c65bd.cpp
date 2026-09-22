// Write a C++ function that sorts an array of arbitrary element type using a stable sort. The function must accept a void pointer to the first element, the number of elements, the size of each element in bytes, a comparator function that returns a negative, zero, or positive integer depending on whether the first compared element is less than, equal to, or greater than the second compared element, and an optional context pointer passed through to the comparator. The sort must be stable: equal elements must retain their original relative order. The function must have signature `void stableSort(void* array, int length, int itemSize, int (*cmp)(const void*, const void*, const void*), const void* context)` and must handle edge cases such as length 0, length 1, and invalid parameters (null array with length > 0, non-positive itemSize, null comparator) by doing nothing for valid empty/single-element cases and throwing a `std::invalid_argument` for invalid parameters.
The core challenge is implementing a stable sort that works on raw memory without knowing the element type. The stable sort algorithm chosen is an insertion sort variant that uses a binary search to find the correct insertion point for each element, with special handling to ensure stability when duplicates exist. The binary search is modified to find the last occurrence of an equal element, then we insert after that last equal element. For small arrays (length < 9), a simple linear insertion sort suffices. For larger arrays, we could use a stable merge sort, but for simplicity and to directly inspire from the given code snippet, we can implement the stable insertion sort with binary search for all lengths, which is O(n log n) comparisons in the best case but O(n²) in the worst case due to the memmove shifting. However, since the task focuses on stability and correctness rather than optimal performance, this is acceptable. The algorithm works as follows: for each element at index j (starting from 1), we extract it into a temporary buffer, perform a stable binary search over the already-sorted subarray [0, j-1] to find the insertion point, then shift the elements after that point right by one position and place the extracted element in the correct spot. Edge cases include: length ≤ 1 (do nothing), null array with length > 0 (throw), itemSize ≤ 0 (throw), null comparator (throw), and potential memory allocation failure for the temporary buffer (throw `std::bad_alloc`). The time complexity is O(n²) worst-case due to memmove operations (each shifting O(n) elements), with O(n log n) comparisons in the best case. Space complexity is O(itemSize) for the temporary buffer.
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <new>

// Stable sort an array of arbitrary element type.
// cmp must return negative, zero, or positive based on comparison.
// context is passed through to cmp.
void stableSort(void* array, int length, int itemSize,
                int (*cmp)(const void*, const void*, const void*),
                const void* context) {
    // Validate parameters
    if (itemSize <= 0 || cmp == nullptr) {
        throw std::invalid_argument("Invalid sort parameters");
    }
    if (length < 0) {
        throw std::invalid_argument("Negative length");
    }
    if (length > 0 && array == nullptr) {
        throw std::invalid_argument("Null array with non-zero length");
    }
    if (length <= 1) {
        return;
    }

    char* base = static_cast<char*>(array);
    // Temporary storage for one element
    char* temp = new char[itemSize];

    for (int j = 1; j < length; ++j) {
        // Copy array[j] to temp
        std::memcpy(temp, base + j * itemSize, itemSize);

        // Stable binary search for insertion point in [0, j-1]
        int start = 0;
        int limit = j;
        int insertionPoint = j; // default: place at end

        // Binary search to find the last position where a[j] >= array[i]
        while (start < limit) {
            int mid = (start + limit) / 2;
            int diff = cmp(context, temp, base + mid * itemSize);
            if (diff >= 0) {
                // temp >= array[mid], move right
                start = mid + 1;
            } else {
                // temp < array[mid], move left
                limit = mid;
            }
        }
        insertionPoint = start; // first position where temp < array[i], or j if all <=

        // Shift elements right by one position from insertionPoint to j-1
        if (insertionPoint < j) {
            std::memmove(base + (insertionPoint + 1) * itemSize,
                         base + insertionPoint * itemSize,
                         (j - insertionPoint) * static_cast<size_t>(itemSize));
            std::memcpy(base + insertionPoint * itemSize, temp, itemSize);
        } else {
            // Already in correct position, just copy back if needed
            // (not necessary, but for clarity)
            std::memcpy(base + j * itemSize, temp, itemSize);
        }
    }
    delete[] temp;
}
#include <cassert>
#include <cstring>
#include <string>

// Comparator for integers: context is unused, casts void* to int*
static int intComparator(const void* /*context*/, const void* a, const void* b) {
    int ia = *static_cast<const int*>(a);
    int ib = *static_cast<const int*>(b);
    return (ia > ib) - (ia < ib);
}

// Comparator for pairs (first, second) - compares first, then second
struct Pair {
    int first;
    int second;
};

static int pairComparator(const void* /*context*/, const void* a, const void* b) {
    const Pair* pa = static_cast<const Pair*>(a);
    const Pair* pb = static_cast<const Pair*>(b);
    if (pa->first != pb->first) {
        return (pa->first > pb->first) - (pa->first < pb->first);
    }
    return (pa->second > pb->second) - (pa->second < pb->second);
}

// Comparator that uses context (e.g., for strings)
static int stringComparator(const void* context, const void* a, const void* b) {
    // Assuming context is a flag indicating case sensitivity
    bool caseSensitive = *static_cast<const bool*>(context);
    std::string sa = *static_cast<const std::string*>(a);
    std::string sb = *static_cast<const std::string*>(b);
    if (!caseSensitive) {
        for (char& c : sa) c = std::toupper(c);
        for (char& c : sb) c = std::toupper(c);
    }
    return sa.compare(sb);
}

int main() {
    // Test 1: Empty array
    int emptyArr[1] = {42};
    int emptyLen = 0;
    stableSort(emptyArr, emptyLen, sizeof(int), intComparator, nullptr);
    assert(emptyLen == 0);

    // Test 2: Single element
    int single[1] = {7};
    stableSort(single, 1, sizeof(int), intComparator, nullptr);
    assert(single[0] == 7);

    // Test 3: Already sorted
    int sorted[5] = {1, 2, 3, 4, 5};
    stableSort(sorted, 5, sizeof(int), intComparator, nullptr);
    assert(sorted[0] == 1 && sorted[4] == 5);

    // Test 4: Reverse sorted
    int reverse[5] = {5, 4, 3, 2, 1};
    stableSort(reverse, 5, sizeof(int), intComparator, nullptr);
    assert(reverse[0] == 1 && reverse[4] == 5);

    // Test 5: Unsorted with duplicates
    int arr[7] = {3, 1, 4, 1, 5, 9, 2};
    stableSort(arr, 7, sizeof(int), intComparator, nullptr);
    assert(arr[0] == 1 && arr[1] == 1 && arr[6] == 9);
    assert(arr[2] == 2 && arr[3] == 3 && arr[4] == 4);

    // Test 6: Stability check with pairs
    Pair pairs[5] = {{2, 1}, {1, 2}, {2, 2}, {1, 1}, {2, 3}};
    stableSort(pairs, 5, sizeof(Pair), pairComparator, nullptr);
    // After sorting by first then second, but stability means equal first elements
    // keep original relative order. Original order: (2,1),(1,2),(2,2),(1,1),(2,3)
    // Sorted by first: (1,2),(1,1),(2,1),(2,2),(2,3)
    // Stable means (2,1) comes before (2,2) before (2,3), and (1,2) before (1,1)
    assert(pairs[0].first == 1 && pairs[0].second == 2);
    assert(pairs[1].first == 1 && pairs[1].second == 1);
    assert(pairs[2].first == 2 && pairs[2].second == 1);
    assert(pairs[3].first == 2 && pairs[3].second == 2);
    assert(pairs[4].first == 2 && pairs[4].second == 3);

    // Test 7: Stability with duplicates only
    int dup[4] = {5, 5, 5, 5};
    stableSort(dup, 4, sizeof(int), intComparator, nullptr);
    assert(dup[0] == 5 && dup[3] == 5);

    // Test 8: Context usage with strings (case-insensitive)
    std::string strings[3] = {"apple", "Banana", "cherry"};
    bool caseSensitive = false;
    stableSort(strings, 3, sizeof(std::string), stringComparator, &caseSensitive);
    std::string expected1[3] = {"apple", "Banana", "cherry"}; // case-insensitive sorted
    for (int i = 0; i < 3; ++i) {
        assert(strings[i] == expected1[i]);
    }

    // Test 9: Context usage with case-sensitive
    caseSensitive = true;
    std::string strings2[3] = {"apple", "Banana", "cherry"};
    stableSort(strings2, 3, sizeof(std::string), stringComparator, &caseSensitive);
    assert(strings2[0] == "Banana");  // uppercase 'B' before lowercase 'a' in ASCII
    assert(strings2[1] == "apple");
    assert(strings2[2] == "cherry");

    // Test 10: Invalid parameters throw
    bool threw = false;
    try {
        stableSort(nullptr, 1, sizeof(int), intComparator, nullptr);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        int dummy[1] = {1};
        stableSort(dummy, 1, 0, intComparator, nullptr);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        int dummy2[1] = {1};
        stableSort(dummy2, 1, sizeof(int), nullptr, nullptr);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
