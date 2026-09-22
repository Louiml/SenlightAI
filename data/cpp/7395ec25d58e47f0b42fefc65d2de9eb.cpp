/*
Write a C++ function that simulates a payroll system for exactly seven employees. The function should accept no arguments and return a `std::vector<std::pair<long, double>>` (or, if preferred, two parallel vectors) where each element contains an employee ID and the calculated gross wages. The employee IDs are fixed and must be: 5658845, 4520125, 7895122, 8777541, 8451277, 1302850, 7580489. For each employee, the function must prompt the user to enter the number of hours worked (a non-negative integer) and the hourly pay rate (a double at least 6.00). Input validation must be implemented: reject negative hours and pay rates below 6.00, re-prompting until valid input is given. Gross wages are computed as hours × pay rate. After processing all seven employees, the function must output a clear table showing each employee ID and their gross wages, then return the collected data. The function must be self-contained (no external global state) and must use `const` where appropriate (e.g., for the employee ID array).
*/
#include <vector>
#include <iostream>
#include <utility>

// Simulate payroll for seven fixed employees.
// Returns a vector of {employee ID, gross wages} pairs.
std::vector<std::pair<long, double>> calculatePayroll() {
    const long empIDs[7] = {5658845, 4520125, 7895122, 8777541, 8451277, 1302850, 7580489};
    std::vector<std::pair<long, double>> payroll;
    payroll.reserve(7);

    for (int i = 0; i < 7; ++i) {
        int hours;
        double payRate;

        std::cout << "Enter the working hours of Employee ID " << empIDs[i] << ": ";
        std::cin >> hours;
        while (hours < 0) {
            std::cout << "ERROR: Hours cannot be negative. Enter again: ";
            std::cin >> hours;
        }

        std::cout << "Enter the pay rate of Employee ID " << empIDs[i] << ": ";
        std::cin >> payRate;
        while (payRate < 6.00) {
            std::cout << "ERROR: Pay rate cannot be less than 6.00. Enter again: ";
            std::cin >> payRate;
        }

        payroll.emplace_back(empIDs[i], hours * payRate);
    }

    std::cout << "\n\n  Emp_ID\t\tWages\n";
    for (const auto& entry : payroll) {
        std::cout << entry.first << "\t\t\t" << entry.second << "\n";
    }
    std::cout << "\n\n";
    return payroll;
}
#include <cassert>
#include <vector>
#include <utility>
#include <sstream>
#include <iostream>

// Include the solution function here (or link appropriately).
// For test purposes, we assume the function is defined above.

int main() {
    // Since the function reads from std::cin, we need to simulate input.
    // We'll redirect cin to a stringstream with valid inputs.
    std::stringstream input;
    input << "40\n10.5\n";   // Emp 5658845: hours=40, rate=10.5 -> wage=420
    input << "35\n15\n";     // Emp 4520125: hours=35, rate=15 -> wage=525
    input << "-5\n40\n12\n"; // Emp 7895122: first hours negative, retry with 40, rate=12 -> wage=480
    input << "20\n6.00\n";   // Emp 8777541: hours=20, rate=6.00 -> wage=120
    input << "0\n100\n";     // Emp 8451277: hours=0, rate=100 -> wage=0
    input << "50\n8.75\n";   // Emp 1302850: hours=50, rate=8.75 -> wage=437.5
    input << "30\n7.00\n";   // Emp 7580489: hours=30, rate=7.00 -> wage=210

    auto oldCinBuf = std::cin.rdbuf(input.rdbuf());

    auto result = calculatePayroll();

    std::cin.rdbuf(oldCinBuf); // restore original cin

    assert(result.size() == 7);
    assert(result[0].first == 5658845 && result[0].second == 420.0);
    assert(result[1].first == 4520125 && result[1].second == 525.0);
    assert(result[2].first == 7895122 && result[2].second == 480.0);
    assert(result[3].first == 8777541 && result[3].second == 120.0);
    assert(result[4].first == 8451277 && result[4].second == 0.0);
    assert(result[5].first == 1302850 && result[5].second == 437.5);
    assert(result[6].first == 7580489 && result[6].second == 210.0);

    // Edge case: pay rate exactly 6.00 is accepted, negative hours rejected.
    // Already tested above with employee 3 (negative hours retry) and employee 4 (rate 6.00).

    return 0;
}
// The solution uses a fixed-size array of seven employee IDs initialized at compile time. The algorithm loops exactly seven times. In each iteration, it prompts for hours, reads the value, and while the value is negative, prints an error and re-prompts. Similarly, it prompts for pay rate, reads it, and while it is less than 6.0, rejects it. Once valid, hours and pay rate are multiplied to compute wages, which are stored in a `std::vector<std::pair<long, double>>`. The function then prints a formatted header and iterates over the vector to display each ID and wage. Edge cases include the first input attempt being invalid (the while loop handles it), and the possibility of non-numeric input (though not required by the specification, the reference solution will simply rely on `cin` failing; for robustness one could clear the stream, but the task only specifies numeric validation). Time complexity is O(7) = O(1) because the number of employees is fixed, but if generalized to `n` employees, it would be O(n). Space complexity is O(1) for the internal employee ID array, plus O(7) for the output vector, also constant.
