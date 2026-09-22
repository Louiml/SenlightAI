/*
Write a C++ function named `parallel_accumulate` that takes a pair of forward iterators (`first` and `last`) and an initial value `init`, and returns the sum of all elements in the range `[first, last)` added to `init`. The function must use recursive parallel decomposition via `std::async`: if the distance between iterators is less than or equal to 25, compute the sum sequentially using `std::accumulate`; otherwise, split the range at the midpoint, asynchronously compute the sum of the first half with the given `init`, recursively compute the second half with a zero initial value, and return the sum of the two results. Ensure the function is templated on iterator and value types, works with any input whose iterators support `std::distance`, `std::advance`, and dereferencing, and handles empty ranges correctly (returning `init`). Provide the function with `const` correctness where applicable, and include necessary headers. Do not include a `main` function in the solution section.
*/
#include <algorithm>
#include <future>
#include <iterator>
#include <numeric>

// Recursively sum the range [first, last) plus init using parallel decomposition.
// Splits the range at the midpoint; processes the first half asynchronously.
template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
    const unsigned long length = std::distance(first, last);
    const unsigned long max_chunk_size = 25;

    // Base case: small or empty range, compute sequentially.
    if (length <= max_chunk_size) {
        return std::accumulate(first, last, init);
    }

    // Recursive case: split at midpoint.
    Iterator mid_point = first;
    std::advance(mid_point, length / 2);

    // Asynchronously compute the first half, passing init as the starting value.
    std::future<T> first_half_result =
        std::async(parallel_accumulate<Iterator, T>, first, mid_point, init);

    // Synchronously compute the second half with a zero initial value.
    T second_half_result = parallel_accumulate(mid_point, last, T());

    // Combine the results.
    return first_half_result.get() + second_half_result;
}
#include <cassert>
#include <vector>
#include <list>
#include <numeric>
#include <algorithm>

int main() {
    // Test with a small vector (length <= 25) — sequential accumulation.
    std::vector<int> small = {1, 2, 3, 4, 5};
    assert(parallel_accumulate(small.begin(), small.end(), 0) == 15);
    assert(parallel_accumulate(small.begin(), small.end(), 100) == 115);

    // Test with a larger vector (exceeds 25) — triggers parallel split.
    std::vector<int> large(100);
    std::iota(large.begin(), large.end(), 1); // 1 to 100
    int expected = std::accumulate(large.begin(), large.end(), 0);
    assert(parallel_accumulate(large.begin(), large.end(), 0) == expected);

    // Test with an initial value on a large range.
    assert(parallel_accumulate(large.begin(), large.end(), 50) == expected + 50);

    // Test with an empty range — returns init.
    std::vector<int> empty;
    assert(parallel_accumulate(empty.begin(), empty.end(), 42) == 42);

    // Test with a list (bidirectional iterator) and doubles.
    std::list<double> dlist = {1.5, 2.5, 3.0};
    assert(parallel_accumulate(dlist.begin(), dlist.end(), 0.0) == 7.0);

    // Test with length exactly 25 (base case) and exactly 26 (recursive).
    std::vector<int> exactly25(25, 2); // all 2s
    assert(parallel_accumulate(exactly25.begin(), exactly25.end(), 0) == 50);
    std::vector<int> exactly26(26, 3); // all 3s
    assert(parallel_accumulate(exactly26.begin(), exactly26.end(), 0) == 78);

    // Test with negative numbers and large values.
    std::vector<int> mixed = {-10, 20, -5, 15};
    assert(parallel_accumulate(mixed.begin(), mixed.end(), 0) == 20);
    std::vector<long long> big(30, 1000000LL);
    assert(parallel_accumulate(big.begin(), big.end(), 5LL) == 30000005LL);

    // Test with non-zero initial value and duplicate elements.
    std::vector<int> dup = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}; // 28 fives
    assert(parallel_accumulate(dup.begin(), dup.end(), 10) == 28 * 5 + 10);
}
// The solution recursively divides the range into two halves until each subrange has at most 25 elements. For the base case, `std::accumulate` sums the elements sequentially. For the recursive case, the first half is computed asynchronously using `std::async` with the same template function, passing `init` as the initial value, while the second half is computed synchronously (or recursively) with `T()` as the initial value, then the two partial sums are added. Key edge cases: an empty range (`first == last`) is handled by `std::accumulate` returning `init` (since it processes no elements), and the recursion naturally handles odd lengths by splitting at `length/2`. The use of `T()` for the second half ensures the recursive return type is correct and avoids double-counting `init`. Time complexity is \(O(n)\) for the total work, but the async calls may overlap, giving potential parallel speedup; in the worst case without parallelism, it is still \(O(n)\). Space complexity is \(O(\log n)\) for the recursion stack and the futures’ internal state, plus temporary storage for each async call. The function must include `<algorithm>` for `std::accumulate`, `<iterator>` for `std::distance` and `std::advance`, `<future>` for `std::async` and `std::future`, and `<utility>` for `std::forward` if needed (not required here). Const correctness: the iterators are passed by value, and we do not modify the range, so they can be taken as-is; the initial value is taken by value.
