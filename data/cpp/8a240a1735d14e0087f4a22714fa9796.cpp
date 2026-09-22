// Write a C++ function `extractLastOccurrenceAndTail` that takes a non-empty string `text` and a character `symbol`. The function must find the index of the **last** occurrence of `symbol` in the string. If the symbol occurs at least once, return a pair containing (a) that index (as an integer, with the first character at index 0), and (b) the substring starting **immediately after** the last occurrence of `symbol` (i.e., everything from `index + 1` to the end of the string). If the symbol is **not** found, return a pair containing `-1` (or a sentinel like `std::string::npos` but explicitly as an integer) and an empty string. The function must be case-sensitive and work for all printable characters, including spaces and digits. The input string may contain multiple repeated symbols, and the symbol may be the very last character (in which case the tail is an empty string). Use `const` qualifiers appropriately and do not modify the input.
// The core algorithm is a simple linear scan, but we can optimize by using the standard library's `rfind` method, which directly returns the index of the last occurrence (or `std::string::npos` if absent). This avoids a manual reverse loop. The main steps are: (1) Call `text.rfind(symbol)` to get the position. (2) If the position equals `std::string::npos`, return `{-1, ""}`. (3) Otherwise, compute the tail as `text.substr(position + 1)` — note that `substr` with only a starting index returns the rest of the string, and if the position is the last character, the substr length is zero, producing an empty string. Edge cases: symbol not present, symbol at the very beginning (tail is the whole string minus the first character), symbol at the very end (tail empty), and multiple identical symbols (we only care about the last one). Time complexity is O(n) due to `rfind` and `substr` (substr itself is O(n) in the worst case, but that's unavoidable as we must create a copy). Auxiliary space is O(n) for the returned substring, but the algorithm itself uses O(1) extra space besides the output.
#include <string>
#include <utility> // for std::pair

// Returns the index of the last occurrence of symbol in text,
// and the substring after that symbol. If symbol is absent,
// returns {-1, ""}.
std::pair<int, std::string> extractLastOccurrenceAndTail(const std::string& text, char symbol) {
    // Find the last occurrence using the standard library.
    std::size_t pos = text.rfind(symbol);
    
    // If not found, return a sentinel.
    if (pos == std::string::npos) {
        return {-1, ""};
    }
    
    // Build the tail substring starting right after the symbol.
    // Note: substr without a second argument takes everything to the end.
    std::string tail = text.substr(pos + 1);
    
    // Cast the position to int (safe for typical string lengths).
    return {static_cast<int>(pos), tail};
}
#include <cassert>
#include <string>
#include <utility>

// The solution function is declared above (included here for completeness in test).
std::pair<int, std::string> extractLastOccurrenceAndTail(const std::string& text, char symbol);

int main() {
    // Basic case: last 'e' in "Hello all the world!" is at index 13, tail = " world!"
    std::pair<int, std::string> r1 = extractLastOccurrenceAndTail("Hello all the world!", 'e');
    assert(r1.first == 13);
    assert(r1.second == " world!");

    // Symbol appears multiple times; tail includes everything after the last one.
    std::pair<int, std::string> r2 = extractLastOccurrenceAndTail("banana", 'a');
    assert(r2.first == 5);
    assert(r2.second == ""); // last 'a' is the final character

    // Symbol not present.
    std::pair<int, std::string> r3 = extractLastOccurrenceAndTail("hello", 'z');
    assert(r3.first == -1);
    assert(r3.second == "");

    // Symbol at the beginning.
    std::pair<int, std::string> r4 = extractLastOccurrenceAndTail("apple", 'a');
    assert(r4.first == 0);
    assert(r4.second == "pple");

    // Symbol is a space and tail has words.
    std::pair<int, std::string> r5 = extractLastOccurrenceAndTail("one two three", ' ');
    assert(r5.first == 7);
    assert(r5.second == "three");

    // Single character string with the symbol.
    std::pair<int, std::string> r6 = extractLastOccurrenceAndTail("x", 'x');
    assert(r6.first == 0);
    assert(r6.second == "");

    // Single character string without the symbol.
    std::pair<int, std::string> r7 = extractLastOccurrenceAndTail("x", 'y');
    assert(r7.first == -1);
    assert(r7.second == "");

    // Repeated symbol with tail after the last one.
    std::pair<int, std::string> r8 = extractLastOccurrenceAndTail("aXbXcXd", 'X');
    assert(r8.first == 5);
    assert(r8.second == "d");

    // Tail contains the symbol itself (after last occurrence).
    std::pair<int, std::string> r9 = extractLastOccurrenceAndTail("ababa", 'a');
    assert(r9.first == 4);
    assert(r9.second == "");
}
