// Write a C++ function `topThreeDescending` that takes a string containing whitespace-separated numbers (which may include leading/trailing/multiple spaces, negative values, and duplicates) and returns a string containing the three largest distinct numbers from the input, in descending order and separated by a single space. If fewer than three distinct numbers exist, return only the available distinct numbers (still descending). If the input has no valid integers, return an empty string.
// The problem requires extracting all distinct numeric values from the input string and selecting the three largest among them. The most natural approach is to use a `std::set<double>` with `std::greater<double>` as the comparator, which automatically stores elements in descending order and keeps only unique values. After reading all numbers from the string stream, iterate over the set and collect up to three elements, building the output string with spaces between them. Edge cases include: input with duplicates (they should be ignored), input with fewer than three distinct numbers, input with no numbers (return empty string), and negative numbers. The main algorithm reads the string using `std::istringstream`, inserts each parsed double into the set, then constructs the result by taking the first up-to-three elements. Time complexity is O(n log k) where n is the count of numbers and k the number of distinct values (at most n), and space complexity is O(k) for the set plus O(1) for the output string (ignoring the set’s internal overhead).
#include <sstream>
#include <set>
#include <string>

// Return the three largest distinct numbers from a whitespace-separated string,
// in descending order, separated by single spaces.
// If fewer than three distinct numbers exist, return only the available ones.
// If no numbers are present, return an empty string.
std::string topThreeDescending(const std::string& input) {
    std::istringstream iss(input);
    std::set<double, std::greater<double>> numbers;
    
    double value;
    while (iss >> value) {
        numbers.insert(value);
    }
    
    std::ostringstream result;
    int count = 0;
    for (const double num : numbers) {
        if (count > 0) {
            result << ' ';
        }
        result << num;
        ++count;
        if (count == 3) {
            break;
        }
    }
    return result.str();
}
#include <cassert>
#include <string>

// Declaration of the solution function
std::string topThreeDescending(const std::string& input);

int main() {
    // Normal case with multiple distinct numbers
    assert(topThreeDescending("10 5 8 3 5 10 1") == "10 8 5");
    
    // Negative numbers and fewer than three distinct
    assert(topThreeDescending("-1 -5 -3") == "-1 -3 -5");
    
    // Duplicates and exactly two distinct
    assert(topThreeDescending("7 7 2 2 2") == "7 2");
    
    // Only one distinct number
    assert(topThreeDescending("4 4 4") == "4");
    
    // Extra whitespace and leading/trailing spaces
    assert(topThreeDescending("  12   -4   9   9   ") == "12 9 -4");
    
    // Empty or invalid input
    assert(topThreeDescending("") == "");
    assert(topThreeDescending("   ") == "");
    
    // Large and small values mixed
    assert(topThreeDescending("1.5 2.5 0.5 3.5") == "3.5 2.5 1.5");
    
    // Exactly three distinct numbers
    assert(topThreeDescending("100 200 300") == "300 200 100");
    
    return 0;
}
