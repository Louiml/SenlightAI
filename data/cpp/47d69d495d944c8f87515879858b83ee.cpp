// Write a C++ function `std::string extractIdentityInfo(const std::string& idNumber)` that parses a Chinese resident identity card number (18 characters: 6 digits region code, 8 digits birth date, 3 digits sequence, 1 checksum digit). Based on the first two digits (the province code), the function must return a formatted string: `"He/She is from <Province>,and his/her birthday is on <MM>,<DD>,<YYYY> based on the table."` where `<Province>` is one of exactly these mappings: `"33"` → `"Zhejiang"`, `"11"` → `"Beijing"`, `"71"` → `"Taiwan"`, `"81"` → `"Hong Kong"`, `"82"` → `"Macao"`, `"54"` → `"Tibet"`, `"21"` → `"Liaoning"`, `"31"` → `"Shanghai"`. For any other province code, use `"Unknown"`. The birth date is extracted from positions 6–13 (0‑based: characters 6‑9 for year, 10‑11 for month, 12‑13 for day). The input is always exactly 18 characters and consists of digits only, but line breaks or spaces may be present in the test string? No—assume the input string is a clean 18‑digit numeric string. The output must exactly match the format, with no extra spaces or newlines. The function must be `const`‑correct and use `std::string` operations only (no C‑style char arrays). Provide a free function (no `main`).

#include <cassert>
#include <string>

// Declaration of the function under test (assume it is in the same translation unit).
std::string extractIdentityInfo(const std::string& idNumber);

int main() {
    // Test a known province (Zhejiang) with a sample birthday.
    assert(extractIdentityInfo("330123199001011234") ==
           "He/She is from Zhejiang,and his/her birthday is on 01,01,1990 based on the table.");
    
    // Test Beijing.
    assert(extractIdentityInfo("110101198502032345") ==
           "He/She is from Beijing,and his/her birthday is on 02,03,1985 based on the table.");
    
    // Test Hong Kong (note the space in the province name).
    assert(extractIdentityInfo("810000200012312233") ==
           "He/She is from Hong Kong,and his/her birthday is on 12,31,2000 based on the table.");
    
    // Test registration with leading zeros in month/day.
    assert(extractIdentityInfo("310101200502090011") ==
           "He/She is from Shanghai,and his/her birthday is on 02,09,2005 based on the table.");
    
    // Test unknown province code.
    assert(extractIdentityInfo("991234199912319999") ==
           "He/She is from Unknown,and his/her birthday is on 12,31,1999 based on the table.");
    
    // Test that the year is extracted correctly (4 digits).
    assert(extractIdentityInfo("210203198809152222") ==
           "He/She is from Liaoning,and his/her birthday is on 09,15,1988 based on the table.");
    
    // Test Taiwan and Macao.
    assert(extractIdentityInfo("710000202402292211") ==
           "He/She is from Taiwan,and his/her birthday is on 02,29,2024 based on the table.");
    assert(extractIdentityInfo("820000201006301111") ==
           "He/She is from Macao,and his/her birthday is on 06,30,2010 based on the table.");
    
    // Test Tibet.
    assert(extractIdentityInfo("540101197012311234") ==
           "He/She is from Tibet,and his/her birthday is on 12,31,1970 based on the table.");
    
    return 0;
}

#include <string>

// Extracts province and birth date from an 18-digit Chinese ID number.
// Returns a formatted string exactly as specified.
std::string extractIdentityInfo(const std::string& idNumber) {
    // Province code is the first two characters.
    std::string provinceCode = idNumber.substr(0, 2);
    std::string province;
    
    if (provinceCode == "33") {
        province = "Zhejiang";
    } else if (provinceCode == "11") {
        province = "Beijing";
    } else if (provinceCode == "71") {
        province = "Taiwan";
    } else if (provinceCode == "81") {
        province = "Hong Kong";
    } else if (provinceCode == "82") {
        province = "Macao";
    } else if (provinceCode == "54") {
        province = "Tibet";
    } else if (provinceCode == "21") {
        province = "Liaoning";
    } else if (provinceCode == "31") {
        province = "Shanghai";
    } else {
        province = "Unknown";
    }
    
    // Birth date: year (chars 6-9), month (chars 10-11), day (chars 12-13).
    std::string year = idNumber.substr(6, 4);
    std::string month = idNumber.substr(10, 2);
    std::string day = idNumber.substr(12, 2);
    
    return "He/She is from " + province + ",and his/her birthday is on " +
           month + "," + day + "," + year + " based on the table.";
}

// The solution first validates that the input length is exactly 18 (though the problem guarantees it, checking is safe). Extract the first two characters as a substring and compare them against a static map (or a series of `if` statements, since the mapping is small). If no match, default to `"Unknown"`. Then extract the birth date components using `substr` with the correct offsets: year = `idNumber.substr(6,4)`, month = `idNumber.substr(10,2)`, day = `idNumber.substr(12,2)`. Build the output using string concatenation or `std::ostringstream` to avoid formatting issues. No need to convert to numeric types, because the substring already has leading zeros (e.g., `"03"` for March). Edge cases: the province code `"33"` appears more than once in the mapping? No, each code is unique. If the input were shorter than 14, `substr` would throw, but the problem guarantees a valid 18‑digit input, so no exception handling is required. Time complexity is O(1) because the string length is fixed (18) and only constant‑time substring operations and comparisons are performed. Space complexity is O(1) auxiliary, excluding the returned string. The output format uses a comma after the month and day, and the year is last. Note that the original snippet used `memcpy` and C strings; we modernize using `std::string` and `substr`. The function must be declared `std::string` and take a `const std::string&`.
