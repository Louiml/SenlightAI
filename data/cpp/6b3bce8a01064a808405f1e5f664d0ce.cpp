// Create a C++ function named `validatePassport` that takes a `std::map<std::string, std::string>` representing a passport (keys are field names like "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid"; values are strings) and returns a `bool` indicating whether the passport is valid according to the strict rules from the original snippet. The function must check: (1) `byr` is an integer between 1920 and 2002 inclusive; (2) `iyr` is an integer between 2010 and 2020 inclusive; (3) `eyr` is an integer between 2020 and 2030 inclusive; (4) `hgt` is either "Ncm" (N between 150 and 193 inclusive) or "Nin" (N between 59 and 76 inclusive); (5) `hcl` starts with '#' and is followed by exactly six characters, each being a digit 0-9 or a lowercase letter a-f; (6) `ecl` is exactly one of {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"}; (7) `pid` is exactly nine digits (allows leading zeros). The field `cid` is ignored. If any required field is missing or malformed, return `false`. You may assume the input map always contains the required keys (except `cid`), but values may be empty strings or malformed.
#include <cassert>
#include <map>
#include <string>

// Declaration of the function under test
bool validatePassport(const std::map<std::string, std::string>& passport);

int main() {
    // Fully valid passport
    std::map<std::string, std::string> valid = {
        {"byr", "1980"}, {"iyr", "2015"}, {"eyr", "2025"},
        {"hgt", "170cm"}, {"hcl", "#123abc"}, {"ecl", "brn"}, {"pid", "000123456"}
    };
    assert(validatePassport(valid) == true);

    // Valid with cid present (ignored)
    std::map<std::string, std::string> validWithCid = valid;
    validWithCid["cid"] = "123";
    assert(validatePassport(validWithCid) == true);

    // byr too low
    std::map<std::string, std::string> badByr = valid;
    badByr["byr"] = "1919";
    assert(validatePassport(badByr) == false);

    // iyr too high
    std::map<std::string, std::string> badIyr = valid;
    badIyr["iyr"] = "2021";
    assert(validatePassport(badIyr) == false);

    // eyr invalid (2021 is actually valid, but we test 2019)
    std::map<std::string, std::string> badEyr = valid;
    badEyr["eyr"] = "2019";
    assert(validatePassport(badEyr) == false);

    // hgt invalid range (120cm)
    std::map<std::string, std::string> badHgtMin = valid;
    badHgtMin["hgt"] = "120cm";
    assert(validatePassport(badHgtMin) == false);

    // hgt invalid unit
    std::map<std::string, std::string> badHgtFormat = valid;
    badHgtFormat["hgt"] = "170";
    assert(validatePassport(badHgtFormat) == false);

    // hcl invalid (missing '#')
    std::map<std::string, std::string> badHcl = valid;
    badHcl["hcl"] = "123abc";
    assert(validatePassport(badHcl) == false);

    // hcl invalid (non-hex character 'g')
    std::map<std::string, std::string> badHclChar = valid;
    badHclChar["hcl"] = "#123abg";
    assert(validatePassport(badHclChar) == false);

    // ecl not in list
    std::map<std::string, std::string> badEcl = valid;
    badEcl["ecl"] = "red";
    assert(validatePassport(badEcl) == false);

    // pid not 9 digits
    std::map<std::string, std::string> badPid = valid;
    badPid["pid"] = "12345678";
    assert(validatePassport(badPid) == false);

    // pid has a letter
    std::map<std::string, std::string> badPidAlpha = valid;
    badPidAlpha["pid"] = "12345a789";
    assert(validatePassport(badPidAlpha) == false);

    // Missing byr key (throws out_of_range, but function should return false)
    std::map<std::string, std::string> missingByr = valid;
    missingByr.erase("byr");
    assert(validatePassport(missingByr) == false);

    return 0;
}
#include <map>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdexcept>

// Validate a passport map according to the given strict rules.
bool validatePassport(const std::map<std::string, std::string>& passport) {
    // byr: 1920-2002
    try {
        int byr = std::stoi(passport.at("byr"));
        if (byr < 1920 || byr > 2002) return false;
    } catch (...) { return false; }

    // iyr: 2010-2020
    try {
        int iyr = std::stoi(passport.at("iyr"));
        if (iyr < 2010 || iyr > 2020) return false;
    } catch (...) { return false; }

    // eyr: 2020-2030
    try {
        int eyr = std::stoi(passport.at("eyr"));
        if (eyr < 2020 || eyr > 2030) return false;
    } catch (...) { return false; }

    // hgt: "Ncm" with 150<=N<=193, or "Nin" with 59<=N<=76
    const std::string& hgt = passport.at("hgt");
    if (hgt.size() < 3) return false;
    if (hgt.substr(hgt.size() - 2) == "cm") {
        try {
            int h = std::stoi(hgt.substr(0, hgt.size() - 2));
            if (h < 150 || h > 193) return false;
        } catch (...) { return false; }
    } else if (hgt.substr(hgt.size() - 2) == "in") {
        try {
            int h = std::stoi(hgt.substr(0, hgt.size() - 2));
            if (h < 59 || h > 76) return false;
        } catch (...) { return false; }
    } else {
        return false;
    }

    // hcl: '#' followed by exactly 6 hex digits (0-9 or a-f)
    const std::string& hcl = passport.at("hcl");
    if (hcl.size() != 7 || hcl[0] != '#') return false;
    if (!std::all_of(hcl.begin() + 1, hcl.end(), [](unsigned char c) {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
    })) return false;

    // ecl: one of amb, blu, brn, gry, grn, hzl, oth
    static const std::string validEyes[] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    const std::string& ecl = passport.at("ecl");
    bool foundEye = false;
    for (const auto& e : validEyes) {
        if (ecl == e) { foundEye = true; break; }
    }
    if (!foundEye) return false;

    // pid: exactly 9 digits
    const std::string& pid = passport.at("pid");
    if (pid.size() != 9) return false;
    if (!std::all_of(pid.begin(), pid.end(), [](unsigned char c) { return std::isdigit(c); })) return false;

    // cid is ignored
    return true;
}
// The solution iterates over each required field in a deterministic order. For each field, extract the string value from the map and validate it using helper logic. For numeric fields (`byr`, `iyr`, `eyr`), use `std::stoi` wrapped in a try-catch to convert to an integer; if conversion fails (e.g., empty string or non-numeric), return `false`. For `hgt`, check if the string ends with "cm" or "in"; if neither, return `false`. Extract the numeric prefix by erasing the last two characters and converting to integer, then verify the range. For `hcl`, check first character is '#', then use `std::all_of` on the remaining substring to verify each character is a hex digit (digit 0-9 or lowercase a-f) and that the length is exactly 6. For `ecl`, use a static array of valid eye colors and check for membership with a loop. For `pid`, first check length is exactly 9, then use `std::all_of` with `std::isdigit`. All checks are independent; the function returns `false` immediately upon the first violation, otherwise returns `true`. Time complexity is O(1) because each validation operates on fixed‑length strings (max 9 characters) and constant field count; space complexity is O(1) beyond the input map.
