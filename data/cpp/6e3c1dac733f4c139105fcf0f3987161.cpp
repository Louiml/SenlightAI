Write a C++ function `std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string> strs)` that groups all strings that are anagrams of each other (i.e., contain the same characters with the same frequencies, but possibly in different orders) into subvectors. The function should return a vector of vectors, where each inner vector contains all strings from the input that are anagrams of each other, preserving their original relative order within each group. The order of the groups themselves does not matter. The input vector may be empty, may contain duplicate strings, may contain single-character strings, and may contain strings with different lengths. The function must not modify the original input vector.
// The core idea is to use a sorted version of each string as a canonical key. For every string in the input, create its key by sorting its characters. Maintain an `unordered_map` that maps each key to the index of the corresponding group in the result vector. If the key already exists, append the current string to that existing group; otherwise, create a new group containing just this string, add it to the result vector, and record its index in the map. This ensures strings that are anagrams produce identical sorted keys and thus end up in the same group. Edge cases: an empty input returns an empty vector; duplicate strings will naturally be placed in the same group (since their sorted keys match); strings of different lengths cannot be anagrams because sorting different lengths produces different key lengths; single-character strings are handled trivially. Time complexity is \(O(N \cdot K \log K)\), where \(N\) is the number of strings and \(K\) is the maximum length of any string (due to sorting each string). Space complexity is \(O(N \cdot K)\) for storing the keys and the result.
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// Group strings that are anagrams of each other.
// Each inner vector contains all strings that are anagrams of one another,
// preserving the original relative order within each group.
std::vector<std::vector<std::string>> groupAnagrams(const std::vector<std::string>& strs) {
    std::vector<std::vector<std::string>> result;
    std::unordered_map<std::string, std::size_t> keyToGroupIndex;

    for (const std::string& str : strs) {
        // Build the canonical key by sorting the characters.
        std::string key = str;
        std::sort(key.begin(), key.end());

        auto iter = keyToGroupIndex.find(key);
        if (iter != keyToGroupIndex.end()) {
            // Key exists: append to the existing group.
            result[iter->second].push_back(str);
        } else {
            // Key is new: create a new group and record its index.
            result.emplace_back(1, str);
            keyToGroupIndex.emplace(key, result.size() - 1);
        }
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution's function is declared in this file; for test purposes,
// include the solution code above or place it in the same translation unit.

int main() {
    // Basic anagram grouping
    std::vector<std::string> test1 = {"eat", "tea", "tan", "ate", "nat", "bat"};
    auto res1 = groupAnagrams(test1);
    assert(res1.size() == 3);
    // Find the group containing "bat" (no anagrams)
    for (auto& group : res1) {
        if (group[0] == "bat") {
            assert(group.size() == 1);
            assert(group[0] == "bat");
        }
    }
    // Verify the other two groups have size 3 each
    int threeCount = 0;
    for (auto& group : res1) {
        if (group.size() == 3) threeCount++;
    }
    assert(threeCount == 2);

    // Empty input
    assert(groupAnagrams({}).empty());

    // All identical strings
    auto res2 = groupAnagrams({"ab", "ab", "ab"});
    assert(res2.size() == 1);
    assert(res2[0].size() == 3);
    assert(res2[0][0] == "ab" && res2[0][1] == "ab" && res2[0][2] == "ab");

    // Single characters and duplicates
    auto res3 = groupAnagrams({"a", "b", "a", "c"});
    assert(res3.size() == 3);
    for (auto& group : res3) {
        if (group[0] == "a") {
            assert(group.size() == 2);
            assert(group[0] == "a" && group[1] == "a");
        } else {
            assert(group.size() == 1);
        }
    }

    // Different lengths cannot be anagrams
    auto res4 = groupAnagrams({"ab", "a", "abc", "ba"});
    assert(res4.size() == 3);
    for (auto& group : res4) {
        if (group.size() == 2) {
            assert((group[0] == "ab" && group[1] == "ba") || (group[0] == "ba" && group[1] == "ab"));
        }
    }

    // Case sensitivity preserved
    auto res5 = groupAnagrams({"Eat", "eat", "ate"});
    assert(res5.size() == 2);
    for (auto& group : res5) {
        if (group[0] == "Eat") {
            assert(group.size() == 1);
        } else {
            assert(group.size() == 2);
        }
    }

    return 0;
}
