/*
Write a C++ function named `phoneKeypadCombinations` that takes a vector of integers (each digit from 2 to 9, though 0 and 1 also possible but produce empty string) and returns a vector of strings containing all possible letter combinations that the number could represent, following the classic telephone keypad mapping (2→"abc", 3→"def", 4→"ghi", 5→"jkl", 6→"mno", 7→"pqrs", 8→"tuv", 9→"wxyz", and 0 or 1→" "). The combinations must be generated in lexicographic order of the digits processed from left to right (i.e., the first digit's letters vary slowest, last digit's letters vary fastest). The function must handle an empty input vector by returning a single empty string. The order of the output strings must exactly match the order produced by the recursive approach described below: for each new digit, duplicate the existing combinations for each letter (without the first letter? no—all letters) and append the letter. Since the original code uses a buggy `d=7` (assignment instead of comparison) and a loop that copies only options.length()-1 times before appending, we need to design a correct version that matches the intended output. The function should be `const`-correct and avoid dynamic allocation overhead by using `std::vector<std::string>`.
*/
#include <vector>
#include <string>
#include <array>

namespace {
    const std::array<std::string, 10> kKeypad = {
        " ",   // 0
        " ",   // 1
        "abc", // 2
        "def", // 3
        "ghi", // 4
        "jkl", // 5
        "mno", // 6
        "pqrs",// 7
        "tuv", // 8
        "wxyz" // 9
    };
}

// Returns all possible letter combinations for the given digit sequence.
// Input: digits - a vector of ints (0-9) representing phone keypad digits.
// Output: vector of strings with all combinations, in lexicographic order.
std::vector<std::string> phoneKeypadCombinations(const std::vector<int>& digits) {
    std::vector<std::string> result(1, ""); // start with one empty string

    for (int digit : digits) {
        const std::string& letters = kKeypad[digit];
        int currentSize = static_cast<int>(result.size());
        // Reserve space for the new combinations
        result.reserve(currentSize * letters.size());
        // For each existing combination, append each letter
        for (int i = 0; i < currentSize; ++i) {
            const std::string base = result[i];
            for (char c : letters) {
                result.push_back(base + c);
            }
        }
        // Remove the old combinations (first currentSize entries)
        result.erase(result.begin(), result.begin() + currentSize);
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Empty input
    assert(phoneKeypadCombinations({}) == std::vector<std::string>{""});

    // Single digit
    assert(phoneKeypadCombinations({2}) == std::vector<std::string>({"a","b","c"}));
    assert(phoneKeypadCombinations({7}) == std::vector<std::string>({"p","q","r","s"}));

    // Two digits
    assert(phoneKeypadCombinations({2,3}) == std::vector<std::string>({"ad","ae","af","bd","be","bf","cd","ce","cf"}));
    assert(phoneKeypadCombinations({9,2}) == std::vector<std::string>({"wa","wb","wc","xa","xb","xc","ya","yb","yc","za","zb","zc"}));

    // Three digits including 7 (four letters)
    assert(phoneKeypadCombinations({2,7,3}) == std::vector<std::string>({
        "apd","ape","apf","aqd","aqe","aqf","ard","are","arf","asd","ase","asf",
        "bpd","bpe","bpf","bqd","bqe","bqf","brd","bre","brf","bsd","bse","bsf",
        "cpd","cpe","cpf","cqd","cqe","cqf","crd","cre","crf","csd","cse","csf"
    }));

    // Edge case with 0 and 1 (space character)
    assert(phoneKeypadCombinations({2,0,3}) == std::vector<std::string>({
        "a d","a e","a f",
        "b d","b e","b f",
        "c d","c e","c f"
    }));

    // Large input, check count (3^3 = 27, 4^2 * 3 = 48)
    assert(phoneKeypadCombinations({2,3,4}).size() == 27);
    assert(phoneKeypadCombinations({7,8,9}).size() == 4*3*4); // 48

    // Check first and last for a longer case
    auto combos = phoneKeypadCombinations({2,3,4,5});
    assert(combos.front() == "adgj");
    assert(combos.back() == "cfil");

    return 0;
}
// The core algorithm is recursive: given a sequence of digits, the combinations for the first n-1 digits are computed, then for each letter in the last digit's mapping, each of the existing combinations is copied and the letter is appended. This is a classic backtracking/DFS problem, but the snippet implements it bottom-up recursively. To generate all combinations correctly, we can use a recursive function that builds strings incrementally, or iteratively build from an initial vector containing an empty string. The key is to ensure that for each digit, we generate `smallOutputSize * letterCount` combinations. The correct order is achieved by iterating over the existing combinations first (inner loop) and then over the letters (outer loop). Actually, the snippet's bug is in the copying step: it copies `options.length()-1` times even when `options.length()` is 3 or 4, but that’s fine because they start with the original smallOutputSize entries already present. However, the real bug is `if(d=7)` which always assigns 7, causing every digit to map to "pqrs" – that’s a logical error we must correct. For the corrected version, the correct mapping is straightforward. The time complexity is O(4^n * n) in the worst case (since each digit can yield up to 4 letters, and string concatenation copies O(length) characters). Space complexity is O(4^n * n) for storing all combinations, plus recursion stack depth O(n). Edge cases: empty input returns one empty string; digits 0 and 1 produce a space character, but that’s an unusual case; usually only 2-9 are used. The output order must be such that if input is [2,3], output is "ad","ae","af","bd","be","bf","cd","ce","cf" – first digit varies slowest, second varies fastest.
