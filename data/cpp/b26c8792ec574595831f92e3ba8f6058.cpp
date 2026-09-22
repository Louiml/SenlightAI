// Create a C++ function that takes a vector of phone records (each containing a first name, last name, and phone number as a long long) and returns a new vector containing only those records whose first name starts with a letter from 'A' to 'M' (case-insensitive), sorted by first name in ascending lexicographical order. The input vector may be empty, may contain duplicate names, and names may have leading/trailing whitespace that should be ignored when checking the first letter but preserved in the output. The function must not modify the input vector.
// The solution approach involves filtering and sorting. First, iterate through the input vector to identify records where the first non-whitespace character of the first name is a letter from 'A' to 'M' (inclusive), converting to uppercase for case-insensitive comparison. Collect these records into a new vector. Then, sort the filtered vector using a custom comparator that compares first names lexicographically using ASCII/character-by-character comparison (ignoring case is not required for sorting since the filtered set already excludes letters N-Z, but to be safe, we can compare using standard string comparison after trimming whitespace). Edge cases: an empty input yields an empty output; records with first names starting with non-alphabetic characters are excluded; names with leading whitespace must be trimmed for the first-letter check; duplicates are preserved. Time complexity is O(n log n) due to sorting, where n is the number of filtered records; space complexity is O(n) for the output vector.
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

struct PhoneRecord {
    std::string firstName;
    std::string lastName;
    long long phoneNumber;
};

// Return records whose first name starts with A-M (case-insensitive), sorted by first name.
std::vector<PhoneRecord> filterAndSort(const std::vector<PhoneRecord>& records) {
    std::vector<PhoneRecord> result;

    // Find first non-whitespace character
    auto firstAlpha = [](const std::string& name) -> char {
        size_t pos = 0;
        while (pos < name.size() && std::isspace(static_cast<unsigned char>(name[pos]))) {
            ++pos;
        }
        if (pos < name.size()) {
            return std::toupper(static_cast<unsigned char>(name[pos]));
        }
        return '\0';
    };

    for (const auto& rec : records) {
        char c = firstAlpha(rec.firstName);
        if (c >= 'A' && c <= 'M') {
            result.push_back(rec);
        }
    }

    // Sort by first name (lexicographically, using standard string comparison)
    std::sort(result.begin(), result.end(), [](const PhoneRecord& a, const PhoneRecord& b) {
        return a.firstName < b.firstName;
    });

    return result;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Struct must be visible here too; include the same struct definition or assume it's above
    struct PhoneRecord {
        std::string firstName;
        std::string lastName;
        long long phoneNumber;
    };

    // Test 1: Basic filter and sort
    std::vector<PhoneRecord> input = {
        {"Zoe", "Smith", 111},
        {"Alice", "Brown", 222},
        {"mike", "Jones", 333},
        {"Nina", "Lee", 444},
        {"Bob", "White", 555}
    };
    auto out = filterAndSort(input);
    assert(out.size() == 3);
    assert(out[0].firstName == "Alice");
    assert(out[1].firstName == "Bob");
    assert(out[2].firstName == "mike");
    assert(out[0].phoneNumber == 222);
    assert(out[2].phoneNumber == 333);

    // Test 2: Empty input
    std::vector<PhoneRecord> empty;
    assert(filterAndSort(empty).empty());

    // Test 3: Leading whitespace, case-insensitivity
    std::vector<PhoneRecord> input2 = {
        {"  Charlie", "X", 1},
        {"  mary", "Y", 2},
        {"  nancy", "Z", 3}
    };
    auto out2 = filterAndSort(input2);
    assert(out2.size() == 2);
    assert(out2[0].firstName == "  Charlie");
    assert(out2[1].firstName == "  mary");

    // Test 4: Duplicate names
    std::vector<PhoneRecord> input3 = {
        {"Ann", "A", 10},
        {"ann", "B", 20},
        {"Ben", "C", 30}
    };
    auto out3 = filterAndSort(input3);
    assert(out3.size() == 3);
    assert(out3[0].firstName == "Ann");
    assert(out3[1].firstName == "ann");
    assert(out3[2].firstName == "Ben");

    // Test 5: Names starting with non-alphabetic or N-Z excluded
    std::vector<PhoneRecord> input4 = {
        {"1st", "D", 1},
        {"Nick", "E", 2},
        {"Amy", "F", 3}
    };
    auto out4 = filterAndSort(input4);
    assert(out4.size() == 1);
    assert(out4[0].firstName == "Amy");

    return 0;
}
