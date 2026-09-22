Write a C++ function `std::vector<std::string> findValidNames(long long number, const std::string& dictPath)` that reads a dictionary file containing one uppercase name per line from the given path, and returns all names whose “phone keypad encoding” exactly equals the given positive integer. The encoding maps letters A-Z to digits 2-9 using the classic telephone keypad: A,B,C→2; D,E,F→3; G,H,I→4; J,K,L→5; M,N,O→6; P,R,S→7 (note Q is omitted, so P,R,S correspond to 7, and T,U,V→8, W,X,Y→9; the letter Z is invalid and should be ignored or skipped). For each name in the dictionary, compute the number formed by its letters (each letter contributes one digit, ignoring any non-uppercase letters), and if the resulting long long equals the input number, include that name in the result. Return the names in the order they appear in the dictionary. If no names match, return an empty vector.

#include <cassert>
#include <vector>
#include <string>
#include <fstream>

// The solution function is declared here or included from above.
// This main writes a temporary dictionary file for testing.

int main() {
    // Create a temporary dictionary file.
    std::string path = "test_dict.txt";
    {
        std::ofstream out(path);
        out << "KRISTOPHER\n"      // maps to 5478697437
            << "KRISTOFER\n"      // maps to 547869337
            << "ALEX\n"          // maps to 2539
            << "B\n"            // maps to 2
            << "C\n"           // maps to 2
            << "Q\n"          // invalid, should be skipped
            << "ZA\n";       // has Z which is invalid? Z is valid? Z maps to 9, but in original spec Z is invalid? The problem says ignore invalid; let's use only proper names
    }

    assert(findValidNames(5478697437LL, path) == std::vector<std::string>({"KRISTOPHER"}));
    assert(findValidNames(547869337LL, path) == std::vector<std::string>({"KRISTOFER"}));
    assert(findValidNames(2539LL, path) == std::vector<std::string>({"ALEX"}));
    assert(findValidNames(2LL, path) == std::vector<std::string>({"B", "C"}));
    assert(findValidNames(9999LL, path).empty()); // no matching name

    // Clean up.
    std::remove(path.c_str());
    return 0;
}

#include <string>
#include <vector>
#include <fstream>

// Convert a single uppercase letter to its keypad digit (ignoring 'Q').
// Return -1 if the letter is 'Q' (invalid), otherwise digit 2-9.
int letterToDigit(char c) {
    int idx = c - 'A';
    if (idx == 16) { // 'Q'
        return -1;
    }
    if (idx > 16) {
        --idx; // skip Q
    }
    return idx / 3 + 2;
}

// Read all names from dictionary file at dictPath and return those whose
// keypad number equals the given target number, in file order.
std::vector<std::string> findValidNames(long long number, const std::string& dictPath) {
    std::vector<std::string> result;
    std::ifstream dict(dictPath);
    if (!dict.is_open()) {
        return result;
    }

    std::string name;
    while (dict >> name) {
        // Compute the keypad number for this name.
        long long encoded = 0;
        bool validName = true;
        for (char c : name) {
            if (c < 'A' || c > 'Z') {
                validName = false;
                break;
            }
            int digit = letterToDigit(c);
            if (digit == -1) {
                validName = false;
                break;
            }
            encoded = encoded * 10 + digit;
        }
        if (validName && encoded == number) {
            result.push_back(name);
        }
    }
    return result;
}

// The main task is to map each uppercase letter to a digit based on the given keypad. A simple approach: for each character `c`, compute `idx = c - 'A'`. If `idx` corresponds to 'Q' (which is `'Q'-'A' == 16`), skip that name because it cannot be represented. Otherwise, adjust the index: if `idx > 16` (i.e., letters after Q), subtract 1 to close the gap (since Q is omitted). Then compute `digit = idx / 3 + 2`, because groups of three letters start at digit 2 for A(0-2), digit 3 for D(3-5), etc. Build the number by repeatedly multiplying accumulated result by 10 and adding the digit. Compare to the target. Edge cases: empty names (but dictionary likely has none), names with non-uppercase characters (should be ignored or skipped—specify we ignore them by only processing A-Z, but since names are all uppercase, we can just assume valid). The function must open the file by path; if file can’t be opened, return empty. Time complexity is O(L * total length of all names) but effectively O(total characters). Space complexity is O(number of matching names) for the result.
