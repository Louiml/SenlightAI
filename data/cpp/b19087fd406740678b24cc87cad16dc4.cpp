Write a C++ function `sortAndFormatStudents` that takes an array of student records (each containing a name, class, date of birth, and GPA) and an integer count, normalizes each student's name to title case (first letter uppercase, remaining lowercase), formats the date of birth to always use two-digit day and month with slashes (e.g., `1/1/2000` becomes `01/01/2000`), then sorts the students by descending GPA (high to low) and returns a `std::vector<std::string>` where each string contains the student’s row formatted exactly as: `ma class name dateOfBirth gpa` with the GPA printed to two decimal places. The `ma` is generated sequentially starting from `B20DCCN001` for the first student, `B20DCCN002` for the second, and so on (always three digits after the prefix, e.g., `B20DCCN010` for the 10th). The original input array must not be modified; copy the data into new student objects before processing. Input names may contain multiple spaces, leading/trailing spaces, and mixed case, and dates may have either one or two digit day/month parts. Assume GPA is between 0.0 and 4.0. The function must be robust to empty arrays (return an empty vector).
The solution processes each student in three passes: first, create a copy of each input student (to preserve the original), generate the `ma` sequentially by formatting the index with zero-padded width of 3 (using `std::to_string` and padding), then trim/normalize the name: split by spaces using `std::istringstream`, convert to lowercase, then set the first character to uppercase, and join with single spaces, removing any empty tokens. Next, normalize the date: if the date has fewer than 10 characters (missing leading zeros), insert `'0'` at position 0 if the second character is `'/'`, and insert `'0'` at position 3 if the fifth character is `'/'`. Then, after processing all students, sort the copies in descending order of GPA using `std::sort` with a comparator that returns `a.gpa > b.gpa`. Finally, format each student into a string using `std::ostringstream`, outputting `ma`, `name`, `class`, `date`, and `gpa` with `std::fixed` and `std::setprecision(2)`, separated by spaces, and return the vector of strings. Edge cases include empty input (return empty vector), names with multiple consecutive spaces (handle by splitting on whitespace and ignoring empty tokens), dates already properly formatted (no insert needed), and GPA ties (order among ties is not specified, but `std::sort` is not stable; ties may be in any order, which is acceptable). Time complexity is \(O(n \log n)\) for sorting plus \(O(n \cdot m)\) for name processing where \(m\) is average name length; space complexity is \(O(n \cdot m)\) for the copies and output strings.
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

struct Student {
    std::string name;
    std::string cls;
    std::string date;
    float gpa;
};

// Process students: normalize names and dates, sort by descending GPA, return formatted rows.
std::vector<std::string> sortAndFormatStudents(const Student students[], int count) {
    std::vector<Student> copy(count);
    std::vector<std::string> result;

    if (count <= 0) return result;

    for (int i = 0; i < count; ++i) {
        copy[i] = students[i];
    }

    // Normalize names and dates, assign sequential codes
    for (int i = 0; i < count; ++i) {
        // Normalize name: split by spaces, title-case each token
        std::istringstream nameStream(copy[i].name);
        std::string token;
        copy[i].name.clear();
        while (nameStream >> token) {
            for (size_t j = 0; j < token.size(); ++j) {
                token[j] = std::tolower(static_cast<unsigned char>(token[j]));
            }
            token[0] = std::toupper(static_cast<unsigned char>(token[0]));
            copy[i].name += token;
            copy[i].name += ' ';
        }
        if (!copy[i].name.empty()) {
            copy[i].name.pop_back(); // remove trailing space
        }

        // Normalize date: ensure day and month have two digits
        if (copy[i].date.size() >= 5) {
            if (copy[i].date[1] == '/') {
                copy[i].date.insert(0, 1, '0');
            }
            if (copy[i].date.size() >= 6 && copy[i].date[4] == '/') {
                copy[i].date.insert(3, 1, '0');
            }
        }
    }

    // Sort by descending GPA (high to low)
    std::sort(copy.begin(), copy.end(), [](const Student& a, const Student& b) {
        return a.gpa > b.gpa;
    });

    // Format output rows
    for (int i = 0; i < count; ++i) {
        // Generate sequential code with zero-padded index
        std::string code = "B20DCCN" + std::string(3 - std::to_string(i + 1).size(), '0') + std::to_string(i + 1);

        std::ostringstream oss;
        oss << code << " " << copy[i].name << " " << copy[i].cls << " " << copy[i].date << " "
            << std::fixed << std::setprecision(2) << copy[i].gpa;
        result.push_back(oss.str());
    }

    return result;
}
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// The solution function is assumed to be defined above.
// We'll include a copy here for the test harness (in a real exercise, it would be in the same file).

int main() {
    // Test 1: normal case with mixed case names and short dates
    Student students1[3] = {
        {"  nguyen van   an  ", "D20CQ01", "1/2/2001", 3.5},
        {"tran Thi Binh", "D20CQ02", "11/12/2000", 3.8},
        {"LE THI CHI", "D20CQ03", "3/4/2002", 2.9}
    };
    std::vector<std::string> result1 = sortAndFormatStudents(students1, 3);
    assert(result1.size() == 3);
    assert(result1[0] == "B20DCCN001 Tran Thi Binh D20CQ02 11/12/2000 3.80");
    assert(result1[1] == "B20DCCN002 Nguyen Van An D20CQ01 01/02/2001 3.50");
    assert(result1[2] == "B20DCCN003 Le Thi Chi D20CQ03 03/04/2002 2.90");

    // Test 2: empty input
    std::vector<std::string> result2 = sortAndFormatStudents(nullptr, 0);
    assert(result2.empty());

    // Test 3: single student with already formatted date and name with single spaces
    Student one[1] = { {"  James  Bond  ", "A12", "05/07/1999", 4.0} };
    std::vector<std::string> result3 = sortAndFormatStudents(one, 1);
    assert(result3.size() == 1);
    assert(result3[0] == "B20DCCN001 James Bond A12 05/07/1999 4.00");

    // Test 4: code generation for ten students (edge for zero-padding)
    Student many[10];
    for (int i = 0; i < 10; ++i) {
        many[i] = {"Student Name", "C10", "1/1/2000", 2.5f};
    }
    std::vector<std::string> result4 = sortAndFormatStudents(many, 10);
    assert(result4[0].substr(0, 14) == "B20DCCN001 ");
    assert(result4[9].substr(0, 14) == "B20DCCN010 ");

    // Test 5: same GPA values (order not guaranteed, but count must match)
    Student equal[2] = {
        {"Alice", "E1", "1/1/2000", 3.2},
        {"Bob", "E2", "2/2/2000", 3.2}
    };
    std::vector<std::string> result5 = sortAndFormatStudents(equal, 2);
    assert(result5.size() == 2);
    // Both rows must still appear with correct data (order can be either)
    std::string row1 = result5[0];
    std::string row2 = result5[1];
    assert(row1.find("Alice") != std::string::npos || row1.find("Bob") != std::string::npos);
    assert(row2.find("Alice") != std::string::npos || row2.find("Bob") != std::string::npos);
    assert(row1 != row2);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
