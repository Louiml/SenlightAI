Dijkstra's quaternion puzzle: Given a string of lowercase letters (only 'i', 'j', 'k') of length L, and a repetition count X (1 ≤ X ≤ 10^12), consider the string S formed by repeating the input string X times. The four symbols {1, i, j, k} form a non-commutative group under multiplication, with the rule: i² = j² = k² = ijk = -1, and the usual quaternion multiplication table (as in the snippet). Write a C++ function `bool canPartitionIntoIJK(const std::string& letters, long long X)` that determines whether the product of all symbols in S (from left to right) can be split into three non-empty contiguous parts (i.e., there exist cut positions a and b with 1 ≤ a < b < L·X) such that the product of the first part equals 'i', the product of the second part equals 'j', and the product of the third part equals 'k'. The function must handle very large X efficiently by exploiting periodicity. Return true if such a partition exists, false otherwise.
#include <cassert>
#include <string>

// Declare the function (assume it's defined elsewhere, for test we include it or copy)
bool canPartitionIntoIJK(const std::string& letters, long long X);

int main() {
    // Test 1: Simple single block "ijk" once: i, j, k trivially
    assert(canPartitionIntoIJK("ijk", 1) == true);

    // Test 2: Single block "ikj" once: total = -1 but can't split because order is wrong
    assert(canPartitionIntoIJK("ikj", 1) == false);

    // Test 3: Repetition makes it possible: "i" * 4? Actually "i" repeated 4 times product = 1, not -1
    assert(canPartitionIntoIJK("i", 4) == false);

    // Test 4: "i" repeated 2 times: i*i = -1, but can't split into i and k
    assert(canPartitionIntoIJK("i", 2) == false);

    // Test 5: Standard case "ji" repeated? Check known example: "ji" twice: product per block = k, k^4=1, not -1
    assert(canPartitionIntoIJK("ji", 2) == false);

    // Test 6: From the original problem: "j" * 4 -> product 1, not -1
    assert(canPartitionIntoIJK("j", 4) == false);

    // Test 7: Combination "ijk" repeated 2 times: blockProduct = -1 (since i*j*k = -1), two copies -> (-1)^2=1, not -1
    assert(canPartitionIntoIJK("ijk", 2) == false);

    // Test 8: "ijk" repeated 1: already true
    assert(canPartitionIntoIJK("ijk", 1) == true);

    // Test 9: "kji" once: product = -1 but cannot split because i segment must come before k segment
    assert(canPartitionIntoIJK("kji", 1) == false);

    // Test 10: Large X but simple: "i" repeated 3 times? i^3 = -i, not -1
    assert(canPartitionIntoIJK("i", 3) == false);

    // Edge: very large X, but letter "kj" and X=3? Let's trust the algorithm for large, just do a functional check
    // "j" repeated 2 -> product -1, but can't have i and k
    assert(canPartitionIntoIJK("j", 2) == false);

    return 0;
}
#include <string>
#include <map>
#include <vector>

// Multiplication table for quaternion units: index 0=1, 1=i, 2=j, 3=k (ignoring sign)
static const char* mulTable[4][4] = {
    {"1", "i", "j", "k"},
    {"i", "-1", "k", "-j"},
    {"j", "-k", "-1", "i"},
    {"k", "j", "-i", "-1"}
};

// Convert a symbol string (like "i", "-j") to its numeric core index: 0 for 1, 1 for i, 2 for j, 3 for k
int symbolIndex(const std::string& s) {
    char c = s[0] == '-' ? s[1] : s[0];
    if (c == '1') return 0;
    return (c - 'i') + 1; // 'i'->1, 'j'->2, 'k'->3
}

// Multiply two quaternion symbols, handling signs
std::string multiply(const std::string& a, const std::string& b) {
    bool negative = false;
    if ((a[0] == '-' && b[0] != '-') || (a[0] != '-' && b[0] == '-')) negative = true;
    int row = symbolIndex(a);
    int col = symbolIndex(b);
    std::string res = mulTable[row][col];
    if (negative) {
        if (res[0] == '-') res = res.substr(1);
        else res = "-" + res;
    }
    return res;
}

// Check if the repeated string can be split into parts with products i, j, k
bool canPartitionIntoIJK(const std::string& letters, long long X) {
    long long L = (long long)letters.size();
    if (L == 0 || X <= 0) return false;

    // Compute product of one full block
    std::string blockProduct = "1";
    for (long long i = 0; i < L; ++i) {
        std::string ch(1, letters[i]);
        blockProduct = multiply(blockProduct, ch);
    }

    // Compute product of X blocks: need cycle length of blockProduct
    // Cycle length: 1 if blockProduct == "1", 2 if == "-1", else 4 (for i,j,k,-i,-j,-k)
    int cycleLen;
    if (blockProduct == "1") {
        cycleLen = 1;
    } else if (blockProduct == "-1") {
        cycleLen = 2;
    } else {
        cycleLen = 4;
    }

    // total product = blockProduct ^ X
    std::string total = "1";
    long long exponent = X % cycleLen;
    for (int e = 0; e < exponent; ++e) {
        total = multiply(total, blockProduct);
    }

    // For valid partition, total must be -1
    if (total != "-1") return false;

    // Find earliest position (0-based) where prefix product becomes 'i'
    // Prefix product sequence repeats every 4L, so search up to 4L-1
    long long limitI = L * 4;
    long long iIndex = -1;
    std::string prefix = "1";
    for (long long pos = 0; pos < limitI; ++pos) {
        std::string ch(1, letters[pos % L]);
        prefix = multiply(prefix, ch);
        if (prefix == "i") {
            iIndex = pos;
            break;
        }
    }
    if (iIndex == -1) return false; // i never appears as prefix product

    // Find earliest position (from the right, 0-based from end) where suffix product becomes 'k'
    // Suffix product sequence also repeats every 4L
    long long limitK = L * 4;
    long long kIndex = -1;
    std::string suffix = "1";
    for (long long pos = 0; pos < limitK; ++pos) {
        // Take character from the end: index L-1-(pos % L)
        std::string ch(1, letters[L - 1 - (pos % L)]);
        suffix = multiply(ch, suffix); // note: multiply(ch, suffix) because we are building from right to left
        if (suffix == "k") {
            kIndex = pos;
            break;
        }
    }
    if (kIndex == -1) return false; // k never appears as suffix product

    // Need iIndex < posI, posK < kIndex, and there must be at least one character between the i segment and k segment
    // The i segment ends at iIndex (inclusive), the k segment starts at (L*X - 1 - kIndex) (inclusive)
    // The middle segment (j) is non-empty if iIndex + kIndex + 2 <= L*X - 1
    long long totalLength = L * X;
    if (iIndex + kIndex + 2 < totalLength) {
        return true;
    }
    return false;
}
// The key observation: the product of any prefix of the repeated string is periodic with period at most 4 times the original length L, because the quaternion group has only 8 elements (including signs). Main steps: (1) Compute the total product `total` of one full block (length L). (2) The total product of X copies is `total^X`, which can be determined by the cycle length of `total` (which is 1, 2, or 4). (3) For a valid partition to exist, the total product of S must equal the product 'i' * 'j' * 'k' = -1. If the total does not equal -1, return false immediately. (4) Find the earliest prefix of the repeated string whose product equals 'i'; because the prefix product sequence repeats every 4L characters, it suffices to search up to index 4L - 1. (5) Similarly, find the earliest suffix (from the right) whose product equals 'k'; search from the end up to length 4L - 1. (6) A partition exists if the index of the found 'i' prefix and the index of the found 'k' suffix (measured from the right) are such that the middle part (possibly including multiple full blocks) has positive length (i.e., iIndex + kIndex + 2 ≤ L*X - 1, meaning there is at least one character between them, because the middle part must be non-empty). Time complexity: O(L) for computing prefix/suffix products, and O(1) for the rest, so O(L) overall. Space complexity: O(1) additional.
