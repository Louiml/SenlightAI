Write a C++ function named `keypadCombinations` that takes a non-negative integer `num` and an array of strings `output[]` (of sufficient capacity) as parameters. The function must generate all possible strings that can be formed by mapping each digit of `num` to its corresponding letters on a telephone keypad (digit 2→"abc", 3→"def", 4→"ghi", 5→"jkl", 6→"mno", 7→"pqrs", 8→"tuv", 9→"wxyz", and digits 0 and 1 map to an empty string meaning they contribute no letters). The function should fill the `output` array with all combinations in any order, and return the total number of combinations generated. If `num` contains digits 0 or 1, they contribute no letters to the combinations, but they still affect the total count—for example, `num = 12` maps only digit 2, so combinations are just "a","b","c" (3 results). If `num` is entirely composed of 0s and 1s (or is 0 itself), the function should output a single empty string `""` and return 1. The input `num` will be non-negative.
#include <cassert>
#include <string>

int keypadCombinations(int num, std::string output[]);

int main() {
    const int MAX = 1000; // assume enough capacity for tests
    std::string out[MAX];
    
    // Single digit cases
    int count = keypadCombinations(2, out);
    assert(count == 3);
    assert(out[0] == "a" && out[1] == "b" && out[2] == "c");
    
    count = keypadCombinations(7, out);
    assert(count == 4);
    assert(out[0] == "p" && out[1] == "q" && out[2] == "r" && out[3] == "s");
    
    // Digit 0 or 1 alone produces empty string
    count = keypadCombinations(0, out);
    assert(count == 1);
    assert(out[0] == "");
    
    count = keypadCombinations(1, out);
    assert(count == 1);
    assert(out[0] == "");
    
    // Number with a 0 or 1 in the middle (digit 10 => only digit 0? Actually 10 % 10 = 0, prefix is 1)
    count = keypadCombinations(10, out);
    assert(count == 1);
    assert(out[0] == "");
    
    // Number with a 1 in tens place (12 => digit 2)
    count = keypadCombinations(12, out);
    assert(count == 3);
    assert(out[0] == "a" && out[1] == "b" && out[2] == "c");
    
    // Two digit numbers
    count = keypadCombinations(23, out);
    assert(count == 9);
    // Check a few combinations
    bool foundAd = false, foundBf = false, foundCf = false;
    for (int i = 0; i < count; ++i) {
        if (out[i] == "ad") foundAd = true;
        if (out[i] == "bf") foundBf = true;
        if (out[i] == "cf") foundCf = true;
    }
    assert(foundAd && foundBf && foundCf);
    
    // Number with a digit having 4 letters (79)
    count = keypadCombinations(79, out);
    assert(count == 4 * 4);
    // Check all combinations have length 2 and first char from {p,q,r,s}, second from {w,x,y,z}
    for (int i = 0; i < count; ++i) {
        assert(out[i].size() == 2);
        char first = out[i][0];
        char second = out[i][1];
        assert((first >='p' && first <= 's') && (second >= 'w' && second <= 'z'));
    }
    
    // Number with leading zeros is not possible for int, but 101 -> digits 1,0,1 => only empty string
    count = keypadCombinations(101, out);
    assert(count == 1);
    assert(out[0] == "");
    
    // Larger number: 234 -> 3*3*3 = 27 combinations, all length 3
    count = keypadCombinations(234, out);
    assert(count == 27);
    for (int i = 0; i < count; ++i) {
        assert(out[i].size() == 3);
    }
    
    return 0;
}
#include <string>

// Generate all keypad letter combinations for a non-negative integer.
// Fills output[] with all combinations and returns the number of combinations.
int keypadCombinations(int num, std::string output[]) {
    // Mapping from digit to its letters; digits 0 and 1 have no letters.
    const std::string mapping[] = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
    
    // Base case: when num becomes 0, we have processed all digits.
    // For num == 0, the only combination is the empty string.
    if (num == 0) {
        output[0] = "";
        return 1;
    }
    
    // Recursively compute combinations for the prefix (num/10).
    int smallerCount = keypadCombinations(num / 10, output);
    
    int digit = num % 10;
    const std::string& current = mapping[digit];
    
    // If the digit maps to no letters (0 or 1), the current digit adds nothing.
    // The total count remains the same as the prefix count.
    if (current.empty()) {
        return smallerCount;
    }
    
    // Number of letters available for this digit.
    int letters = current.size();
    int totalCount = smallerCount * letters;
    
    // Duplicate the existing prefix strings so we have `letters` copies of each.
    // Start from the end and copy backwards to avoid overwriting.
    for (int i = 1; i < letters; ++i) {
        for (int j = 0; j < smallerCount; ++j) {
            output[i * smallerCount + j] = output[j];
        }
    }
    
    // Append each letter to the appropriate block of prefixes.
    for (int letterIdx = 0; letterIdx < letters; ++letterIdx) {
        for (int prefixIdx = 0; prefixIdx < smallerCount; ++prefixIdx) {
            int index = letterIdx * smallerCount + prefixIdx;
            output[index] += current[letterIdx];
        }
    }
    
    return totalCount;
}
// The solution uses a recursive divide-and-conquer approach similar to the provided snippet. The core idea: take the last digit `n = num % 10`, get its mapping string `current`. First, solve the smaller problem recursively for `num / 10`, which fills the `output` array with all combinations of the remaining prefix digits, say `smallerProblem` count of strings. Then, for each of the `current.size()` letters in the current digit's mapping, we need to append that letter to every prefix combination. The provided snippet handles this by first duplicating the existing `smallerProblem` strings enough times (so that we have `current.size()` copies of each prefix), and then appending the appropriate letter to each block. Edge cases: if the current digit is 0 or 1, the mapping is empty string. The snippet sets `output[0] = ""` and returns 1, effectively ignoring that digit for combination generation but still adding 1 to the multiplicative count (since multiplying by 1 doesn't change count). If the number is 0 or 1, the recursion base case returns 1 with output[0]="". Also note that digits with 4 letters (7 and 9) are handled correctly because the mapping array contains "pqrs" and "wxyz". Time complexity: If the number has `d` digits and the product of letter counts is `C` total combinations, generating all combinations takes O(d * C) time because each combination of length `d` is built by copying and appending. Space complexity: O(C) for the output array itself, plus O(d) recursion stack depth.
