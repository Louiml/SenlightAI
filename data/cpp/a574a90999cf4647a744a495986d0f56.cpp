Create a C++ function `fractionMultiplicationTable` that takes an integer denominator `den` (assumed to be at least 2) and returns a `std::vector<std::string>` representing the fractional multiplication table formatted exactly as shown. Each string in the vector corresponds to a row of the table, with entries separated by a single space character. The first row must contain the header labels (all proper fractions with the given denominator, in ascending order from `1/den` to `(den-1)/den`), followed by a row of dashes (`-----------------------------------------`), and then `den-1` rows, each starting with the row label followed by the products of that row's fraction with each column's fraction. All fractions must be fully reduced (e.g., `2/6` becomes `1/3`), and each product must be simplified. The function must handle any integer `den >= 2` and produce the table without leading/trailing spaces on each line. For example, for `den = 6`, the expected output is exactly the table shown in the prompt.
// The core solution involves generating all proper fractions with the given denominator (i.e., numerators from 1 to den-1) and representing each as a `Fraction` object that automatically reduces itself upon construction using the Euclidean algorithm (or a subtraction-based GCD). For each pair of fractions (row and column), we multiply the numerators and denominators, create a new `Fraction` to reduce the result, and format it as `"num/den"`. The table structure is built row by row: first the header row with the fractions, then a separator line of dashes, then for each row fraction, the row label followed by all products. We must ensure that the `Fraction` class handles edge cases such as zero numerator (not possible here since numerators start at 1), and that the reduction works for any positive integers. The time complexity is \(O(n^3)\) due to the nested loops over \(n = den-1\) fractions for each column and row, plus GCD reduction which is \(O(\log \min(num, den))\); overall for a given `den`, it is \(O(den^3 \log den)\). The space complexity is \(O(den^2)\) for storing the output strings.
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>

class Fraction {
private:
    int num;
    int den;

    void reduce() {
        if (den == 0) {
            std::cerr << "Invalid denominator\n";
            std::exit(1);
        }
        if (num == 0) {
            den = 1;
            return;
        }
        int a = std::abs(num);
        int b = std::abs(den);
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        int gcd = a;
        num /= gcd;
        den /= gcd;
    }

public:
    Fraction(int n, int d = 1) : num(n), den(d) {
        reduce();
    }

    Fraction multiply(const Fraction& other) const {
        return Fraction(num * other.num, den * other.den);
    }

    std::string toString() const {
        return std::to_string(num) + "/" + std::to_string(den);
    }
};

// Build the fractional multiplication table for a given denominator.
std::vector<std::string> fractionMultiplicationTable(int den) {
    std::vector<Fraction> fractions;
    for (int i = 1; i < den; ++i) {
        fractions.emplace_back(i, den);
    }

    std::vector<std::string> table;

    // Header row: list all fractions
    std::string header;
    for (size_t i = 0; i < fractions.size(); ++i) {
        if (i > 0) header += " ";
        header += fractions[i].toString();
    }
    table.push_back(header);

    // Separator line
    table.push_back("-----------------------------------------");

    // Data rows
    for (const auto& rowFrac : fractions) {
        std::string row = rowFrac.toString();
        for (const auto& colFrac : fractions) {
            Fraction product = rowFrac.multiply(colFrac);
            row += " " + product.toString();
        }
        table.push_back(row);
    }

    return table;
}
#include <cassert>
#include <string>
#include <vector>

// Assume the solution function is declared above (included from the same compilation unit).

int main() {
    // Test for den = 6 as in the prompt
    std::vector<std::string> table = fractionMultiplicationTable(6);
    assert(table.size() == 7); // header + separator + 5 data rows
    assert(table[0] == "1/6 1/3 1/2 2/3 5/6");
    assert(table[1] == "-----------------------------------------");
    assert(table[2] == "1/6 1/36 1/18 1/12 1/9 5/36");
    assert(table[3] == "1/3 1/18 1/9 1/6 2/9 5/18");
    assert(table[4] == "1/2 1/12 1/6 1/4 1/3 5/12");
    assert(table[5] == "2/3 1/9 2/9 1/3 4/9 5/9");
    assert(table[6] == "5/6 5/36 5/18 5/12 5/9 25/36");

    // Test for den = 2 (only one fraction)
    std::vector<std::string> table2 = fractionMultiplicationTable(2);
    assert(table2.size() == 3);
    assert(table2[0] == "1/2");
    assert(table2[1] == "-----------------------------------------");
    assert(table2[2] == "1/2 1/4");

    // Test for den = 3
    std::vector<std::string> table3 = fractionMultiplicationTable(3);
    assert(table3.size() == 4);
    assert(table3[0] == "1/3 2/3");
    assert(table3[1] == "-----------------------------------------");
    assert(table3[2] == "1/3 1/9 2/9");
    assert(table3[3] == "2/3 2/9 4/9");

    // Test for den = 4 (check reduction)
    std::vector<std::string> table4 = fractionMultiplicationTable(4);
    assert(table4[0] == "1/4 1/2 3/4");
    assert(table4[2] == "1/4 1/16 1/8 3/16");
    assert(table4[3] == "1/2 1/8 1/4 3/8");
    assert(table4[4] == "3/4 3/16 3/8 9/16");

    // Test for den = 5 (no products reduce beyond first factors)
    std::vector<std::string> table5 = fractionMultiplicationTable(5);
    assert(table5.size() == 6);
    assert(table5[0] == "1/5 2/5 3/5 4/5");
    assert(table5[2] == "1/5 1/25 2/25 3/25 4/25");
    assert(table5[3] == "2/5 2/25 4/25 6/25 8/25");
    assert(table5[4] == "3/5 3/25 6/25 9/25 12/25");
    assert(table5[5] == "4/5 4/25 8/25 12/25 16/25");

    return 0;
}
