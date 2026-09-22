// Create a C++ function named `countWordFrequencies` that takes a non-empty string containing words separated by spaces and punctuation (specifically the characters space, tab, newline, period, comma, semicolon, colon, exclamation, question mark, single quote, double quote, and hyphen), and returns a `std::map<std::string, unsigned int>` where each key is a word in its original case (no lowercase conversion) and the value is the number of times that word appears. The function must treat words as case-sensitive, ignore empty tokens (e.g., consecutive punctuation or spaces), and consider hyphen as a delimiter (so "state-of-the-art" becomes three words: "state", "of", "the", "art"). Duplicate words should be counted, and the output map should be sorted alphabetically by word.

To solve this, we need to tokenize the input string using a delimiter set containing all the specified punctuation and whitespace characters. The most straightforward approach is to use `std::istringstream` combined with a custom delimiter character set by reading character-by-character, accumulating non-delimiter characters into a temporary word, and finalizing the word when a delimiter is encountered or the input ends. Alternatively, we can use `std::string::find_first_of` to locate the next delimiter, extract substrings between delimiters, and skip empty tokens. For each token extracted, we increment its count in a `std::map<std::string, unsigned int>`. The `std::map` automatically keeps keys sorted and handles duplicate insertion by incrementing the existing value. Edge cases include leading/trailing delimiters, multiple consecutive delimiters, and an empty token list (though input is guaranteed non-empty, it could contain only delimiters). Time complexity is \(O(n \cdot w)\) where \(n\) is the input length and \(w\) is the average word length (due to string copying into the map), but effectively linear for typical inputs, and space complexity is \(O(m \cdot L)\) where \(m\) is the number of distinct words and \(L\) is the average word length.

#include <map>
#include <string>

// Count word frequencies in a string, treating specified punctuation as delimiters.
std::map<std::string, unsigned int> countWordFrequencies(const std::string& text) {
    const std::string delimiters = " \t\n.,;:!?'\"-";
    std::map<std::string, unsigned int> freq;
    std::string::size_type start = 0;

    while (start < text.length()) {
        // Find the first delimiter from current position
        std::string::size_type end = text.find_first_of(delimiters, start);
        if (end == std::string::npos) {
            end = text.length();  // Remaining text has no delimiters
        }

        // Extract token if non-empty
        if (end > start) {
            std::string word = text.substr(start, end - start);
            ++freq[word];
        }

        // Move past the delimiter(s)
        start = (end == text.length()) ? end : end + 1;
    }

    return freq;
}

#include <cassert>
#include <map>
#include <string>

// The solution function is assumed to be declared above.

int main() {
    // Basic case with multiple words and punctuation
    std::map<std::string, unsigned int> result = countWordFrequencies("Hello, world! Hello world.");
    assert(result.size() == 2);
    assert(result["Hello"] == 2);
    assert(result["world"] == 2);

    // Case sensitivity and hyphen as delimiter
    result = countWordFrequencies("State-of-the-art State");
    assert(result.size() == 4);
    assert(result["State"] == 2);          // Both "State" from the phrase and standalone "State"
    assert(result["of"] == 1);
    assert(result["the"] == 1);
    assert(result["art"] == 1);

    // Multiple consecutive delimiters and leading/trailing delimiters
    result = countWordFrequencies("  a,,b  c;;;d  ");
    assert(result.size() == 4);
    assert(result["a"] == 1);
    assert(result["b"] == 1);
    assert(result["c"] == 1);
    assert(result["d"] == 1);

    // Only delimiters (no words)
    result = countWordFrequencies("--- ,,, ...");
    assert(result.empty());

    // Single word with no delimiters
    result = countWordFrequencies("lone");
    assert(result.size() == 1);
    assert(result["lone"] == 1);

    // Repeated word many times
    result = countWordFrequencies("same same same");
    assert(result.size() == 1);
    assert(result["same"] == 3);

    // Newline and tab as delimiters
    result = countWordFrequencies("line1\nline2\tline1");
    assert(result.size() == 2);
    assert(result["line1"] == 2);
    assert(result["line2"] == 1);
}
