/*
Write a C++ function `generateEmail` that takes a single line of text (a student's full name, possibly containing multiple spaces as separators or surrounding whitespace) as a std::string, and returns a string representing their institutional email address in the format: first initial of the first name (lowercase), first initial of the last name (lowercase), followed by the entire last name (lowercase) and then "@ptit.edu.vn". The input may contain arbitrary whitespace (spaces, tabs) between names, and it is guaranteed to contain at least one non-whitespace character. The output should preserve the order of the names as given (e.g., for "Nguyen Van An", the output would be "nvan@ptit.edu.vn" — note: first initial 'n', last initial 'a', full last name "an"). The last word in the input is always considered the last name. All output letters must be lowercase.
*/
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <algorithm>

// Generate an institutional email address from a full name.
// Format: firstInitial + lastInitial + lowercaseLastName + "@ptit.edu.vn"
std::string generateEmail(const std::string& fullName) {
    // Tokenize the input by any whitespace.
    std::istringstream input(fullName);
    std::vector<std::string> names;
    std::string word;
    while (input >> word) {
        names.push_back(word);
    }

    // Guaranteed at least one token.
    std::string firstName = names.front();
    std::string lastName = names.back();

    // Convert first initials and full last name to lowercase.
    char firstInitial = static_cast<char>(std::tolower(static_cast<unsigned char>(firstName[0])));
    char lastInitial = static_cast<char>(std::tolower(static_cast<unsigned char>(lastName[0])));
    std::transform(lastName.begin(), lastName.end(), lastName.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // Build the email.
    std::string result;
    result.reserve(2 + lastName.size() + 12); // 2 initials + last name + suffix
    result.push_back(firstInitial);
    result.push_back(lastInitial);
    result += lastName;
    result += "@ptit.edu.vn";

    return result;
}
#include <cassert>
#include <string>

// Assume generateEmail is defined above.

int main() {
    assert(generateEmail("Nguyen Van An") == "nvan@ptit.edu.vn");
    assert(generateEmail("  Le   Thi  Mai  ") == "lmai@ptit.edu.vn");
    assert(generateEmail("Tran Quoc Bao") == "tbao@ptit.edu.vn");
    assert(generateEmail("Alice") == "aalice@ptit.edu.vn");
    assert(generateEmail("John Paul Smith") == "jsmith@ptit.edu.vn");
    assert(generateEmail("  X  ") == "xx@ptit.edu.vn");
    assert(generateEmail("Pham ANH TU") == "ptu@ptit.edu.vn");
    assert(generateEmail("Hoang\tVan\tLong") == "hlong@ptit.edu.vn");
    return 0;
}
// The approach is to trim leading/trailing whitespace from the input string, then split it into tokens by any whitespace (using std::istringstream, which automatically ignores multiple spaces and tabs). Store the tokens in a vector. The last token is the last name; all previous tokens are first/middle names. The first character of the first token and the first character of the last token are extracted, converted to lowercase using `std::tolower` (cast to unsigned char to avoid undefined behavior for negative char values). The last token is also converted fully to lowercase. Then the result is constructed by concatenating the first initial, the last initial, the lowercase last name, and the fixed suffix "@ptit.edu.vn". Edge cases: if the input has only one token (e.g., "Alice"), then the first and last tokens are the same, but the problem still expects the first initial of that sole token and the entire token as the last name, so the output would be "aalice@ptit.edu.vn". Ensure that leading/trailing whitespace does not create empty tokens. Time complexity is O(n) where n is the length of the input string (for tokenization and lowercase conversion). Space complexity is O(n) for storing the tokens and the result string.
