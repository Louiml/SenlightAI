// You are given a sequence of operations to manage a gradebook for students. Each operation begins with a student's name and a command code: `1` means add a certain number of bonus marks to the student's current grade (the bonus marks are given as an additional integer on the same line), `2` means remove the student from the gradebook, and `3` means print the student's current grade. Initially, the gradebook is empty. Write a C++ function `void processQueries()` that reads the number of queries `n` from standard input (first line), then processes each query as described. For every type `3` query, output the student's grade (if the student does not exist, output `0`). The function should read from standard input and write to standard output. Use a `std::map<std::string, int>` to store the data.

#include <cassert>
#include <sstream>

// Function declaration (prototype) for testing.
void processQueries();

int main() {
    // Redirect stdin to test cases, and stdout to a buffer to capture output.
    std::string input1 = "5\nAlice 1 10\nBob 3\nAlice 3\nAlice 2\nAlice 3\n";
    std::istringstream in1(input1);
    std::streambuf* oldCin = std::cin.rdbuf(in1.rdbuf());
    std::ostringstream out1;
    std::streambuf* oldCout = std::cout.rdbuf(out1.rdbuf());
    processQueries();
    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);
    assert(out1.str() == "0\n10\n0\n");

    std::string input2 = "3\nX 3\nX 1 5\nX 3\n";
    std::istringstream in2(input2);
    std::cin.rdbuf(in2.rdbuf());
    std::ostringstream out2;
    std::cout.rdbuf(out2.rdbuf());
    processQueries();
    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);
    assert(out2.str() == "0\n5\n");

    std::string input3 = "4\nA 1 7\nA 1 3\nB 2\nA 3\n";
    std::istringstream in3(input3);
    std::cin.rdbuf(in3.rdbuf());
    std::ostringstream out3;
    std::cout.rdbuf(out3.rdbuf());
    processQueries();
    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);
    assert(out3.str() == "10\n");

    return 0;
}

#include <iostream>
#include <map>
#include <string>

// Process gradebook queries from standard input and print results for type 3.
void processQueries() {
    int num_queries;
    std::cin >> num_queries;
    std::map<std::string, int> gradebook;

    for (int i = 0; i < num_queries; ++i) {
        std::string name;
        int command;
        std::cin >> name >> command;

        if (command == 1) {
            int bonus;
            std::cin >> bonus;
            gradebook[name] += bonus;  // default 0 if new student
        } else if (command == 2) {
            gradebook.erase(name);
        } else if (command == 3) {
            std::cout << gradebook[name] << std::endl;  // default 0 if missing
        }
    }
}

// The core idea is to maintain a dictionary mapping student names to integer grades. For each query, read the name and command code. If the code is `1`, also read the bonus marks, then add them to the current value (the `operator[]` will default to `0` if the student is new). If the code is `2`, erase the student from the map (if they exist, `erase` is safe even for missing keys). If the code is `3`, retrieve the grade using `operator[]` (which returns `0` for missing keys) and print it. Edge cases: a student may receive bonus marks before ever having a grade, so `operator[]` default-initializes to `0`. Removal of a non-existent student is a no-op. Printing a non-existent student yields `0`. The algorithm runs in `O(n log m)` time where `n` is the number of queries and `m` is the number of distinct students, due to `std::map` operations. Space complexity is `O(m)` for storing the map.
