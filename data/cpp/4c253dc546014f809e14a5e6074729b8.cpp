/*
Write a C++ function that takes a `std::vector<std::string>` where each string represents a single passport entry in the format `"key:value"` (e.g., `"byr:1990"`), and returns `true` if the passport is valid according to the rules below, and `false` otherwise. The function should ignore the optional `"cid"` (country ID) field entirely—it is never required. Validation requires all seven mandatory fields (`byr`, `iyr`, `eyr`, `hgt`, `hcl`, `ecl`, `pid`) to be present, and each value must satisfy: `byr` is a four-digit year between 1920 and 2002 inclusive; `iyr` is a four-digit year between 2010 and 2020 inclusive; `eyr` is a four-digit year between 2020 and 2030 inclusive; `hgt` is a number followed by `cm` (150–193 inclusive) or `in` (59–76 inclusive); `hcl` is a `#` followed by exactly six lowercase hexadecimal digits; `ecl` is exactly one of `amb`, `blu`, `brn`, `gry`, `grn`, `hzl`, `oth`; `pid` is exactly nine digits. Any missing field, malformed value, or out-of-range number makes the passport invalid. The input vector may be empty or unordered, and duplicate keys should be treated as an error (i.e., invalid). The function must be `const`-correct and should not modify the input.
*/

#include <map>
#include <set>
#include <regex>
#include <string>
#include <vector>

// Helper: validate a four-digit year within [min, max]
static bool valid_year(const std::string &str, int min, int max) {
    const static std::regex yr_re(R"(\d{4})");
    std::smatch m;
    if (!std::regex_match(str, m, yr_re)) return false;
    int value = std::stoi(m[0]);
    return value >= min && value <= max;
}

// Main function: validate a passport given as a vector of "key:value" strings
bool isValidPassport(const std::vector<std::string>& entries) {
    std::map<std::string, std::string> pass;
    for (const auto& entry : entries) {
        auto colon = entry.find(':');
        if (colon == std::string::npos) return false; // malformed entry
        std::string key = entry.substr(0, colon);
        std::string value = entry.substr(colon + 1);
        if (key == "cid") continue; // ignore cid
        auto res = pass.insert({key, value});
        if (!res.second) return false; // duplicate key
    }

    // Required fields
    const std::set<std::string> required = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    if (pass.size() != required.size()) return false;

    // Validate each field
    if (!valid_year(pass.at("byr"), 1920, 2002)) return false;
    if (!valid_year(pass.at("iyr"), 2010, 2020)) return false;
    if (!valid_year(pass.at("eyr"), 2020, 2030)) return false;

    // Height
    const std::regex hgt_re(R"((\d+)(in|cm))");
    std::smatch m;
    if (!std::regex_match(pass.at("hgt"), m, hgt_re)) return false;
    int hgt_val = std::stoi(m[1]);
    if (m[2] == "in" && (hgt_val < 59 || hgt_val > 76)) return false;
    if (m[2] == "cm" && (hgt_val < 150 || hgt_val > 193)) return false;

    // Hair color, eye color, passport ID
    const std::regex hcl_re(R"(#[0-9a-f]{6})");
    const std::regex ecl_re(R"(amb|blu|brn|gry|grn|hzl|oth)");
    const std::regex pid_re(R"([0-9]{9})");
    if (!std::regex_match(pass.at("hcl"), hcl_re)) return false;
    if (!std::regex_match(pass.at("ecl"), ecl_re)) return false;
    if (!std::regex_match(pass.at("pid"), pid_re)) return false;

    return true;
}

#include <cassert>
#include <vector>
#include <string>

// Forward declaration of the function under test
bool isValidPassport(const std::vector<std::string>& entries);

int main() {
    // Valid passport with all fields
    assert(isValidPassport({"byr:2000", "iyr:2015", "eyr:2025", "hgt:170cm",
                            "hcl:#123abc", "ecl:brn", "pid:123456789"}));

    // Valid passport with cid included (ignored)
    assert(isValidPassport({"byr:1930", "iyr:2013", "eyr:2024", "hgt:60in",
                            "hcl:#abcdef", "ecl:amb", "pid:000000001", "cid:123"}));

    // Missing one required field
    assert(!isValidPassport({"byr:2000", "iyr:2015", "eyr:2025", "hgt:170cm",
                             "hcl:#123abc", "ecl:brn"}));

    // Invalid height (too tall for cm)
    assert(!isValidPassport({"byr:2000", "iyr:2015", "eyr:2025", "hgt:200cm",
                             "hcl:#123abc", "ecl:brn", "pid:123456789"}));

    // Invalid hair color (uppercase letters)
    assert(!isValidPassport({"byr:2000", "iyr:2015", "eyr:2025", "hgt:170cm",
                             "hcl:#12ABcd", "ecl:brn", "pid:123456789"}));

    // Invalid eye color
    assert(!isValidPassport({"byr:2000", "iyr:2015", "eyr:2025", "hgt:170cm",
                             "hcl:#123abc", "ecl:red", "pid:123456789"}));

    // Invalid PID (too short)
    assert(!isValidPassport({"byr:2000", "iyr:2015", "eyr:2025", "hgt:170cm",
                             "hcl:#123abc", "ecl:brn", "pid:12345678"}));

    // Duplicate key
    assert(!isValidPassport({"byr:2000", "byr:2001", "iyr:2015", "eyr:2025",
                             "hgt:170cm", "hcl:#123abc", "ecl:brn", "pid:123456789"}));

    // Empty input
    assert(!isValidPassport({}));

    // Exactly valid boundaries (byr min, hgt max cm)
    assert(isValidPassport({"byr:1920", "iyr:2010", "eyr:2020", "hgt:193cm",
                            "hcl:#000000", "ecl:oth", "pid:999999999"}));

    // Invalid year format (not four digits)
    assert(!isValidPassport({"byr:192", "iyr:2015", "eyr:2025", "hgt:170cm",
                             "hcl:#123abc", "ecl:brn", "pid:123456789"}));

    return 0;
}

// The solution builds a `std::map<std::string, std::string>` from the vector entries, checking for duplicate keys as it inserts (using `insert` and verifying the return value). After building the map, we verify that it contains exactly the seven required keys (ignoring `cid` if present) by comparing against a `std::set`. Then, each value is validated with helper functions: `valid_year` uses `std::regex` to match exactly four digits and range-checks the integer; `hgt` is parsed with a regex capturing the number and unit, then range-checked per unit; `hcl`, `ecl`, and `pid` are checked with exact regex matches. Edge cases include: empty input (returns false), missing or extra fields (extra unknown fields like `cid` are allowed but duplicates of any key are invalid), and malformed numeric strings. Time complexity is \(O(N)\) where \(N\) is the number of entries (each regex match is constant-length), and space complexity is \(O(N)\) for the map. Regex matching is linear in the string length, but all strings are short (≤10 characters), so effectively constant.
