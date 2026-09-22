Write a C++ function `findStrobogrammatic(int n)` that returns a vector of all strings of length `n` that are strobogrammatic, meaning the string reads the same when rotated 180 degrees. A strobogrammatic number is composed only of digits that remain valid under 180-degree rotation: 0, 1, 8 map to themselves; 6 and 9 swap with each other. The function must generate all such strings for any positive integer `n` (e.g., for `n=1` the valid strings are "0", "1", "8"; for `n=2` they are "00", "11", "69", "88", "96"; note that for `n>=2` leading zeros are allowed in intermediate generation but the final result should include strings like "00" only if the problem statement permits them, which it does here as per the given snippet). The function must not include any leading-zero restriction except that when constructing the outermost pair, `'0'` cannot be used as the first character if `n>1` to avoid numbers with fewer than `n` digits—however, the provided snippet allows a leading zero for even `n` (e.g., "00") because it only excludes zero when the current length is exactly one less than the target half. For simplicity, follow the snippet's behavior: do not filter out leading zeros at all; instead, only avoid adding a zero at the outermost position when the current length already equals `n/2 - 1` (i.e., when the new length would exceed the half-width). The function should return an empty vector if no such strings exist for the given `n` (though for all positive integers there will be at least one, e.g., all zeros). Implement the algorithm iteratively using a stack, building strings from the middle outward, and return the result in any order.
// The solution builds strobogrammatic numbers from the center outward. For odd `n`, the center digit must be one of `'0'`, `'1'`, or `'8'` (since these are self-symmetric). For even `n`, the center is empty. We maintain a stack of current partial strings; each iteration, we pop a string, and for each pair `(left, right)` from the mapping {0→0, 1→1, 6→9, 8→8, 9→6}, we prepend `left` and append `right` to form the next longer string. We continue until the length reaches `n`. The key edge case: when adding a pair would make the string the full length (i.e., current length + 2 == n), we must not use `'0'` as the leftmost digit, because that would produce a string with a leading zero that still has length `n` (e.g., "00" for n=2 is allowed by the snippet? Actually it is allowed because the snippet only checks when `(curr.length()+2)/2 == n/2` and then skips `pair.first=='0'`; this condition is true only when `curr.length()+2 == n` and `n` is even, or `curr.length()+2 == n+1` for odd n? Let’s analyze: for odd n=3, starting with center "1", curr.length=1, (1+2)/2=1, n/2=1 (integer division), so it would skip zero, thus no leading zero. For even n=2, starting with "", curr.length=0, (0+2)/2=1, n/2=1, so it skips zero, meaning "00" is NOT generated in the snippet. But the problem statement says to include "00" for n=2? The snippet actually excludes leading zeros on the outermost pair entirely, so for n=2 only "11","69","88","96" are produced; "00" is excluded. So to match the snippet exactly, we enforce: when constructing the outermost pair (i.e., when the current string length equals n/2 - 1), we do not allow '0' as the left digit. For odd n, the center is already placed, and the outermost pair is added when the current length is n/2 - 1 as well; for n=1, the center itself can be '0'. So the rule: at any step, if the newly formed string will have length n, the leftmost character cannot be '0'. This is equivalent to the snippet's condition. Time complexity is O(5^(n/2) * n) due to generating all combinations and string concatenations; space complexity is O(5^(n/2)) for the stack and result.
#include <vector>
#include <string>
#include <stack>
#include <map>

// Return all strobogrammatic strings of length n, built with iterative expansion.
std::vector<std::string> findStrobogrammatic(int n) {
    // Mapping of a digit to its 180-degree rotation.
    const std::map<char, char> rotation = {
        {'0', '0'}, {'1', '1'}, {'6', '9'}, {'8', '8'}, {'9', '6'}
    };

    std::vector<std::string> result;
    std::stack<std::string> current;

    // Center for odd lengths: any self-symmetric digit; for even lengths: empty string.
    if (n % 2 == 1) {
        current.push("1");
        current.push("8");
        current.push("0");
    } else {
        current.push("");
    }

    while (!current.empty()) {
        // If all strings have reached the desired length, collect and return.
        if (current.top().size() / 2 == n / 2) {
            while (!current.empty()) {
                result.push_back(current.top());
                current.pop();
            }
            return result;
        }

        std::stack<std::string> next;
        while (!current.empty()) {
            std::string middle = current.top();
            current.pop();

            for (const auto& pair : rotation) {
                // Avoid leading zero on the final outer pair.
                if (middle.size() + 2 == n && pair.first == '0') {
                    continue;
                }
                next.push(pair.first + middle + pair.second);
            }
        }
        current.swap(next);
    }
    return {};
}
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    // Helper to check if a set of strings matches exactly (order independent).
    auto check = [](int n, std::vector<std::string> expected) {
        auto got = findStrobogrammatic(n);
        std::sort(got.begin(), got.end());
        std::sort(expected.begin(), expected.end());
        assert(got == expected);
    };

    check(1, {"0", "1", "8"});
    check(2, {"11", "69", "88", "96"}); // No "00" due to leading-zero rule.
    check(3, {"101", "111", "181", "609", "619", "689", "808", "818", "888", "906", "916", "986"});
    check(4, {"1001", "1111", "1691", "1881", "6009", "6119", "6699", "6889", "8008", "8118", "8698", "8888", "9006", "9116", "9696", "9886"});
    
    // Single-digit edge case already covered; test for n=0? Not required but ensure no crash.
    // The problem assumes n >= 1.
    assert(findStrobogrammatic(2).size() == 4);
    assert(findStrobogrammatic(3).size() == 12);
    return 0;
}
