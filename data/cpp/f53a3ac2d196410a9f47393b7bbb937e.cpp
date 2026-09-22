// Write a C++ function named `averageExcludingExtremes` that takes a `std::vector<int>` of salary values (each between 1000 and 1,000,000, as per the original problem constraints) and returns the average salary after removing the single minimum and single maximum salary. The function should exclude exactly one occurrence of the smallest and one occurrence of the largest value, even if there are duplicates. The input vector is guaranteed to have at least 3 elements. Return the result as a `double`. The function must not modify the input vector (apply `const` correctness), and you may use any standard library functions.

#include <cassert>
#include <vector>

int main() {
    std::vector<int> s1 = {4000, 3000, 1000, 2000};
    assert(averageExcludingExtremes(s1) == 2500.0);  // (3000+2000)/2

    std::vector<int> s2 = {1000, 2000, 3000};
    assert(averageExcludingExtremes(s2) == 2000.0);

    std::vector<int> s3 = {8000, 9000, 2000, 3000, 6000, 1000};
    assert(averageExcludingExtremes(s3) == 6500.0);  // (8000+9000+3000+6000)/4

    std::vector<int> s4 = {1000, 1000, 1000};
    assert(averageExcludingExtremes(s4) == 1000.0);

    std::vector<int> s5 = {1000, 1000, 2000, 2000};
    assert(averageExcludingExtremes(s5) == 1500.0);  // remove one 1000 and one 2000, left with 1000 and 2000

    std::vector<int> s6 = {5000, 5000, 5000, 5000, 5000};
    assert(averageExcludingExtremes(s6) == 5000.0);

    std::vector<int> s7 = {1000000, 1000, 1000, 1000000};
    assert(averageExcludingExtremes(s7) == 500500.0); // (1000+1000000)/2

    std::vector<int> s8 = {1000, 2000, 3000, 4000, 5000};
    assert(averageExcludingExtremes(s8) == 3000.0);  // (2000+3000+4000)/3

    std::vector<int> s9 = {1000, 1000, 2000, 3000, 3000};
    assert(averageExcludingExtremes(s9) == 2000.0);  // (1000+2000+3000)/3

    std::vector<int> s10 = {9000, 8000, 7000, 6000, 5000};
    assert(averageExcludingExtremes(s10) == 7000.0); // (8000+7000+6000)/3

    return 0;
}

#include <vector>
#include <algorithm>
#include <numeric>

// Returns the average salary after removing one minimum and one maximum value.
// Input: vector of at least 3 salaries. Does not modify the input.
double averageExcludingExtremes(const std::vector<int>& salary) {
    int minSalary = *std::min_element(salary.begin(), salary.end());
    int maxSalary = *std::max_element(salary.begin(), salary.end());

    long long total = std::accumulate(salary.begin(), salary.end(), 0LL);
    total -= minSalary;
    total -= maxSalary;

    return static_cast<double>(total) / (salary.size() - 2);
}

// The core idea is to compute the sum of all salaries, then subtract the minimum and maximum values, and finally divide by the number of elements minus two. The simplest approach is to use `std::min_element` and `std::max_element` to find the smallest and largest salaries, then sum all elements while skipping one occurrence of each extreme. However, if there are duplicate extremes, we must skip only one of each. A straightforward method is to compute the total sum of all elements, then subtract the min and max values. This automatically removes exactly one occurrence of each extreme because subtracting the numeric value removes one instance (the sum includes all occurrences, and subtracting the value removes one occurrence). Then divide by `salary.size() - 2` to get the average. Edge cases: an input of size 3 yields exactly one middle value. Duplicate minimums or maximums are handled correctly because we subtract the value once, not all occurrences. Time complexity is O(n) for the sum plus O(n) for finding min and max, so overall O(n). Space complexity is O(1) auxiliary, ignoring input storage.
