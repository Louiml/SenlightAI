// Write a C++ function `stableFileSorter` that takes a vector of file name strings, where each file name consists of a leading sequence of letters (the "head"), followed by a sequence of digits (the "number"), and then an optional trailing sequence of letters or digits (the "tail"). The function must return a new vector containing the original file names sorted according to the following rules: first, compare the head parts case-insensitively (using ASCII lowercase conversion); if heads are equal, compare the numeric values of the number parts (interpreting leading zeros as part of the integer, e.g., "007" equals 7); if both head and number are equal, preserve the original relative order of the input (stable sort). The input may contain file names with no tail, but every file name must contain at least one digit. The function must not modify the input vector.
// The solution mirrors the provided snippet's approach but refines it into a self-contained function. For each file name, isolate the head, number, and tail by locating the first digit and the first non-digit after that digit using `std::find_if`. Convert the head to lowercase for case-insensitive comparison. Store each parsed file as a tuple (original index, headLower, numberString) in a separate vector. Then apply `std::stable_sort` on this vector using a comparator that first compares the lowercased head lexicographically, and if equal, compares `std::stoi(number)` numerically (this correctly handles leading zeros). Because the sort is stable, ties in both head and number retain their original index order, which corresponds to the input order. Finally, rebuild the answer by extracting the original file names from the input vector using the stored indices. Edge cases: file names may have uppercase letters in head or tail (tail is irrelevant for sorting), numbers may have leading zeros, and all file names are guaranteed to contain at least one digit. Time complexity is O(n log n) for sorting, with O(n) extra space for parsing and indices. Space complexity is O(n) for the auxiliary vector and output.
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <utility>

// Helper: check if a character is a digit
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Helper: convert a string to lowercase in place (returns a new string)
std::string toLower(const std::string& s) {
    std::string result = s;
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
    }
    return result;
}

// Main function: stable-sort file names by head (case-insensitive), then by numeric value.
std::vector<std::string> stableFileSorter(const std::vector<std::string>& files) {
    // Each element: (original index, lowercased head, number substring)
    std::vector<std::tuple<int, std::string, std::string>> parsed;
    parsed.reserve(files.size());

    for (size_t i = 0; i < files.size(); ++i) {
        const std::string& file = files[i];

        // Find start of digits (first digit)
        auto itDigit = std::find_if(file.begin(), file.end(), isDigit);
        int headLen = static_cast<int>(itDigit - file.begin());

        // Find end of digits (first non-digit after head)
        auto itEndDigit = std::find_if(itDigit, file.end(), [](char c) { return !isDigit(c); });
        int numberLen = static_cast<int>(itEndDigit - itDigit);

        // Extract parts
        std::string head = toLower(file.substr(0, headLen));
        std::string number = file.substr(headLen, numberLen);

        parsed.emplace_back(i, head, number);
    }

    // Stable sort by head, then by numeric value of number
    std::stable_sort(parsed.begin(), parsed.end(),
        [](const std::tuple<int, std::string, std::string>& a,
           const std::tuple<int, std::string, std::string>& b) {
            const std::string& headA = std::get<1>(a);
            const std::string& headB = std::get<1>(b);
            if (headA != headB) {
                return headA < headB;
            }
            // Convert to integer, leading zeros handled automatically
            return std::stoi(std::get<2>(a)) < std::stoi(std::get<2>(b));
        });

    // Rebuild answer using original indices
    std::vector<std::string> result;
    result.reserve(parsed.size());
    for (const auto& entry : parsed) {
        result.push_back(files[std::get<0>(entry)]);
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// Include the solution function here (or link it)

int main() {
    // Basic case: same head, numbers sorted numerically
    std::vector<std::string> test1 = {"img12.png", "img10.png", "img2.png", "img1.png"};
    std::vector<std::string> expected1 = {"img1.png", "img2.png", "img10.png", "img12.png"};
    assert(stableFileSorter(test1) == expected1);

    // Case-insensitive head sorting: "A" before "b"
    std::vector<std::string> test2 = {"b1.txt", "A10.txt", "A2.txt", "b1.txt"};
    std::vector<std::string> expected2 = {"A10.txt", "A2.txt", "b1.txt", "b1.txt"};
    assert(stableFileSorter(test2) == expected2);

    // Leading zeros in numbers: "007" < "8"
    std::vector<std::string> test3 = {"file007.txt", "file8.txt", "file12.txt"};
    std::vector<std::string> expected3 = {"file007.txt", "file8.txt", "file12.txt"};
    assert(stableFileSorter(test3) == expected3);

    // Ties in head and number: preserve original order
    std::vector<std::string> test4 = {"abc1", "abc1", "abc1"};
    assert(stableFileSorter(test4) == test4);

    // Mixed case with tails and different numbers
    std::vector<std::string> test5 = {"F-5 Freedom", "F-4 Phantom", "F-1", "F-15 Eagle"};
    std::vector<std::string> expected5 = {"F-1", "F-4 Phantom", "F-5 Freedom", "F-15 Eagle"};
    assert(stableFileSorter(test5) == expected5);

    // Single element
    std::vector<std::string> test6 = {"a1"};
    assert(stableFileSorter(test6) == test6);

    // Head with no tail
    std::vector<std::string> test7 = {"a10", "a1", "a2"};
    std::vector<std::string> expected7 = {"a1", "a2", "a10"};
    assert(stableFileSorter(test7) == expected7);

    return 0;
}
