Given a binary string `A` consisting only of characters `'0'` and `'1'`, write a C++ function `flipSubstring` that returns a vector of two integers representing the 1‑based indices `[L, R]` of the smallest interval that, when flipped (i.e., every `'0'` becomes `'1'` and every `'1'` becomes `'0'`), maximizes the total number of `'1'`s in the resulting string. If no such flip increases the number of `'1'`s compared to the original string, return an empty vector. If multiple intervals achieve the same maximum gain, choose the one with the smallest starting index; if ties persist, choose the shortest interval (smallest length). The input string is non-empty. The function must be self-contained, use only standard C++ libraries, and operate in linear time.
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared here (or included from the solution above).
// For the test, we repeat the function definition in a single file.
std::vector<int> flipSubstring(const std::string& A);

int main() {
    // Basic cases
    assert(flipSubstring("0") == std::vector<int>({1, 1}));          // flip the single zero
    assert(flipSubstring("1") == std::vector<int>());                // no improvement
    assert(flipSubstring("01") == std::vector<int>({1, 1}));         // flip index 1 gives "11"
    assert(flipSubstring("10") == std::vector<int>({2, 2}));         // flip index 2 gives "11"
    assert(flipSubstring("000") == std::vector<int>({1, 3}));        // flip whole string

    // Tie-breaking: same max sum, smallest left index first
    assert(flipSubstring("0011") == std::vector<int>({1, 1}));       // subarrays [1,1] or [2,2] both sum 1, choose leftmost
    assert(flipSubstring("1100") == std::vector<int>({3, 3}));       // only improvement is a zero alone

    // Multiple zeros in a row
    assert(flipSubstring("1001") == std::vector<int>({2, 3}));       // flip "00" gives "1111"
    assert(flipSubstring("10101") == std::vector<int>({2, 2}));      // only one zero; ties with others but leftmost

    // No zeros at all
    assert(flipSubstring("111") == std::vector<int>());

    // Mixed with equal sums and tie-breaking by shortest length
    // "010" -> transformed: [-1,1,-1]; max subarray of sum 1 at index 1 only -> [2,2]
    assert(flipSubstring("010") == std::vector<int>({2, 2}));
    // "00100" -> transformed: [1,1,-1,1,1]; max sum 2 can be [0,1] or [3,4]; both length 2, choose leftmost [0,1] -> [1,2]
    assert(flipSubstring("00100") == std::vector<int>({1, 2}));

    return 0;
}
#include <vector>
#include <string>

// Given a binary string, return the 1-based [L, R] interval that, when flipped,
// maximizes the number of '1's. If no improvement is possible, return an empty vector.
std::vector<int> flipSubstring(const std::string& A) {
    const int n = static_cast<int>(A.size());
    int bestSum = 0;          // maximum positive subarray sum found so far
    int currentSum = 0;       // running sum for current subarray
    int bestL = 0, bestR = -1; // best interval in 0-based indices (empty initially)
    int left = 0;             // tentative left boundary of current subarray

    for (int right = 0; right < n; ++right) {
        // Transform: '0' -> +1, '1' -> -1
        int value = (A[right] == '0') ? 1 : -1;
        currentSum += value;

        if (currentSum < 0) {
            // A negative sum can only decrease the total, so reset and move left.
            left = right + 1;
            currentSum = 0;
        } else if (currentSum > bestSum) {
            // New maximum found.
            bestSum = currentSum;
            bestL = left;
            bestR = right;
        } else if (currentSum == bestSum && bestSum > 0) {
            // Tie-breaking: prefer smaller left index; if same left, shorter length.
            if (left < bestL || (left == bestL && (right - left) < (bestR - bestL))) {
                bestL = left;
                bestR = right;
            }
        }
    }

    if (bestSum <= 0) {
        return {};  // no beneficial flip exists
    }
    return {bestL + 1, bestR + 1};  // convert to 1-based indices
}
// The problem reduces to finding a contiguous subarray with the maximum sum in a transformed integer array where each original `'0'` is mapped to `+1` (because flipping it increases the count of `'1'`s by one) and each original `'1'` is mapped to `-1` (because flipping it decreases the count by one). Any flip of a subarray changes the total number of `'1'`s by the sum of that transformed segment. Therefore, we need the subarray with the maximum positive sum. If the maximum sum is `<= 0`, flipping any segment cannot improve the string, so return an empty vector.
//
// The standard Kadane’s algorithm is applied: iterate through the array while keeping a running sum (`currentSum`). If `currentSum` becomes negative, reset it to zero and move the tentative left boundary to the next index, because any subarray that includes a negative prefix cannot be optimal. When `currentSum` exceeds the best sum found so far, update the best left and right endpoints. To handle tie‑breaking correctly with the smallest starting index and then shortest length, we must maintain the best interval according to those rules. Specifically, when `currentSum` equals the best sum, we do not update unless the new interval has a smaller left index, or if the left index is the same, a smaller length. After the scan, if `bestSum <= 0`, return an empty vector; otherwise return the 1‑based `[L+1, R+1]`.
//
// Edge cases:
// - The string contains only `'1'`s: all transformed values are `-1`, max sum remains `<= 0`, return empty.
// - The string contains only `'0'`s: the entire string yields the maximum positive sum, return `[1, n]`.
// - Ties: e.g., `"101"` transforms to `[-1,1,-1]`. The best subarray is just index 1 (0‑based) with sum 1, but there are two potential intervals of length 1 (index 1 only? actually only one). For `"0011"` → `[1,1,-1,-1]`, max subarray could be `[0,0]` (sum 1) or `[1,1]` (sum 1). The tie‑break chooses the smaller left index, so `[1,1]` (1‑based) is selected.
//
// Time complexity is O(n) because we traverse the string once, and space complexity is O(n) for the transformed vector (which can be avoided by computing on the fly, but it is acceptable to store it for clarity). Alternatively, we can compute the transformation directly in the loop without storing an array, reducing auxiliary space to O(1), but the provided reference solution stores it for readability. We will present a version that avoids the extra vector to be more efficient, but the core logic remains Kadane's algorithm.
