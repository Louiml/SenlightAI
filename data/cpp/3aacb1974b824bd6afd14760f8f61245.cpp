Create a C++ function named `sumOfFirstPages` that takes a vector of non-negative integers representing page counts of chapters in a book and an integer `limit` (where `limit` is the maximum number of chapters to include in the sum). The function should return the sum of the first `limit-1` chapters' page counts, but if `limit` is less than or equal to 1, the sum should be 0. Also, if the vector is empty, return 0. The function must handle cases where `limit` exceeds the vector size by summing all available elements (since we only skip the `limit`-th element if it exists, and `limit` starts counting from 1). For example, with pages `{100, 200, 300, 400}` and `limit=3`, you include the first 2 chapters (sum=300), skipping the 3rd chapter. With `limit=5` on the same vector, you include the first 4 chapters (sum=1000) because the 5th element does not exist. Ensure the function is `const`-correct and works with a `std::vector<int>`.

The problem is a straightforward aggregation with a conditional skip. The main idea is to iterate over the vector with an index (0-based) and accumulate page counts, skipping only the element whose 1-based position equals `limit`. Edge cases include: an empty vector (return 0), `limit <= 1` (skip the first element or no elements, effectively sum nothing, but since we skip the `limit`-th element, if limit=1 we skip the first element; but if vector is empty, nothing to skip), and `limit` larger than the vector size (no element is skipped, so sum all). The algorithm runs in O(n) time, where n is the size of the vector, and uses O(1) auxiliary space. A simple loop from index 0 to n-1, adding the value if `(index+1) != limit`. This naturally handles all cases without needing special checks, because when `limit` is greater than n, the condition `(index+1) != limit` is always true, so all elements are added. When `limit <= 1`, the condition `(index+1) == limit` is true only for index=0 (if limit=1) or never for limit≤0 (but limit is expected to be positive), but the function should handle limit<=1 gracefully by skipping the first element if limit=1 and returning 0 for an empty vector. We can also add an early return for empty vectors for clarity.

#include <vector>
#include <cstddef>

// Sum of all page counts except the element at 1-based position `limit`.
int sumOfFirstPages(const std::vector<int>& pages, int limit) {
    if (pages.empty()) {
        return 0;
    }
    int sum = 0;
    for (std::size_t i = 0; i < pages.size(); ++i) {
        if (static_cast<int>(i + 1) != limit) {
            sum += pages[i];
        }
    }
    return sum;
}

#include <cassert>
#include <vector>

// The solution function is declared above; here is the test runner.
int main() {
    std::vector<int> pages1 = {100, 200, 300, 400};
    assert(sumOfFirstPages(pages1, 3) == 300); // skip 3rd (index 2)
    assert(sumOfFirstPages(pages1, 1) == 900); // skip 1st, sum rest
    assert(sumOfFirstPages(pages1, 5) == 1000); // no skip, sum all
    assert(sumOfFirstPages(pages1, 2) == 700); // skip 2nd, sum 1st+3rd+4th

    std::vector<int> pages2 = {50};
    assert(sumOfFirstPages(pages2, 1) == 0); // skip the only element
    assert(sumOfFirstPages(pages2, 2) == 50); // no skip

    std::vector<int> empty;
    assert(sumOfFirstPages(empty, 10) == 0);
    assert(sumOfFirstPages(empty, 0) == 0);

    std::vector<int> pages3 = {5, 10, 15};
    assert(sumOfFirstPages(pages3, 0) == 30); // limit <=1, skip nothing (since no index equals 0 offset)
    assert(sumOfFirstPages(pages3, -3) == 30); // negative limit, skip nothing

    return 0;
}
