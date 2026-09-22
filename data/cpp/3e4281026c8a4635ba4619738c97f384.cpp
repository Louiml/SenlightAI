You are given an array of `n` positive integers and a target integer `k`. Write a C++ function named `findMinimumQuotient` that takes as input a `std::vector<int>` (containing the array values), the target `k`, and returns the smallest possible value of `k / a[i]` over all indices `i` where `a[i]` is a divisor of `k` (i.e., `k % a[i] == 0`). If no such divisor exists in the array, return `-1`. The function must handle cases where the array might be empty, and it must not assume the array is sorted. The input numbers and `k` are all positive and fit within the range of a 32-bit signed integer.
#include <cassert>
#include <vector>

int findMinimumQuotient(const std::vector<int>& a, int k);

int main() {
    // Basic case: divisors exist
    std::vector<int> v1 = {2, 4, 5};
    assert(findMinimumQuotient(v1, 20) == 4); // 20/5=4, 20/4=5, 20/2=10

    // Multiple divisors, choose smallest quotient
    std::vector<int> v2 = {1, 3, 7};
    assert(findMinimumQuotient(v2, 21) == 3); // 21/7=3 is smallest

    // No divisors
    std::vector<int> v3 = {2, 3, 4};
    assert(findMinimumQuotient(v3, 7) == -1);

    // Empty vector
    std::vector<int> v4;
    assert(findMinimumQuotient(v4, 10) == -1);

    // Duplicate values
    std::vector<int> v5 = {2, 2, 5};
    assert(findMinimumQuotient(v5, 10) == 2); // 10/5=2

    // k is exactly one of the elements
    std::vector<int> v6 = {7, 3};
    assert(findMinimumQuotient(v6, 7) == 1); // 7/7=1

    // Larger array with all divisors
    std::vector<int> v7 = {2, 3, 4, 6, 12, 24};
    assert(findMinimumQuotient(v7, 24) == 1); // 24/24=1

    // k is smaller than some elements but still divisible by others
    std::vector<int> v8 = {2, 9, 20};
    assert(findMinimumQuotient(v8, 18) == 2); // 18/9=2

    // All elements are multiples of k? Actually k must be divisible by them
    std::vector<int> v9 = {1, 2, 3};
    assert(findMinimumQuotient(v9, 6) == 2); // 6/3=2

    // Single element that divides
    std::vector<int> v10 = {5};
    assert(findMinimumQuotient(v10, 5) == 1);
}
#include <vector>
#include <climits>

// Finds the minimum quotient k / a[i] for indices where a[i] divides k.
// Returns -1 if no divisor is found or the vector is empty.
int findMinimumQuotient(const std::vector<int>& a, int k) {
    int best = -1;
    for (int value : a) {
        if (value > 0 && k % value == 0) {
            int quotient = k / value;
            if (best == -1 || quotient < best) {
                best = quotient;
            }
        }
    }
    return best;
}
// The task is a straightforward linear scan. For each element in the input vector, check if it evenly divides `k` (i.e., `k % value == 0`). If yes, compute the quotient `k / value` and keep the minimum such quotient. Initialize the answer to a sentinel value like `-1` or a very large number; after scanning all elements, if no divisor was found, return `-1`, otherwise return the minimum quotient. Edge cases include: the array is empty (return `-1`), the target `k` is smaller than some array elements (then those elements cannot be divisors unless `k` itself is zero, which is not allowed since positive), and duplicate divisors (no special handling needed). Time complexity is O(n) since we visit each element once; auxiliary space is O(1) beyond the input vector.
