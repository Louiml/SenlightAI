// Given a string containing only lowercase English letters, write a C++ function `arrangeString(const std::string& s)` that returns a string with the same characters rearranged such that all characters are sorted in non-decreasing order, unless all characters in the input string are identical. In that special case, the function should return an empty string. The input string will be non-empty and will consist solely of lowercase letters from 'a' to 'z'.

#include <cassert>
#include <string>

// The solution function is declared above (not repeated here for brevity).
int main() {
    assert(arrangeString("cba") == "abc");
    assert(arrangeString("zzz") == "");
    assert(arrangeString("a") == "");
    assert(arrangeString("hello") == "ehllo");
    assert(arrangeString("aabb") == "aabb");  // already sorted but not all identical
    assert(arrangeString("bbaa") == "aabb");
    assert(arrangeString("abab") == "aabb");
    assert(arrangeString("zx") == "xz");
    assert(arrangeString("aaabbb") == "aaabbb");
    assert(arrangeString("baab") == "aabb");
}

#include <algorithm>
#include <string>

// Rearrange characters in non-decreasing order.
// If all characters are identical, return an empty string.
std::string arrangeString(const std::string& s) {
    std::string result = s;  // work on a copy
    std::sort(result.begin(), result.end());
    // If the first and last characters are the same, all are identical.
    if (result.front() == result.back()) {
        return std::string();  // empty string
    }
    return result;
}

// The solution is straightforward: copy the input string into a local `std::string`, then sort it using `std::sort`. After sorting, check if the first and last characters of the sorted string are equal. If they are equal, it means every character in the string is the same (since the string is sorted, and if the minimum equals the maximum). In that case, return an empty string. Otherwise, return the sorted string. Edge cases include a single-character string (which has all identical characters and should return an empty string) and strings with repeated but not all-identical characters (which should be sorted normally). The time complexity is \(O(n \log n)\) due to sorting, where \(n\) is the length of the string. The auxiliary space complexity is \(O(n)\) for the copy of the string (or \(O(1)\) if we modify the parameter, but we use a copy to keep the input unchanged). The constant-time comparison after sorting is negligible.
