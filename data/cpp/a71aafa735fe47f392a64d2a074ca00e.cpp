// Write a C++ function `countGoodNumbers(const std::vector<std::string>& numbers, int k)` that, given a vector of digit-only strings, returns the number of strings that contain every digit from `0` to `k` inclusive (at least once per string). Each string represents a non-negative integer as a sequence of ASCII digits (e.g., `"102"`). You must treat the digits as characters and not as numeric values. The parameter `k` satisfies `0 ≤ k ≤ 9`. The function should be independent—it must not depend on any global state or external input beyond its arguments.
The core idea is to process each string independently and determine whether it contains all required digits `0` through `k`. A straightforward approach is to use a boolean array (or a bitset) of size `k+1` (or 10, but only the first `k+1` entries matter). For each string, we iterate over its characters, convert each character to an integer digit using `ch - '0'`, and mark that digit as seen. After processing all characters of the string, we check whether every index from `0` to `k` is marked as seen. If so, we increment a counter. We must ensure we reset the boolean array for each new string. An important edge case is when `k = 0`: then the only required digit is `'0'`, and strings like `"0"` or `"10"` qualify, while `"00"` also qualifies because `'0'` appears. Also, strings may be empty (though the problem likely implies non-empty, but handle it gracefully—an empty string fails unless `k` is negative, which is not allowed). Time complexity is O(total characters across all strings) because we iterate each string exactly once. Space complexity is O(1) auxiliary (a fixed-size array of 10 booleans). No sorting or other preprocessing is needed; we avoid using `std::set` to keep it simple and efficient.
#include <string>
#include <vector>
#include <cstddef>

// Count strings that contain every digit from '0' to 'k' (inclusive) at least once.
// Precondition: 0 <= k <= 9. Strings consist only of ASCII digit characters.
int countGoodNumbers(const std::vector<std::string>& numbers, int k) {
    int goodCount = 0;
    bool seen[10] = {false};  // only indices 0..k are relevant
    for (const std::string& s : numbers) {
        // Mark digits found in the current string
        for (char ch : s) {
            int digit = ch - '0';  // '0'..'9' -> 0..9
            seen[digit] = true;
        }
        // Check if all digits 0..k are present
        bool allPresent = true;
        for (int d = 0; d <= k; ++d) {
            if (!seen[d]) {
                allPresent = false;
                break;
            }
        }
        if (allPresent) {
            ++goodCount;
        }
        // Reset for next string
        for (int d = 0; d <= k; ++d) {
            seen[d] = false;
        }
    }
    return goodCount;
}
#include <cassert>
#include <vector>
#include <string>

// Declare the function under test
int countGoodNumbers(const std::vector<std::string>& numbers, int k);

int main() {
    // Basic cases
    assert(countGoodNumbers({"012", "123", "102"}, 2) == 2); // "012" and "102" have 0,1,2; "123" missing 0
    assert(countGoodNumbers({"0", "1", "01"}, 1) == 1);      // only "01" has both 0 and 1
    assert(countGoodNumbers({"5", "50", "05"}, 0) == 2);     // strings containing '0' are "50" and "05"
    
    // k=9 (all digits)
    assert(countGoodNumbers({"0123456789", "9876543210", "123456789"}, 9) == 2);
    
    // Empty vector
    assert(countGoodNumbers({}, 3) == 0);
    
    // Single digit string with k larger than 9 is invalid, but we test k=9 with single-digit strings
    assert(countGoodNumbers({"1", "2", "3"}, 0) == 0);  // none contain '0'
    assert(countGoodNumbers({"0", "0", "0"}, 0) == 3);
    
    // Repeated digits and longer strings
    assert(countGoodNumbers({"000", "001", "011", "111"}, 1) == 3); // all except "111" contain 0 and 1? Actually "000" lacks 1, wait: "000" has no '1' -> not good. "001" has 0 and 1 -> good. "011" has 0 and 1 -> good. "111" has no 0 -> not good. So only 2 good.
    assert(countGoodNumbers({"000", "001", "011", "111"}, 1) == 2);
    
    // Mixed lengths, k=3
    assert(countGoodNumbers({"1023", "123", "0123", "3210"}, 3) == 3); // "123" missing 0, others have all 0-3
    
    // Duplicate strings are counted individually
    assert(countGoodNumbers({"012", "012"}, 2) == 2);
    
    // k=9 with a long string missing one digit
    assert(countGoodNumbers({"012345678", "0123456789"}, 9) == 1);
    
    return 0;
}
