Write a C++ function named `sortArrayByChoice` that takes a `std::vector<int>` and an integer `method` (1 = ascending, 2 = descending, otherwise invalid) and returns a new `std::vector<int>` containing the sorted elements according to the method. If `method` is not 1 or 2, return an empty vector. The input vector may contain any integers, including duplicates, and may be empty (in which case, if method is valid, return an empty vector). Your function must not modify the original vector. Solve using a stable in-place bubble sort algorithm (swapping adjacent elements) that works on a copy of the input.

The solution creates a local copy of the input vector so the original remains unchanged. Then, depending on the method, it performs a standard bubble sort: for ascending order, swap adjacent elements if the left is strictly greater than the right; for descending order, swap if the left is strictly less than the right. After each outer pass, the largest (or smallest) element bubbles to the end, so the inner loop length decreases by one each time, as in the classic bubble sort. Edge cases: if the vector is empty, the loops do nothing and we return the empty copy; if method is not 1 or 2, return an empty vector immediately (even if input is non-empty). Duplicates do not require swapping (strict comparisons), preserving relative order for equal elements, making the sort stable. Time complexity is O(n²) in the average and worst case due to nested loops, and O(n) in the best case if we added early exit, but here without early exit it's always O(n²). Space complexity is O(n) because we copy the input vector; no extra significant auxiliary space.

#include <vector>

// Sorts a copy of the input vector according to method.
// method 1: ascending, method 2: descending, otherwise returns empty.
std::vector<int> sortArrayByChoice(const std::vector<int>& input, int method) {
    if (method != 1 && method != 2) {
        return {};
    }
    std::vector<int> result = input; // copy to avoid modifying original
    const int n = static_cast<int>(result.size());
    
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            bool needSwap = false;
            if (method == 1 && result[j] > result[j + 1]) {
                needSwap = true;
            } else if (method == 2 && result[j] < result[j + 1]) {
                needSwap = true;
            }
            if (needSwap) {
                int temp = result[j];
                result[j] = result[j + 1];
                result[j + 1] = temp;
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// (function declaration and implementation from above would be placed here)

int main() {
    std::vector<int> empty;
    assert(sortArrayByChoice(empty, 1) == empty);
    assert(sortArrayByChoice(empty, 2) == empty);
    assert(sortArrayByChoice(empty, 3) == empty);

    std::vector<int> single = {42};
    assert(sortArrayByChoice(single, 1) == single);
    assert(sortArrayByChoice(single, 2) == single);

    std::vector<int> duplicates = {3, 1, 3, 2};
    std::vector<int> asc = {1, 2, 3, 3};
    std::vector<int> desc = {3, 3, 2, 1};
    assert(sortArrayByChoice(duplicates, 1) == asc);
    assert(sortArrayByChoice(duplicates, 2) == desc);

    std::vector<int> negatives = {-5, -1, -10, 0};
    std::vector<int> ascNeg = {-10, -5, -1, 0};
    std::vector<int> descNeg = {0, -1, -5, -10};
    assert(sortArrayByChoice(negatives, 1) == ascNeg);
    assert(sortArrayByChoice(negatives, 2) == descNeg);

    std::vector<int> original = {5, 4, 3, 2, 1};
    std::vector<int> sortedAsc = {1, 2, 3, 4, 5};
    std::vector<int> sortedDesc = {5, 4, 3, 2, 1};
    assert(sortArrayByChoice(original, 1) == sortedAsc);
    assert(sortArrayByChoice(original, 2) == sortedDesc);
    // Verify original was not modified
    assert(original[0] == 5 && original[4] == 1);

    // Invalid method returns empty
    assert(sortArrayByChoice({1,2,3}, 0).empty());
}
