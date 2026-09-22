// Write a C++ function that processes a list of lecturer records. Each record contains a full name and a subject name. The function must assign a lecturer code ("GV" followed by a zero-padded two-digit index starting from 1), derive a subject abbreviation by taking the uppercase first letter of each word in the subject name, and extract the last word of the full name as the sort key. Then sort the records in ascending order primarily by the last name (the last word of the full name), and if two records have the same last name, by the lecturer code (lexicographically). The function takes a vector of pairs (name, subject) and returns a vector of strings formatted as `"<code> <full name> <subject abbreviation>"` in sorted order. Handle cases where multiple lecturers share the same last name and ensure the code numbering is based on the original input order (not the sorted order).

The solution involves several distinct steps: 
1. **Code generation**: For each record at original index `i` (0-based), create the code `"GV" + (i+1 < 10 ? "0" + to_string(i+1) : to_string(i+1))`. This ensures zero-padding for indices 1–9.
2. **Subject abbreviation**: Parse the subject name word by word using a string stream. For each word, take the first character, convert it to uppercase using `std::toupper` (cast to `unsigned char` to avoid undefined behavior for non-ASCII), and append it to an abbreviation string. No separator between letters.
3. **Last name extraction**: Parse the full name word by word, keeping the last token found. This yields the surname.
4. **Sorting**: Use `std::sort` with a custom comparator that first compares `lastName`; if equal, compares the generated code lexicographically (which works because codes are "GV01", "GV02", ...).
5. **Output formatting**: After sorting, build each output string as `code + " " + name + " " + abbreviation`.
Time complexity: O(n * m + n log n), where n is the number of records and m is the average length of names/subjects (for parsing and extracting). Space complexity: O(n + m) for the result and temporary storage (each record stores its parsed fields).
Edge cases: Empty subject name (abbreviation becomes empty string), single-word names (last name is that word), duplicate last names (tie-break by code), and more than 9 records (codes become "GV10", "GV11", etc.). The comparator must be a strict weak ordering, so if both last name and code are equal (impossible because codes are unique), it would return false.

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

struct LecturerRecord {
    std::string name;
    std::string subject;
    std::string code;
    std::string abbreviation;
    std::string lastName;
};

// Process a list of lecturer records and return sorted output strings.
std::vector<std::string> processLecturers(const std::vector<std::pair<std::string, std::string>>& input) {
    std::vector<LecturerRecord> records;
    records.reserve(input.size());
    
    // Build records with code, abbreviation, and lastName
    for (size_t i = 0; i < input.size(); ++i) {
        LecturerRecord rec;
        rec.name = input[i].first;
        rec.subject = input[i].second;
        
        // Generate code based on original index (1-based)
        rec.code = "GV";
        if (i + 1 < 10) {
            rec.code += "0" + std::to_string(i + 1);
        } else {
            rec.code += std::to_string(i + 1);
        }
        
        // Build subject abbreviation
        std::istringstream subStream(rec.subject);
        std::string word;
        while (subStream >> word) {
            rec.abbreviation += static_cast<char>(std::toupper(static_cast<unsigned char>(word[0])));
        }
        
        // Extract lastName (last word of name)
        std::istringstream nameStream(rec.name);
        std::string token;
        while (nameStream >> token) {
            rec.lastName = token; // keeps last token
        }
        
        records.push_back(std::move(rec));
    }
    
    // Sort by lastName, then by code
    auto comparator = [](const LecturerRecord& a, const LecturerRecord& b) {
        if (a.lastName != b.lastName) {
            return a.lastName < b.lastName;
        }
        return a.code < b.code;
    };
    std::sort(records.begin(), records.end(), comparator);
    
    // Format output
    std::vector<std::string> result;
    result.reserve(records.size());
    for (const auto& rec : records) {
        result.push_back(rec.code + " " + rec.name + " " + rec.abbreviation);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic test with different last names
    std::vector<std::pair<std::string, std::string>> input1 = {
        {"Alice Johnson", "Computer Science"},
        {"Bob Smith", "Data Structures"},
        {"Carol Brown", "Artificial Intelligence"}
    };
    std::vector<std::string> result1 = processLecturers(input1);
    std::vector<std::string> expected1 = {
        "GV03 Carol Brown AI",
        "GV01 Alice Johnson CS",
        "GV02 Bob Smith DS"
    };
    assert(result1 == expected1);

    // Test with same last name, tie-break by code
    std::vector<std::pair<std::string, std::string>> input2 = {
        {"David Lee", "Machine Learning"},
        {"Emily Lee", "Operating Systems"},
        {"Frank Moore", "Database Systems"}
    };
    std::vector<std::string> result2 = processLecturers(input2);
    std::vector<std::string> expected2 = {
        "GV02 Emily Lee OS",
        "GV01 David Lee ML",
        "GV03 Frank Moore DS"
    };
    assert(result2 == expected2);

    // Test with more than 9 records (code without leading zero)
    std::vector<std::pair<std::string, std::string>> input3;
    for (int i = 0; i < 10; ++i) {
        input3.push_back({"Name" + std::to_string(i), "Subject" + std::to_string(i)});
    }
    std::vector<std::string> result3 = processLecturers(input3);
    assert(result3.size() == 10);
    // Check that the 10th record in sorted order (if it has the largest last name) uses "GV10"
    // Since Name9 has lastName "Name9" which is lexicographically largest, it should be last.
    assert(result3.back().substr(0, 4) == "GV10");
    // Check first record uses GV01 (assuming Name0 is smallest)
    assert(result3.front().substr(0, 4) == "GV01");

    // Test with single-word name and empty subject
    std::vector<std::pair<std::string, std::string>> input4 = {
        {"Zoe", ""},
        {"Adam", "C++ Programming"}
    };
    std::vector<std::string> result4 = processLecturers(input4);
    std::vector<std::string> expected4 = {
        "GV02 Adam CP",
        "GV01 Zoe "
    };
    assert(result4 == expected4);

    // Test with single record
    std::vector<std::pair<std::string, std::string>> input5 = {
        {"Sam Allen", "Physics"}
    };
    std::vector<std::string> result5 = processLecturers(input5);
    std::vector<std::string> expected5 = {
        "GV01 Sam Allen P"
    };
    assert(result5 == expected5);

    return 0;
}
