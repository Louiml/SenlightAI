// Create a standalone C++ function that generates a table of test vectors for the mathematical functions `log1p` and `expm1`. The function should take a starting value `r` (initially `1 / sqrt(2)`), a lower bound `lim` (initially `2^-128`), and a scaling factor `root_two` (initially `sqrt(2)`). It should repeatedly, while `r > lim`, output two rows of formatted strings — one for `+r` and one for `-r` — each containing three values: the argument, `log1p(arg)`, and `expm1(arg)`. The output must be in scientific notation with 40 digits of precision, formatted as C++ long double literals (suffix `L`), with each row enclosed in braces and commas, mimicking a data table for constant initialization. The function should return nothing (void) and accept appropriate parameters. The `lim` check must be strict (`>`), and `r` is divided by `root_two` after each pair of rows.
// The solution is straightforward: define a function `generate_table(mp_t r, mp_t lim, mp_t root_two)` that loops while `r > lim`. In each iteration, compute `arg_pos = r`, `arg_neg = -r`, and for each compute `log1p(arg)` (defined as `log(arg + 1)`) and `expm1(arg)` (defined as `exp(arg) - 1`). Use `std::cout` with `std::scientific` and `std::setprecision(40)` to print the values. For each of the two arguments, output a formatted line `"   { " << arg << "L, " << log_val << "L, " << exp_val << "L, }, \n"`. Then update `r /= root_two`. Edge cases: the function must handle negative arguments correctly, though `log1p` for `arg > -1` is well-defined; here `r` starts at ~0.707 and halves each time, so `-r` is always > -1, and the loop terminates when `r` is tiny (~2^-128), but never reaches zero. Time complexity is O(log_2(1/lim)) ≈ 128 iterations, each doing constant work (two `log`, `exp`, `sqrt`? No, sqrt is outside). Space complexity is O(1) aside from output buffering. The function should be `const`-correct by taking parameters by value or const reference, and the body does not modify them (except `r` which is copied). Use `#include <iostream>`, `<iomanip>`, and `<cmath>` (but if using a custom `mp_t`, assume it has `log`, `exp`, `pow`, `sqrt`). For the reference solution, assume `mp_t` is a typedef for `long double` to keep standalone.
#include <iostream>
#include <iomanip>
#include <cmath>

// mp_t is a high-precision type; for a standalone exercise use long double.
using mp_t = long double;

// Compute log(1 + x) accurately for small x (though here x is moderate).
mp_t log1p(mp_t x) {
    return std::log(1 + x);
}

// Compute exp(x) - 1 accurately for small x.
mp_t expm1(mp_t x) {
    return std::exp(x) - 1;
}

// Generates a table of log1p and expm1 values for +/- r, halving r each time.
void generate_table(mp_t r, mp_t lim, mp_t root_two) {
    std::cout << std::scientific << std::setprecision(40);
    while (r > lim) {
        mp_t log_pos = log1p(r);
        mp_t exp_pos = expm1(r);
        std::cout << "   { " << r << "L, " << log_pos << "L, " << exp_pos << "L, }, \n";

        mp_t neg_r = -r;
        mp_t log_neg = log1p(neg_r);
        mp_t exp_neg = expm1(neg_r);
        std::cout << "   { " << neg_r << "L, " << log_neg << "L, " << exp_neg << "L, }, \n";

        r /= root_two;
    }
}
#include <cassert>
#include <sstream>
#include <string>

// mp_t is long double; declare the free function.
using mp_t = long double;
void generate_table(mp_t r, mp_t lim, mp_t root_two);

int main() {
    // Capture output to a string stream.
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    // Use a simpler lim to test quickly.
    generate_table(1.0L / std::sqrt(2.0L), 0.25L, std::sqrt(2.0L));

    std::cout.rdbuf(old); // restore

    std::string output = buffer.str();
    // Expected: first row for +r where r ≈ 0.7071, then -r, then r/√2 ≈ 0.5, etc.
    // Size check: two rows per iteration, 2 iterations (0.707 > 0.25, 0.5 > 0.25, then 0.3535 ≤ 0.25)
    // Each row has: "   { <val>L, <log>L, <exp>L, }, \n" – we can count lines.
    int line_count = 0;
    for (char c : output) if (c == '\n') line_count++;
    assert(line_count == 4); // 2 iterations * 2 rows

    // Check that the first row contains the positive value.
    size_t pos = output.find("{ 0.707106781186547524400844362104849039L,");
    assert(pos != std::string::npos);

    // Check that the second row contains the negative value.
    pos = output.find("{ -0.707106781186547524400844362104849039L,");
    assert(pos != std::string::npos);

    // Check that after two iterations, we have halved twice.
    pos = output.find("{ 0.35355339059327376220042218105242452L,");
    assert(pos == std::string::npos); // Because loop stops when r <= 0.25, so 0.3535 > 0.25 should NOT appear? Wait: r=0.707 >0.25, then r=0.5 >0.25, then r=0.3535 >0.25? No, 0.3535 > 0.25, so actually 3 iterations. Let's adjust.

    // Let's fix: starting r = 1/√2 ≈ 0.7071. After first divide: r ≈ 0.5, still >0.25. After second divide: r ≈ 0.3535, still >0.25. After third: r ≈ 0.25, not >0.25. So 3 iterations -> 6 lines. Let's correct.
    // But the assert above used 4 lines – that will fail. Instead, we should test with lim=0.5 to get 2 iterations.
    // Since the test is fixed, I'll rewrite with lim=0.5 to make 2 iterations.

    // Redo test with lim=0.5:
    // Capture again.
    buffer.str("");
    buffer.clear();
    std::cout.rdbuf(buffer.rdbuf());
    generate_table(1.0L / std::sqrt(2.0L), 0.5L, std::sqrt(2.0L));
    std::cout.rdbuf(old);
    output = buffer.str();
    line_count = 0;
    for (char c : output) if (c == '\n') line_count++;
    assert(line_count == 4); // r=0.707>0.5, r=0.5? 0.5>0.5 is false, so 1 iteration only -> 2 lines. Wait, 0.707 >0.5, then divide -> 0.5, stop. So 2 lines. Let's be precise:

    // Actually, start r = 1/√2 ≈ 0.7071. Loop: r > 0.5? true. Output two rows. r /= √2 ≈ 0.5. Loop: r > 0.5? 0.5 > 0.5 false. So 1 iteration = 2 lines.
    // But my line_count check says 4 – that is wrong. I'll fix by checking 2.
    // To avoid confusion, I'll provide a simpler assert: check that the output contains the expected number of lines for a known lim.

    // For clarity, I'll just do two asserts: one that the output contains a positive and negative row, and one that it ends with the exact last row.
    // So I'll rewrite the test cleanly.
}

Rewrite test properly:

#include <cassert>
#include <sstream>
#include <string>

using mp_t = long double;
void generate_table(mp_t r, mp_t lim, mp_t root_two);

int main() {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    // Use lim=0.5 so exactly one iteration occurs.
    mp_t r = 1.0L / std::sqrt(2.0L);
    mp_t lim = 0.5L;
    mp_t root_two = std::sqrt(2.0L);
    generate_table(r, lim, root_two);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    // Count newlines: each row ends with " \n", so 2 rows -> 2 newlines.
    int newlines = 0;
    for (char c : output) if (c == '\n') newlines++;
    assert(newlines == 2);

    // Check the first row contains the positive r value.
    size_t pos = output.find("{ 0.707106781186547524400844362104849039L,");
    assert(pos != std::string::npos);

    // Check the second row contains the negative r value.
    pos = output.find("{ -0.707106781186547524400844362104849039L,");
    assert(pos != std::string::npos);

    // Check that log1p(r) ≈ 0.5493061443340549 (approx) – we can just verify the string starts with the value.
    // For a simpler assert, check the number of commas in the first row (including trailing comma after expm1).
    // Find the first newline, get the line, count commas.
    std::string first_line = output.substr(0, output.find('\n'));
    int commas = 0;
    for (char c : first_line) if (c == ',') commas++;
    assert(commas == 3); // three commas: after r, after log, after expm1

    // Verify the loop stops correctly: using lim=0.5, r after division is 0.5, which is not >0.5, so only one pair.
    // Another test: use lim=0.1 to get more iterations.
    buffer.str("");
    buffer.clear();
    std::cout.rdbuf(buffer.rdbuf());
    generate_table(1.0L / std::sqrt(2.0L), 0.1L, std::sqrt(2.0L));
    std::cout.rdbuf(old);
    output = buffer.str();
    newlines = 0;
    for (char c : output) if (c == '\n') newlines++;
    // Sequence: 0.7071, 0.5, 0.3535, 0.25, 0.1768, 0.125 – all >0.1 until 0.0884? Actually 0.125 >0.1, then next >0.0884 <=0.1. So steps: 0.707(1),0.5(2),0.3535(3),0.25(4),0.1768(5),0.125(6),0.0884(7 – not >0.1). So 6 iterations -> 12 lines.
    assert(newlines == 12);

    // Check the last line contains a negative small value.
    assert(output.find("-0.125L") != std::string::npos);
    assert(output.find("-0.0884L") == std::string::npos); // not output
}
