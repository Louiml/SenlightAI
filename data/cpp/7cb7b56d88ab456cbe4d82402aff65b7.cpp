Write a C++ function named `generateStrobogrammatic` that takes a non-negative integer `n` and returns a vector of strings containing all strobogrammatic numbers of length `n` in lexicographic order. A strobogrammatic number is one that reads the same when rotated 180 degrees; valid digit pairs are (0,0), (1,1), (8,8), (6,9), and (9,6). Numbers cannot have leading zeros unless the length is exactly 1 (so "0" is valid, but "00", "01", etc., are not). The function must be `const`-correct and use a depth-first search (DFS) to build the numbers from the outer digits inward. For odd `n`, the middle digit must be one of {0,1,8}. The returned vector should be sorted lexicographically (which naturally arises from the order of digit insertion). Handle edge cases: `n == 0` should return an empty vector (there are no length-0 strobogrammatic numbers), and `n == 1` should return `{"0","1","8"}`.
The solution uses a recursive DFS that fills the string from both ends simultaneously. We define a helper that places valid pairs at symmetric positions. The recursion depth is `n/2` (integer division). At each step, we try the four valid pairs for the outer positions: (1,1), (6,9), (8,8), (9,6). We also allow (0,0) only when we are not at the very first pair (i.e., `cur != 0`) to avoid leading zeros. When the recursion reaches the middle (`cur == n/2`), if `n` is even, we push the completed string; if `n` is odd, we insert each of the three possible middle digits (0,1,8) at position `n/2` and push each version. The order of insertion naturally yields lexicographic order because we try pairs in ascending order and append to the vector in that order. Time complexity is `O(4^(n/2) * n)` due to generating each candidate string of length `n`; space complexity is `O(n)` for recursion depth plus `O(4^(n/2) * n)` for the output storage.
#include <vector>
#include <string>

// Generate all strobogrammatic numbers of length n in lexicographic order.
std::vector<std::string> generateStrobogrammatic(int n) {
    if (n < 0) return {};
    std::vector<std::string> result;
    if (n == 0) return result;

    std::string str(n, ' ');
    const char pairs[4][2] = {{'1','1'}, {'6','9'}, {'8','8'}, {'9','6'}};
    const char middle[3] = {'0','1','8'};

    // Recursively fill positions from outside inward.
    auto dfs = [&](auto&& self, int cur) -> void {
        if (cur == n / 2) {
            if (n % 2 == 0) {
                result.push_back(str);
            } else {
                for (char c : middle) {
                    str[n / 2] = c;
                    result.push_back(str);
                }
            }
            return;
        }
        // Try valid non-zero pairs for the outermost positions.
        for (const auto& p : pairs) {
            str[cur] = p[0];
            str[n - cur - 1] = p[1];
            self(self, cur + 1);
        }
        // Allow (0,0) only if not at the very first pair.
        if (cur != 0) {
            str[cur] = '0';
            str[n - cur - 1] = '0';
            self(self, cur + 1);
        }
    };

    dfs(dfs, 0);
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or link it).

int main() {
    // Basic cases
    assert(generateStrobogrammatic(0).empty());
    assert(generateStrobogrammatic(1) == std::vector<std::string>({"0","1","8"}));
    assert(generateStrobogrammatic(2) == std::vector<std::string>({"11","69","88","96"}));
    assert(generateStrobogrammatic(3) == std::vector<std::string>({"101","111","181","609","619","689","808","818","888","906","916","986"}));

    // Check lexicographic order for n=4 (small set)
    std::vector<std::string> four = generateStrobogrammatic(4);
    assert(four.size() == 12);
    assert(four[0] == "1001");
    assert(four[1] == "1111");
    assert(four[2] == "1691");
    assert(four[3] == "1881");
    assert(four[4] == "1961");
    assert(four[5] == "6009");
    assert(four[6] == "6119");
    assert(four[7] == "6699");
    assert(four[8] == "6889");
    assert(four[9] == "6969");
    assert(four[10] == "8008");
    assert(four[11] == "8118");

    // Ensure no leading zeros for n>1
    for (const auto& s : generateStrobogrammatic(3)) {
        assert(s[0] != '0');
    }
    for (const auto& s : generateStrobogrammatic(4)) {
        assert(s[0] != '0');
    }

    // Ensure all are valid strobogrammatic by simple rotation check
    auto isStrobogrammatic = [](const std::string& s) {
        std::string rotated;
        for (char ch : s) {
            if (ch == '0' || ch == '1' || ch == '8') rotated += ch;
            else if (ch == '6') rotated += '9';
            else if (ch == '9') rotated += '6';
            else return false;
        }
        std::reverse(rotated.begin(), rotated.end());
        return rotated == s;
    };
    for (int n = 2; n <= 6; ++n) {
        for (const auto& s : generateStrobogrammatic(n)) {
            assert(isStrobogrammatic(s));
        }
    }

    return 0;
}
