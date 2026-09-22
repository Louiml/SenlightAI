/*
Write a C++ function named `generateEmail` that takes a person's full name as a string (words separated by single spaces, with possible leading/trailing spaces) and returns a university email handle. The function must convert the entire name to lowercase, extract the last word as the primary part, append the first letter of each preceding word (in order) to that last word, and return the concatenated string. Multiple consecutive spaces and leading/trailing spaces must be handled gracefully. The input contains only alphabetic characters and spaces, and the name always has at least two words. The function must not print anything and must be independent of any global state.
*/
#include <string>
#include <sstream>
#include <vector>

// Convert a full name into an email handle: lowercase name, take last word,
// then append first letter of each earlier word.
std::string generateEmail(const std::string& fullName) {
    // 1. Convert to lowercase
    std::string lower;
    lower.reserve(fullName.size());
    for (char c : fullName) {
        if (c >= 'A' && c <= 'Z') {
            lower.push_back(c + 32);
        } else {
            lower.push_back(c);
        }
    }

    // 2. Split into words using a stream (handles multiple spaces)
    std::istringstream iss(lower);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }

    // 3. Build result: last word + first initial of all previous words
    std::string result = words.back();
    for (size_t i = 0; i + 1 < words.size(); ++i) {
        result.push_back(words[i][0]);
    }
    return result;
}
#include <cassert>
#include <string>

int main() {
    assert(generateEmail("Nguyen Van An") == "annv");
    assert(generateEmail("John Paul Smith") == "smithjp");
    assert(generateEmail("  Alice   Bob  ") == "boba");
    assert(generateEmail("Mary Jane Watson") == "watsonmj");
    assert(generateEmail("A B C") == "cab");
    assert(generateEmail("LE QUANG DUNG") == "dunglq");
    assert(generateEmail("Tran  thi   huong") == "huongtt");
    return 0;
}
// The core algorithm processes the input string in three phases. First, convert every uppercase letter to its lowercase equivalent by iterating through the string and applying `tolower` or a manual +32 offset for ASCII. Second, split the normalized string into words using a `std::istringstream` and a fixed-size array or a `std::vector<std::string>`; the stream naturally skips multiple spaces and leading/trailing whitespace, so word extraction is robust. Third, take the last word as the base, then prepend (or append in order) the first character of every word except the last one. The order matters: for "Nguyen Van An", the result is "an" + "n" + "v" → "annv". Edge cases include names with extra spaces (e.g., "  Alice   Bob  ") which still yield two words, and names where words may be single letters (still handled). Time complexity is \(O(L)\), where \(L\) is the total length of the input string, because each character is visited a constant number of times for case conversion and splitting. Space complexity is \(O(L)\) for storing the split words and the result string. The solution uses `std::string` for simplicity and `const std::string&` for input to avoid copying.
