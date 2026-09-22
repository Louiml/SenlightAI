Write a C++ function named `rodizioMes` that takes a single character representing the last digit of a vehicle's license plate (as a character from '0' to '9') and returns a `std::string` describing the month or deadline for that plate's vehicular restriction (rodízio) in Portuguese, based on the following mapping: '0' → "dezembro", '1' → "abril", '2' → "maio", '3' → "junho", '4' → "julho", '5' → "agosto", '6' → "agosto", '7' → "setembro", '8' → "outubro", '9' → "novembro". The function must use a `switch` statement and, for any invalid character (not '0'-'9'), return the string "inválida". The function must be `const`-correct (no mutation of parameters, return a `std::string` by value) and include a descriptive comment.

The solution uses a `switch` statement directly on the character parameter, handling each digit case. Since the input is a single character, we must ensure that only the ASCII digits '0' through '9' are treated as valid; all other characters (including letters, punctuation, or multi-digit numbers read as characters) fall into the `default` case, returning `"inválida"`. The mapping is straightforward: each case returns the corresponding month string exactly as given, with no spaces or extra words. The time complexity is O(1) because a switch on a single character is constant time, and the space complexity is O(1) for the returned string (constant size of the largest month name). Edge cases include: passing `'0'` (returns "dezembro"), passing `'5'` and `'6'` (both return "agosto", which is a duplicate mapping), and passing any non-digit (e.g., `'a'`, `' '`, `'\n'`) which yields "inválida". The function does not need any includes beyond `<string>` for the return type; in the test we will include the necessary headers.

#include <string>

// Returns the rodízio deadline month for a given license plate's last digit.
// Input: a character '0' to '9'. If not a digit, returns "inválida".
std::string rodizioMes(char placa) {
    switch (placa) {
        case '0': return "dezembro";
        case '1': return "abril";
        case '2': return "maio";
        case '3': return "junho";
        case '4': return "julho";
        case '5': return "agosto";
        case '6': return "agosto";
        case '7': return "setembro";
        case '8': return "outubro";
        case '9': return "novembro";
        default:  return "inválida";
    }
}

#include <cassert>
#include <string>

// Declaration of the solution function (already defined above).
std::string rodizioMes(char placa);

int main() {
    assert(rodizioMes('0') == "dezembro");
    assert(rodizioMes('1') == "abril");
    assert(rodizioMes('2') == "maio");
    assert(rodizioMes('3') == "junho");
    assert(rodizioMes('4') == "julho");
    assert(rodizioMes('5') == "agosto");
    assert(rodizioMes('6') == "agosto");  // duplicate mapping
    assert(rodizioMes('7') == "setembro");
    assert(rodizioMes('8') == "outubro");
    assert(rodizioMes('9') == "novembro");
    // Edge cases: invalid inputs
    assert(rodizioMes('a') == "inválida");
    assert(rodizioMes(' ') == "inválida");
    assert(rodizioMes('\n') == "inválida");
    assert(rodizioMes('x') == "inválida");
    return 0;
}
