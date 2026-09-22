Write a C++ function that takes a positive integer `n` representing the total number of apples and returns a string describing how many full dozens of apples are contained in `n`, but only if `n` is an exact multiple of 12. If `n` is not divisible by 12, the function should return an empty string. Your function should be named `appleDozenDescription` and accept the integer as its parameter. For example, if `n = 24`, the function returns `"2 docena de manzanas"` (note the space between the number and the word "docena", and the phrase "de manzanas" after it). If `n = 25`, it returns `""`. The function should handle all positive integers, including small values like 1 (which returns `""`) and those that are exactly multiples of 12.
#include <cassert>
#include <string>

int main() {
    assert(appleDozenDescription(12) == "1 docena de manzanas");
    assert(appleDozenDescription(24) == "2 docena de manzanas");
    assert(appleDozenDescription(36) == "3 docena de manzanas");
    assert(appleDozenDescription(120) == "10 docena de manzanas");
    assert(appleDozenDescription(1) == "");
    assert(appleDozenDescription(11) == "");
    assert(appleDozenDescription(13) == "");
    assert(appleDozenDescription(23) == "");
    assert(appleDozenDescription(48) == "4 docena de manzanas");
    assert(appleDozenDescription(0) == "0 docena de manzanas");
}
#include <string>

// Returns a string describing the number of full dozens of apples if n is an exact multiple of 12.
// If n is not divisible by 12, returns an empty string.
std::string appleDozenDescription(int n) {
    const int dozens = n / 12;
    if (n % 12 == 0) {
        return std::to_string(dozens) + " docena de manzanas";
    }
    return "";
}
// The solution is straightforward: compute the integer division of `n` by 12 (which gives the number of full dozens) and check whether `n % 12 == 0` to determine if `n` is an exact multiple. If the remainder is zero, we return a string constructed by converting the quotient to a string and appending `" docena de manzanas"`. Otherwise, we return an empty string. Edge cases include `n = 0` (though the task specifies positive integers, if zero were passed, `0` is a multiple of 12 and would return `"0 docena de manzanas"`), and `n` being less than 12 (e.g., `n = 11` gives remainder 11, so empty string; `n = 12` gives quotient 1 and returns correct string). Time complexity is O(1) since only constant-time arithmetic and string construction are performed. Space complexity is O(1) auxiliary, plus the size of the returned string which is small and proportional to the number of digits of the quotient (O(log n) worst-case digits, but constant for typical inputs).
