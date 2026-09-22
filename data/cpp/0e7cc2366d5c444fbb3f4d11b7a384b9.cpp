/*
Write a C++ function named `validatePasswords` that takes a first line string (which may be ignored), an integer array of length five containing `length`, `upper`, `lower`, `number`, `other`, and a vector of strings representing up to 10 candidate passwords, and returns a boolean indicating whether all candidate passwords satisfy the following requirements: each password must have exactly `length` characters; it must contain at least `upper` uppercase letters (`'A'`–`'Z'`), at least `lower` lowercase letters (`'a'`–`'z'`), at least `number` digits (`'0'`–`'9'`), and at least `other` characters that are not letters or digits. Additionally, the sum of the four minimum counts must not exceed `length`, and the requirements themselves must be valid (length between 12 and 16 inclusive, and each minimum count at least 2). If any requirement is invalid or any password fails, the function returns `false`; otherwise it returns `true`. The function should ignore the first line of input entirely and should handle an empty password vector by returning `false`.
*/
#include <string>
#include <vector>

// Validate passwords against given length and character category requirements.
// Returns true if all passwords meet the criteria, false otherwise.
bool validatePasswords(
    int length,
    int upper,
    int lower,
    int number,
    int other,
    const std::vector<std::string>& passwords
) {
    // Validate the rule parameters.
    if (length < 12 || length > 16) return false;
    if (upper < 2 || lower < 2 || number < 2 || other < 2) return false;
    if (upper + lower + number + other > length) return false;
    if (passwords.empty()) return false;

    // Check each password.
    for (const std::string& pwd : passwords) {
        int upper_real = 0;
        int lower_real = 0;
        int number_real = 0;
        int other_real = 0;

        for (char ch : pwd) {
            if (ch >= 'a' && ch <= 'z') {
                ++lower_real;
            } else if (ch >= 'A' && ch <= 'Z') {
                ++upper_real;
            } else if (ch >= '0' && ch <= '9') {
                ++number_real;
            } else {
                ++other_real;
            }
        }

        int total = static_cast<int>(pwd.size());
        if (total != length) return false;
        if (upper_real < upper) return false;
        if (lower_real < lower) return false;
        if (number_real < number) return false;
        if (other_real < other) return false;
    }

    return true;
}
#include <cassert>
#include <vector>
#include <string>

// Function declaration (from solution).
bool validatePasswords(int length, int upper, int lower, int number, int other,
                       const std::vector<std::string>& passwords);

int main() {
    // Valid: length 12, each min 2, total 8 <= 12, password meets all.
    assert(validatePasswords(12, 2, 2, 2, 2, {"Abcdef12!@#"}));

    // Valid: multiple passwords all correct.
    assert(validatePasswords(14, 3, 3, 2, 2,
        {"Abcdefgh123!!", "XYZxyz456##"}));

    // Invalid: password length mismatch.
    assert(!validatePasswords(12, 2, 2, 2, 2, {"Abcdef12!@"}));

    // Invalid: password missing enough uppercase.
    assert(!validatePasswords(12, 3, 2, 2, 2, {"Abcdef12!@#"}));

    // Invalid: requirement sum exceeds length.
    assert(!validatePasswords(12, 4, 4, 4, 4, {"Abcdef12!@#"}));

    // Invalid: length out of range.
    assert(!validatePasswords(11, 2, 2, 2, 2, {"Abcdef12!@"}));

    // Invalid: one minimum count below 2.
    assert(!validatePasswords(12, 1, 2, 2, 2, {"Abcdef12!@#"}));

    // Invalid: empty password list.
    assert(!validatePasswords(12, 2, 2, 2, 2, {}));

    // Valid: exactly the minimum counts.
    assert(validatePasswords(12, 2, 2, 2, 2, {"AAbb12!!@@##"}));

    // Invalid: password has an unexpected extra non-alphanumeric character.
    assert(!validatePasswords(12, 2, 2, 2, 2, {"AAbb12!!@@# "}));

    return 0;
}
// The solution first validates the rule parameters: length must be between 12 and 16, each minimum count must be at least 2, and the sum of the four minimums must not exceed length. If any rule fails, immediately return `false`. Next, iterate over each candidate password string. For each password, count the occurrences of four character categories by traversing the string once: lowercase letters, uppercase letters, digits, and all other characters. After counting, check that the total character count equals `length` and that each category count meets or exceeds the required minimum. If any password violates these, return `false`. If all passwords pass, return `true`. The time complexity is \(O(P \cdot L)\) where \(P\) is the number of passwords (at most 10) and \(L\) is the maximum password length (bounded by 16 in valid cases), so effectively constant time. Space complexity is \(O(1)\) beyond the input storage. Edge cases include empty password vector, invalid rule parameters, and passwords of different lengths.
