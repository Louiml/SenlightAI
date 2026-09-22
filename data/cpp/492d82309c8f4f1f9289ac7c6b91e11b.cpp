Write a C++ function that takes a vector of three or four double-precision floating-point values along with a corresponding vector of derivative values (of equal length), and returns the average of the values after removing the single smallest value (if given three values) or the single largest value (if given four values), along with the matching average of the derivatives for the retained values. The function must handle all possible orderings of the input, including ties, and ensure the returned value and derivative are correctly paired—i.e., if a value is removed, its corresponding derivative is also removed from the average. The input sizes are guaranteed to be exactly 3 or 4; any other size should trigger an assertion failure. The function should be const-correct and use only standard headers.

The solution requires two related operations:  
1. **For three values**: Find and exclude the smallest value and its derivative, then average the remaining two values and their derivatives.  
2. **For four values**: Find and exclude the largest value and its derivative, then average the remaining three values and their derivatives.  

The simplest robust approach is to compute the sum of all values and derivatives, then subtract the excluded element. To identify which element to exclude, we scan for the index of the minimum (for three) or maximum (for four). In case of ties (equal numeric values), any matching index yields the same numerical result, but we must ensure only one element is removed. Since we remove exactly one element (the first occurrence of the extremum), the pair is correctly handled.  

Edge cases include:  
- Ties among the extremum values: the first encountered is removed; the result is still correct because all tied values are equal numerically, and the derivative average uses the remaining derivatives.  
- All values equal: removing any one still yields the same average.  
- Exactly length 3 or 4: we pre-check and assert otherwise.  

Time complexity is O(n) for a single pass to find the extremum index, then O(1) arithmetic, so O(n) where n is 3 or 4 (constant-time for fixed size). Space complexity is O(1) beyond the input. We return a pair of doubles (or a small struct) containing the averaged value and the averaged derivative.

#include <vector>
#include <utility>
#include <cassert>

// Given a vector of values and a parallel vector of derivatives (same size),
// return a pair {avgValue, avgDerivative} where:
//   - For 3 values: remove the smallest value (and its derivative), average the rest.
//   - For 4 values: remove the largest value (and its derivative), average the rest.
// Assumes input size is exactly 3 or 4.
std::pair<double, double> averageWithoutExtremum(
    const std::vector<double>& values,
    const std::vector<double>& derivatives
) {
    const size_t n = values.size();
    assert(n == 3 || n == 4);
    assert(derivatives.size() == n);

    // Find index to remove: min for n=3, max for n=4.
    size_t removeIdx = 0;
    for (size_t i = 1; i < n; ++i) {
        if (n == 3) {
            // Remove the smallest value.
            if (values[i] < values[removeIdx]) {
                removeIdx = i;
            }
        } else { // n == 4
            // Remove the largest value.
            if (values[i] > values[removeIdx]) {
                removeIdx = i;
            }
        }
    }

    // Sum the remaining values and derivatives.
    double sumValues = 0.0;
    double sumDerivatives = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (i != removeIdx) {
            sumValues += values[i];
            sumDerivatives += derivatives[i];
        }
    }

    const size_t count = (n == 3) ? 2 : 3;
    return {sumValues / count, sumDerivatives / count};
}

#include <cassert>
#include <vector>
#include <utility>

// Declaration of the solution function (assume it's in the same translation unit)
std::pair<double, double> averageWithoutExtremum(
    const std::vector<double>& values,
    const std::vector<double>& derivatives
);

int main() {
    // Test 3 values: remove smallest (value=1, derivative=10)
    {
        std::vector<double> v = {3.0, 1.0, 2.0};
        std::vector<double> d = {30.0, 10.0, 20.0};
        auto result = averageWithoutExtremum(v, d);
        assert(result.first == 2.5);  // (3+2)/2
        assert(result.second == 25.0); // (30+20)/2
    }

    // Test 3 values with ties for smallest: remove first occurrence (value=1, derivative=10)
    {
        std::vector<double> v = {1.0, 1.0, 4.0};
        std::vector<double> d = {10.0, 999.0, 40.0};
        auto result = averageWithoutExtremum(v, d);
        assert(result.first == 2.5);  // (1+4)/2
        assert(result.second == 519.5); // (999+40)/2
    }

    // Test 4 values: remove largest (value=5, derivative=50)
    {
        std::vector<double> v = {1.0, 3.0, 2.0, 5.0};
        std::vector<double> d = {10.0, 30.0, 20.0, 50.0};
        auto result = averageWithoutExtremum(v, d);
        assert(result.first == 2.0);  // (1+3+2)/3
        assert(result.second == 20.0); // (10+30+20)/3
    }

    // Test 4 values with ties for largest: remove first occurrence (value=5, derivative=50)
    {
        std::vector<double> v = {5.0, 1.0, 2.0, 5.0};
        std::vector<double> d = {50.0, 10.0, 20.0, 999.0};
        auto result = averageWithoutExtremum(v, d);
        assert(result.first == 2.6666666666666665);  // (1+2+5)/3
        assert(result.second == 343.0); // (10+20+999)/3
    }

    // Test all equal values
    {
        std::vector<double> v = {7.0, 7.0, 7.0};
        std::vector<double> d = {1.0, 2.0, 3.0};
        auto result = averageWithoutExtremum(v, d);
        assert(result.first == 7.0);
        assert(result.second == 2.5); // (2+3)/2 after removing one entry

        std::vector<double> v4 = {7.0, 7.0, 7.0, 7.0};
        std::vector<double> d4 = {1.0, 2.0, 3.0, 4.0};
        auto result4 = averageWithoutExtremum(v4, d4);
        assert(result4.first == 7.0);
        assert(result4.second == 2.0); // (1+2+3)/3 after removing one entry
    }

    return 0;
}
