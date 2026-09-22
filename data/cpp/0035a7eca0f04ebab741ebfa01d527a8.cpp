/*
Write a C++ function `firstTenDigitsOfLargeSum` that takes a `std::vector<std::string>&` containing non-negative integer strings (each consisting only of digits, with no leading zeros except for the number "0" itself), and returns a string containing the first 10 digits of the exact sum of all those integers. The integers can be arbitrarily large (far beyond 64‑bit range), so the sum must be computed using string-based addition. The function must not use any arbitrary-precision library (like `boost::multiprecision`), only standard C++ string manipulation. If the total sum has fewer than 10 digits, return the entire sum as a string (do not pad with leading zeros). The input vector will contain at least one element. Example: for input `{"12345678901234567890", "98765432109876543210"}`, the exact sum is `111111111011111111100`, so the function returns `"1111111110"` (the first 10 digits).
*/

#include <string>
#include <vector>

// Helper: add two non-negative integer strings and return the sum as a string.
std::string addStrings(const std::string& a, const std::string& b) {
    std::string result;
    int carry = 0;
    int i = static_cast<int>(a.size()) - 1;
    int j = static_cast<int>(b.size()) - 1;

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i] - '0';
        if (j >= 0) sum += b[j] - '0';
        result.push_back(static_cast<char>('0' + sum % 10));
        carry = sum / 10;
        --i;
        --j;
    }

    // The result is built reversed; reverse it to get the correct order.
    std::reverse(result.begin(), result.end());
    return result;
}

// Return the first 10 digits of the exact sum of all integer strings in nums.
std::string firstTenDigitsOfLargeSum(const std::vector<std::string>& nums) {
    std::string sum = "0";
    for (const std::string& num : nums) {
        sum = addStrings(sum, num);
    }
    if (sum.size() <= 10) {
        return sum;
    }
    return sum.substr(0, 10);
}

#include <cassert>
#include <string>
#include <vector>

// (The solution function is assumed to be declared/defined above.)

int main() {
    // Single small number
    assert(firstTenDigitsOfLargeSum({"0"}) == "0");
    assert(firstTenDigitsOfLargeSum({"12345"}) == "12345");

    // Two numbers that overflow 64-bit
    std::vector<std::string> v1 = {"12345678901234567890", "98765432109876543210"};
    assert(firstTenDigitsOfLargeSum(v1) == "1111111110");

    // Sum with exactly 10 digits
    std::vector<std::string> v2 = {"9999999999", "1"};
    assert(firstTenDigitsOfLargeSum(v2) == "10000000000"); // 11 digits, first 10 are "1000000000"
    std::vector<std::string> v3 = {"1000000000", "1"};
    assert(firstTenDigitsOfLargeSum(v3) == "1000000001");

    // Many numbers
    std::vector<std::string> v4 = {"5", "5", "5", "5"};
    assert(firstTenDigitsOfLargeSum(v4) == "20");

    // Large numbers with different lengths
    std::vector<std::string> v5 = {"999", "1", "1000"};
    assert(firstTenDigitsOfLargeSum(v5) == "2000");

    // All zeros
    std::vector<std::string> v6 = {"0", "0", "000"};
    // Note: "000" is technically invalid per spec, but addition handles it fine.
    assert(firstTenDigitsOfLargeSum(v6) == "0");

    // Single huge number
    std::vector<std::string> v7 = {"123456789012345678901234567890"};
    assert(firstTenDigitsOfLargeSum(v7) == "1234567890");

    return 0;
}

// The solution is to accumulate the sum using a helper that adds two decimal strings digit‑by‑digit from right to left, exactly like pen‑and‑paper addition. The main function initializes the result string to `"0"` and then for each input number, calls the helper to add the current accumulated sum with that number. The helper maintains a carry, processes each digit from the least significant position, and prepends each computed digit to the result string. After processing all inputs, the accumulated sum is the exact total. Then, simply take a substring of the first 10 characters (or the whole string if its length ≤ 10). Edge cases: (1) numbers may have different lengths – the loop continues until both indices are exhausted and carry is zero; (2) the input may consist only of `"0"` – then the sum remains `"0"` and the output is `"0"` (since it's shorter than 10 digits); (3) the sum may be exactly 10 digits or fewer – in that case, return the whole sum; (4) there could be many numbers, but the addition remains O(total number of digits). Time complexity: O(total number of digits across all inputs), because each addition step processes each digit of the numbers involved. Space complexity: O(max length of numbers) for the temporary result in the helper, and O(length of final sum) for the final string.
