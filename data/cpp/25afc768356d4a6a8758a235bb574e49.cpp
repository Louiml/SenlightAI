Given a positive integer `n` (1 ≤ n ≤ 20), write a C++ function `generateAllBinaryPalindromes` that returns a `vector<string>` containing all binary strings of length exactly `n` that are palindromes. The strings must be constructed using only characters `'0'` and `'1'`, and the output list should be sorted in lexicographical (dictionary) order. For example, for `n = 3`, the function should return `{"000", "010", "101", "111"}`. Ensure the function handles both even and odd values of `n` correctly, and that all generated strings are unique and valid.

#include <cassert>
#include <string>
#include <vector>

// Prototype of the solution function (declared here for the test).
std::vector<std::string> generateAllBinaryPalindromes(int n);

int main() {
    // Test basic cases
    auto res1 = generateAllBinaryPalindromes(1);
    assert(res1 == std::vector<std::string>({"0", "1"}));

    auto res2 = generateAllBinaryPalindromes(2);
    assert(res2 == std::vector<std::string>({"00", "11"}));

    auto res3 = generateAllBinaryPalindromes(3);
    assert(res3 == std::vector<std::string>({"000", "010", "101", "111"}));

    auto res4 = generateAllBinaryPalindromes(4);
    assert(res4 == std::vector<std::string>({"0000", "0110", "1001", "1111"}));

    // Test larger n (odd and even)
    auto res5 = generateAllBinaryPalindromes(5);
    assert(res5.size() == 8);  // 2^3 = 8
    assert(res5.front() == "00000");
    assert(res5.back() == "11111");
    // Ensure sorted order
    for (size_t i = 1; i < res5.size(); ++i) {
        assert(res5[i - 1] < res5[i]);
    }

    auto res6 = generateAllBinaryPalindromes(6);
    assert(res6.size() == 8);  // 2^3 = 8
    for (size_t i = 1; i < res6.size(); ++i) {
        assert(res6[i - 1] < res6[i]);
    }

    // Test all strings are palindromes
    for (const auto& s : res6) {
        std::string rev(s.rbegin(), s.rend());
        assert(s == rev);
    }

    return 0;
}

#include <string>
#include <vector>

// Generate all binary palindromes of exactly length n, sorted lexicographically.
// n must be >= 1.
std::vector<std::string> generateAllBinaryPalindromes(int n) {
    std::vector<std::string> result;
    const int half = (n + 1) / 2;  // number of characters to choose

    // Recursive lambda to build the first half.
    // When half is built, construct the full palindrome.
    std::function<void(std::string&)> build = [&](std::string& prefix) {
        if (static_cast<int>(prefix.size()) == half) {
            std::string palindrome = prefix;
            // Mirror the appropriate part: for odd n, skip the middle character.
            for (int i = half - 1 - (n % 2); i >= 0; --i) {
                palindrome.push_back(prefix[i]);
            }
            result.push_back(std::move(palindrome));
            return;
        }
        prefix.push_back('0');
        build(prefix);
        prefix.back() = '1';
        build(prefix);
        prefix.pop_back();
    };

    std::string start;
    build(start);
    return result;
}

// The key insight is that a palindrome of length `n` is completely determined by its first `ceil(n/2)` characters. The remaining characters are forced by the palindrome condition: the `i`-th character (0-indexed) must equal the character at position `n-1-i`. Therefore, we only need to generate all possible binary combinations of length `m = (n+1)/2` (using integer division: `(n+1)/2` works for both even and odd), and for each combination, construct the full string by mirroring. For `n` odd, the middle character is shared, so we only mirror the first `n/2` characters (integer division) onto the end. For even, we mirror all `m` characters. This approach avoids generating and testing all `2^n` possible strings, reducing the search space to `2^m` which is at most `2^10` for `n ≤ 20`. The simplest implementation is a recursive helper that builds the first half, then when it reaches length `m`, constructs the palindrome and adds it to the result. Alternatively, we can use an iterative bitmask from `0` to `2^m - 1`. The lexicographical order is naturally obtained by enumerating combinations in binary order (with `'0'` treated as smaller than `'1'`). Time complexity is `O(2^(n/2) * n)` for generating and copying strings; space complexity is `O(2^(n/2) * n)` for the output, plus `O(n)` recursion depth.
