/*
Write a C++ function that takes a `std::map<int, std::string>` and returns a formatted string where each key-value pair is rendered on its own line as "key value" (with a single space between the integer and the string), in ascending order of keys. The function must preserve the natural sorted order of `std::map` and handle any map contents, including empty maps (which should return an empty string). The input map is read-only; do not modify it.
*/
#include <map>
#include <string>

// Return a formatted string of map entries, one per line as "key value".
std::string formatMap(const std::map<int, std::string>& input) {
    std::string result;
    for (const auto& entry : input) {
        result += std::to_string(entry.first) + " " + entry.second + "\n";
    }
    return result;
}
#include <cassert>
#include <map>
#include <string>

// The solution function is defined above; test it here.
int main() {
    std::map<int, std::string> m1 = {{1, "Mansi"}, {2, "Kumari"}, {3, "Khushi"}};
    assert(formatMap(m1) == "1 Mansi\n2 Kumari\n3 Khushi\n");

    std::map<int, std::string> m2 = {{5, "five"}, {1, "one"}};
    assert(formatMap(m2) == "1 one\n5 five\n");

    std::map<int, std::string> m3;
    assert(formatMap(m3) == "");

    std::map<int, std::string> m4 = {{10, "ten"}};
    assert(formatMap(m4) == "10 ten\n");

    std::map<int, std::string> m5 = {{2, "two"}, {2, "two"}, {3, "three"}}; // duplicate key ignored
    assert(formatMap(m5) == "2 two\n3 three\n");

    std::map<int, std::string> m6 = {{-1, "negative"}, {0, "zero"}};
    assert(formatMap(m6) == "-1 negative\n0 zero\n");
}
// The solution iterates through the input `std::map` using a constant reference to each pair, which automatically provides keys in ascending order due to the internal sorted structure of `std::map`. For each pair, we append the key (converted to string via `std::to_string`), a single space, the value, and a newline character `'\n'` to an accumulating string. For an empty map, the loop never executes and the function returns an empty string. Edge cases include values that contain spaces or special characters—those are printed as-is since we do not alter the string content. Time complexity is O(n) where n is the number of map entries, because we traverse each entry once. Space complexity is O(total length of all keys and values) for the output string, plus O(n) for storing the map itself (but the map is provided as input). The solution uses `const` reference in the range-based loop to avoid copying large strings, and applies `const` to the map parameter to enforce read-only access.
