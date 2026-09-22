Write a C++ function named `countValidPassports` that takes a `std::vector<std::string>` where each string represents a single passport record with key:value pairs separated by spaces (e.g., `"byr:1937 iyr:2010 eyr:2020 hgt:183cm hcl:#fffffd ecl:amb pid:012345678"`). A passport is considered valid if it contains all of the following required fields: `byr`, `iyr`, `eyr`, `hgt`, `hcl`, `ecl`, and `pid` (the `cid` field is optional and ignored). The function should return the number of valid passports in the input vector. Assume each string is non-empty and contains at least one field, and fields are separated by a single space. The function may be used in both sequential and parallel contexts, so it must be thread-safe and not rely on any global mutable state.
// The solution iterates over each passport string in the input vector. For each passport, we need to check that all seven required field keys appear somewhere in the string. A straightforward approach is to use `std::string::find` for each required key. However, care must be taken to avoid false positives: for example, the key `byr` could appear as a substring in another key like `xbyr`, but given the problem specification that fields are space-separated key:value pairs, a key match using `find` is acceptable because each key is a distinct token. Still, a more robust check would parse the string into tokens and compare the exact key before the colon. For efficiency and clarity, we'll use `find` as in the original snippet, but note that this could falsely match if a key is a substring of a value (e.g., `hcl` inside a value). To be safe, we can parse the passport string into a set of keys by splitting on spaces and extracting the part before the colon. This adds a small overhead but guarantees correctness. Time complexity is O(P * F * L) where P is number of passports, F is 7 fields, and L is average length of a passport string (since `find` is O(L) worst-case). Parsing into a set would add O(L) per passport as well, still linear. Auxiliary space is O(F) for the set per passport, or O(1) if using simple `find` checks. Edge cases: empty passport string (should be invalid), all fields present, missing one field, and extra `cid` field present (should still be valid if all required present). The function must be `const`-correct and not modify the input.
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>

// Counts how many passport strings contain all required fields.
// Required fields are: byr, iyr, eyr, hgt, hcl, ecl, pid.
// The 'cid' field is optional.
uint32_t countValidPassports(const std::vector<std::string>& passports) {
    const std::unordered_set<std::string> required = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    uint32_t validCount = 0;

    for (const auto& passport : passports) {
        std::unordered_set<std::string> presentKeys;
        std::istringstream stream(passport);
        std::string token;

        // Split by spaces and extract the key before the colon.
        while (stream >> token) {
            size_t colonPos = token.find(':');
            if (colonPos != std::string::npos) {
                presentKeys.insert(token.substr(0, colonPos));
            }
        }

        // Check if all required keys are present.
        bool isValid = true;
        for (const auto& key : required) {
            if (presentKeys.find(key) == presentKeys.end()) {
                isValid = false;
                break;
            }
        }
        if (isValid) {
            ++validCount;
        }
    }

    return validCount;
}
#include <cassert>
#include <string>
#include <vector>

int main() {
    // Fully valid passport with optional cid.
    assert(countValidPassports({"byr:1937 iyr:2010 eyr:2020 hgt:183cm hcl:#fffffd ecl:amb pid:012345678"}) == 1);

    // Missing one required field (hgt missing), should be invalid.
    assert(countValidPassports({"byr:1937 iyr:2010 eyr:2020 hcl:#fffffd ecl:amb pid:012345678"}) == 0);

    // Valid passport plus extra cid, still valid.
    assert(countValidPassports({"byr:1940 iyr:2015 eyr:2025 hgt:170cm hcl:#123abc ecl:gry pid:123456789 cid:123"}) == 1);

    // Empty string should be invalid.
    assert(countValidPassports({""}) == 0);

    // Multiple passports: first valid, second invalid.
    std::vector<std::string> passports = {
        "byr:1990 iyr:2016 eyr:2024 hgt:170cm hcl:#abcd ef ecl:brn pid:012345678",
        "byr:1990 iyr:2016 eyr:2024 hgt:170cm hcl:#abcd ef ecl:brn"
    };
    assert(countValidPassports(passports) == 1);

    // All required fields present but order is shuffled.
    assert(countValidPassports({"pid:000000001 ecl:zzz hcl:#000000 hgt:60in eyr:2030 iyr:2020 byr:2000"}) == 1);

    // Duplicate fields with one missing required key (byr present twice, but pid missing).
    assert(countValidPassports({"byr:2000 byr:2001 iyr:2020 eyr:2020 hgt:60in hcl:#000000 ecl:amb"}) == 0);

    // Only optional cid field, invalid.
    assert(countValidPassports({"cid:123"}) == 0);

    // Passport with exactly all required fields in order.
    assert(countValidPassports({"byr:1970 iyr:2011 eyr:2021 hgt:160cm hcl:#111111 ecl:oth pid:999999999"}) == 1);
}
