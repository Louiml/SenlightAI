/*
Write a C++ function `sortEmployeesByBirthDate` that takes an array of employee records and sorts them in non-decreasing order by birth date, comparing first by year, then month, then day. Each employee record has fields for ID, name, gender, birth date in `dd/mm/yyyy` format, address, tax ID, and contract date. The function must modify the array in place and preserve the relative order of employees with identical birth dates (stable sort). The input array size is provided, and the function is expected to handle between 1 and 50 employees.
*/

#include <string>
#include <sstream>
#include <algorithm>

struct Employee {
    std::string id;
    std::string name;
    std::string gender;
    std::string birthDate;  // dd/mm/yyyy
    std::string address;
    std::string taxId;
    std::string contractDate;
};

// Compare two employees by birth date (year, month, day)
bool employeeBirthDateLess(const Employee& a, const Employee& b) {
    // Extract day, month, year from "dd/mm/yyyy"
    std::istringstream dateStreamA(a.birthDate);
    std::istringstream dateStreamB(b.birthDate);

    std::string dayA, monthA, yearA;
    std::string dayB, monthB, yearB;

    std::getline(dateStreamA, dayA, '/');
    std::getline(dateStreamA, monthA, '/');
    std::getline(dateStreamA, yearA, '/');

    std::getline(dateStreamB, dayB, '/');
    std::getline(dateStreamB, monthB, '/');
    std::getline(dateStreamB, yearB, '/');

    // Compare year, then month, then day (lexicographic works due to zero-padding)
    if (yearA != yearB) return yearA < yearB;
    if (monthA != monthB) return monthA < monthB;
    return dayA < dayB;
}

// Sort an array of employees by birth date (stable)
void sortEmployeesByBirthDate(Employee* employees, int n) {
    std::stable_sort(employees, employees + n, employeeBirthDateLess);
}

#include <cassert>
#include <string>

// Include the solution code here (or link appropriately)

int main() {
    // Test 1: Basic sorting by year
    Employee arr1[3] = {
        {"001", "Alice", "F", "15/03/1990", "Addr1", "Tax1", "01/01/2020"},
        {"002", "Bob", "M", "20/07/1985", "Addr2", "Tax2", "02/02/2021"},
        {"003", "Carol", "F", "10/01/1992", "Addr3", "Tax3", "03/03/2022"}
    };
    sortEmployeesByBirthDate(arr1, 3);
    assert(arr1[0].id == "002");
    assert(arr1[1].id == "001");
    assert(arr1[2].id == "003");

    // Test 2: Same year, different months
    Employee arr2[2] = {
        {"A", "X", "M", "01/12/2000", "A1", "T1", "C1"},
        {"B", "Y", "F", "01/01/2000", "A2", "T2", "C2"}
    };
    sortEmployeesByBirthDate(arr2, 2);
    assert(arr2[0].id == "B");
    assert(arr2[1].id == "A");

    // Test 3: Same year and month, different days
    Employee arr3[2] = {
        {"X", "P", "M", "25/10/1995", "A1", "T1", "C1"},
        {"Y", "Q", "F", "10/10/1995", "A2", "T2", "C2"}
    };
    sortEmployeesByBirthDate(arr3, 2);
    assert(arr3[0].id == "Y");
    assert(arr3[1].id == "X");

    // Test 4: Duplicate birth dates – stable order preserved
    Employee arr4[3] = {
        {"1", "A", "M", "15/05/1988", "A1", "T1", "C1"},
        {"2", "B", "F", "15/05/1988", "A2", "T2", "C2"},
        {"3", "C", "M", "02/02/1987", "A3", "T3", "C3"}
    };
    sortEmployeesByBirthDate(arr4, 3);
    assert(arr4[0].id == "3");
    assert(arr4[1].id == "1");
    assert(arr4[2].id == "2");

    // Test 5: Single employee (no change)
    Employee arr5[1] = {{"007", "Solo", "M", "31/12/2001", "Addr", "Tax", "Contract"}};
    sortEmployeesByBirthDate(arr5, 1);
    assert(arr5[0].id == "007");

    // Test 6: Dates with leading zeros
    Employee arr6[2] = {
        {"A", "One", "F", "01/02/2000", "Add1", "Tax1", "C1"},
        {"B", "Two", "M", "01/02/2000", "Add2", "Tax2", "C2"}
    };
    sortEmployeesByBirthDate(arr6, 2);
    assert(arr6[0].id == "A"); // stable, same date

    return 0;
}

// The main challenge is parsing the birth date string into its components (day, month, year) to enable comparison. Since the date is in `dd/mm/yyyy` format, we can use a string stream with `/` as the delimiter to extract the day, month, and year into separate strings, then convert them to integers or compare lexicographically because zero-padded two-digit day/month and four-digit year allow direct string comparison. The comparison function should first compare years, then months, then days – if tie, return false (to maintain stability). We can use `std::stable_sort` to guarantee stable ordering, which is O(n log n) in average case. Edge cases include only one employee (no sorting needed), duplicate birth dates (stable order preserved), and dates with leading zeros (handled by string extraction). Time complexity is O(n log n) for sorting plus O(n) for parsing, and auxiliary space is O(n) for the sorting algorithm's internal storage.
