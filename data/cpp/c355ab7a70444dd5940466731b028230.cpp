// Write a standalone C++ function named `stableSortWithMetrics` that accepts a pointer to an integer array and its size, creates a deep copy of the original array, performs **insertion sort** on the copy (not the original), and returns a `std::pair<long long, long long>` where the first element is the number of comparisons performed (i.e., how many times the inner while-loop condition `array1[j] > temp` is evaluated) and the second element is the number of assignments made to array elements (each swap or shift counts as one assignment). The original array must remain unmodified. If the size is 0 or negative, return `{0, 0}` and do nothing. The function must use raw pointers (no `std::vector`) internally, and must free all dynamically allocated memory before returning. Assume the array contains valid integers, possibly with duplicates and negative values. The function should be `const`-correct with respect to the input, meaning it must accept the input array as `const int*` and only read from it.

#include <cassert>
#include <utility>

// Declare the function from the solution
std::pair<long long, long long> stableSortWithMetrics(const int* arr, int count);

int main() {
    // Empty array
    int empty[] = {};
    assert(stableSortWithMetrics(empty, 0) == std::make_pair(0LL, 0LL));

    // Null pointer
    assert(stableSortWithMetrics(nullptr, 5) == std::make_pair(0LL, 0LL));

    // Single element: one while condition check, one placement assignment
    int single[] = {42};
    auto res1 = stableSortWithMetrics(single, 1);
    assert(res1.first == 1);
    assert(res1.second == 1);
    assert(single[0] == 42); // original unchanged

    // Sorted ascending: n-1 while checks (each false immediately)
    int asc[] = {1, 2, 3, 4};
    auto res2 = stableSortWithMetrics(asc, 4);
    // Outer loop i=0: check once (j=-1), assign once (place temp)
    // i=1: check once (1>2? false), assign once
    // i=2: check once (2>3? false), assign once
    // i=3: check once (3>4? false), assign once
    // Total comparisons = 4, assignments = 4
    assert(res2.first == 4);
    assert(res2.second == 4);
    assert(asc[0] == 1 && asc[3] == 4);

    // Descending: worst case for comparisons and shifts
    int desc[] = {4, 3, 2, 1};
    auto res3 = stableSortWithMetrics(desc, 4);
    // Manually compute: 
    // i=0: comparisons=1, assignments=1
    // i=1: j=0, check 4>3 true, shift, j=-1, check j>=0 false -> total comparisons 2, assignments 1+1+1=3? Actually:
    // For i=1: while check1: j=0, 4>3? true, shift (assign1), j=-1, check2: j>=0? false, loop ends. Place temp assign. So comps=2, assigns=2 (shift + place)
    // i=2: j=1: check1 (3>2? true), shift assign, j=0, check2 (4>2? true), shift assign, j=-1, check3 false. Place assign. comps=3, assigns=3
    // i=3: j=2: check1 (2>1? true), shift, j=1, check2 (3>1? true), shift, j=0, check3 (4>1? true), shift, j=-1, check4 false. Place assign. comps=4, assigns=4
    // Total comps=1+2+3+4=10, assigns=1+2+3+4=10
    assert(res3.first == 10);
    assert(res3.second == 10);
    assert(desc[0] == 1 && desc[3] == 4);

    // Duplicates: multiple shifts but stable
    int dup[] = {2, 1, 2, 1};
    auto res4 = stableSortWithMetrics(dup, 4);
    // Counts manually:
    // i=0: comp1, assign1 -> {1,1}
    // i=1: j=0, check (2>1? true), shift assign, j=-1, check false, place assign -> comps 2, assigns 2
    // i=2: j=1, check (1>2? false), place assign -> comps 1, assigns 1
    // i=3: j=2, check (2>1? true), shift assign, j=1, check (1>1? false), place assign -> comps 2, assigns 2
    // Total comps=1+2+1+2=6, assigns=1+2+1+2=6
    assert(res4.first == 6);
    assert(res4.second == 6);
    // Check sorted order
    assert(dup[0] == 1 && dup[1] == 1 && dup[2] == 2 && dup[3] == 2);

    // Negative numbers
    int neg[] = {-5, -1, -10};
    auto res5 = stableSortWithMetrics(neg, 3);
    assert(res5.first == 3); // i=0:1, i=1:1, i=2:1 (check -5 > -1? false) -> total 3
    assert(res5.second == 3); // each i just places temp
    assert(neg[0] == -10 && neg[1] == -5 && neg[2] == -1);

    return 0;
}

#include <utility>   // for std::pair
#include <cstddef>   // for size_t

// Performs insertion sort on a copy of the input array and returns
// {comparison_count, assignment_count}. Original array remains unchanged.
// Handles null or non-positive size by returning {0,0}.
std::pair<long long, long long> stableSortWithMetrics(const int* arr, int count) {
    if (arr == nullptr || count <= 0) {
        return {0, 0};
    }

    // Deep copy the input array
    int* copy = new int[count];
    for (int i = 0; i < count; ++i) {
        copy[i] = arr[i];
    }

    long long comparisons = 0;
    long long assignments = 0;

    // Insertion sort on the copy
    for (int i = 0; i < count; ++i) {
        int j = i - 1;
        int temp = copy[i];

        // Count the first evaluation of the while condition
        ++comparisons;
        while (j >= 0 && copy[j] > temp) {
            copy[j + 1] = copy[j];
            ++assignments;
            --j;
            ++comparisons; // each time we re-evaluate the condition after decrementing j
        }

        // Place temp in its correct position
        copy[j + 1] = temp;
        ++assignments;
    }

    delete[] copy;
    return {comparisons, assignments};
}

// The main algorithm is to simulate insertion sort exactly as in the provided snippet, but on a copied array so the caller's data is untouched. We must count every evaluation of the inner while condition, even if the condition is false immediately (because the condition is checked). For each iteration of the outer loop, we start with `j = i-1` and `temp = array1[i]`. The while loop condition `array1[j] > temp && j >= 0` is evaluated; note that due to C++ short-circuit evaluation, if `j` is negative, we still count the evaluation as one, but we must avoid accessing `array1[-1]`. However, in the original code, the condition writes `array1[j]` before checking `j >= 0`, which is a bug. In our corrected version, we must check `j >= 0` first to avoid out-of-bounds access, but still count the comparison as one. For each successful shift (when `array1[j] > temp` and `j >= 0`), we perform one assignment `array1[j+1] = array1[j]`. After the loop, we perform one final assignment `array1[j+1] = temp`. The number of assignments is exactly the number of shifts plus one per outer loop iteration (for placing temp). Edge cases: empty array, single element, all duplicates (while condition true for many shifts), and descending order (worst-case). Time complexity is O(n^2) worst-case and average, O(n) best-case on already sorted input (but still we count all n-1 while-condition checks). Space complexity is O(n) for the copied array, plus O(1) auxiliary.
