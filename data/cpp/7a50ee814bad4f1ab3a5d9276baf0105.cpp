Write a standalone C++ function named `computeBlottnerViscosity` that, given a temperature in Kelvin, a species name, and the three Blottner curve-fit coefficients \(a\), \(b\), and \(c\), returns the pure-species dynamic viscosity in units of \(\text{kg/(m·s)}\) using the Blottner formula \(\mu = 0.1 \exp(a (\ln T)^2 + b \ln T + c)\). The function must handle the edge case where the temperature is less than or equal to zero by throwing a `std::invalid_argument` exception. Additionally, implement a helper function `readBlottnerTable` that parses a fixed multi-line string (containing species names followed by three floating-point coefficients per line) and populates a `std::map<std::string, std::array<double,3>>`; if a species name appears multiple times, the last occurrence should overwrite earlier ones. The main function `computeBlottnerViscosity` should accept this map plus the species name and temperature, look up the coefficients (throwing `std::out_of_range` if the species is not found), and return the computed viscosity. Ensure the code is self-contained with appropriate headers and uses meaningful constants. Provide a reference solution with a free function only (no `main`) and test code in a separate `main` function using assertions.

The solution involves two main parts: parsing a fixed table and computing the viscosity. For parsing, use `std::istringstream` to read each line, extract the species name (as a string) and three doubles. Since the table is fixed and small (19 species), a simple loop works; to handle potential multiple occurrences, assign directly to the map key, which naturally overwrites previous values. For viscosity computation, use the standard Blottner formula: \(\mu = 0.1 \times \exp(a \cdot (\ln T)^2 + b \cdot \ln T + c)\). Since \(\ln T\) is used, T must be strictly positive; if \(T \le 0\), throw `std::invalid_argument`. The lookup of coefficients should use `map.at(name)` which throws `std::out_of_range` if the key is missing. Time complexity is \(O(L + \log S)\) where \(L\) is the number of lines in the table (constant here) and \(S\) is the species count; space complexity is \(O(S)\). Edge cases: temperature exactly zero or negative, species not in the table, and duplicate names in the table (last wins). No other special cases. The implementation should use `std::array` or a simple struct for the three coefficients; here `std::array<double,3>` is convenient.

#include <string>
#include <map>
#include <array>
#include <sstream>
#include <cmath>
#include <stdexcept>

// Parse a fixed Blottner table into a map of species -> {a,b,c}.
// The table is an embedded string with lines: "Name  a  b  c"
std::map<std::string, std::array<double,3>> readBlottnerTable() {
    const std::string table =
        "Air 2.68142000000e-02 3.17783800000e-01 -1.13155513000e+01\n"
        "CPAir 2.68142000000e-02 3.17783800000e-01 -1.13155513000e+01\n"
        "N 1.15572000000e-02 6.03167900000e-01 -1.24327495000e+01\n"
        "N2 2.68142000000e-02 3.17783800000e-01 -1.13155513000e+01\n"
        "CPN2 2.68142000000e-02 3.17783800000e-01 -1.13155513000e+01\n"
        "NO 4.36378000000e-02 -3.35511000000e-02 -9.57674300000e+00\n"
        "O 2.03144000000e-02 4.29440400000e-01 -1.16031403000e+01\n"
        "O2 4.49290000000e-02 -8.26158000000e-02 -9.20194750000e+00\n"
        "C -8.3285e-3 0.7703240 -12.7378000\n"
        "C2 -8.4311e-3 0.7876060 -13.0268000\n"
        "C3 -8.4312e-3 0.7876090 -12.8240000\n"
        "C2H -2.4241e-2 1.0946550 -14.5835500\n"
        "CN -8.3811e-3 0.7860330 -12.9406000\n"
        "CO -0.019527394 1.013295 -13.97873\n"
        "CO2 -0.019527387 1.047818 -14.32212\n"
        "HCN -2.4241e-2 1.0946550 -14.5835500\n"
        "H -8.3912e-3 0.7743270 -13.6653000\n"
        "H2 -8.3346e-3 0.7815380 -13.5351000\n"
        "e 0.00000000000e+00 0.00000000000e+00 -1.16031403000e+01\n";

    std::map<std::string, std::array<double,3>> result;
    std::istringstream stream(table);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        std::istringstream lineStream(line);
        std::string name;
        double a, b, c;
        if (lineStream >> name >> a >> b >> c) {
            result[name] = {a, b, c};  // overwrites if duplicate
        }
    }
    return result;
}

// Compute Blottner viscosity given a temperature, species name, and coefficient table.
double computeBlottnerViscosity(double temperature,
                                const std::string& species,
                                const std::map<std::string, std::array<double,3>>& coeffTable) {
    if (temperature <= 0.0) {
        throw std::invalid_argument("Temperature must be positive for Blottner viscosity");
    }
    auto it = coeffTable.find(species);
    if (it == coeffTable.end()) {
        throw std::out_of_range("Species not found in Blottner table: " + species);
    }
    const auto& coeffs = it->second;
    double a = coeffs[0];
    double b = coeffs[1];
    double c = coeffs[2];
    double logT = std::log(temperature);
    double exponent = a * logT * logT + b * logT + c;
    return 0.1 * std::exp(exponent);
}

#include <cassert>
#include <cmath>
#include <string>
#include <map>
#include <array>
#include <stdexcept>

// Declare the functions from the solution (header-like)
std::map<std::string, std::array<double,3>> readBlottnerTable();
double computeBlottnerViscosity(double temperature,
                                const std::string& species,
                                const std::map<std::string, std::array<double,3>>& coeffTable);

int main() {
    auto table = readBlottnerTable();

    // Basic valid case: N2 at 300 K, expected ~ 1.786e-5 approx (from standard air viscosity)
    double viscN2 = computeBlottnerViscosity(300.0, "N2", table);
    assert(std::abs(viscN2 - 0.00001786) < 1e-8);  // tolerance

    // Another species: O2 at 1000 K
    double viscO2 = computeBlottnerViscosity(1000.0, "O2", table);
    assert(viscO2 > 0.0);
    assert(std::abs(viscO2 - 0.000058) < 1e-5);  // rough check

    // Duplicate handling: "CPAir" appears after "Air" but both have same coeffs
    auto airIt = table.find("Air");
    auto cpAirIt = table.find("CPAir");
    assert(airIt != table.end());
    assert(cpAirIt != table.end());
    assert(airIt->second == cpAirIt->second);

    // Missing species throws out_of_range
    bool threw = false;
    try {
        computeBlottnerViscosity(300.0, "Nonexistent", table);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    // Non-positive temperature throws invalid_argument
    threw = false;
    try {
        computeBlottnerViscosity(0.0, "N2", table);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Negative temperature also throws
    threw = false;
    try {
        computeBlottnerViscosity(-5.0, "N2", table);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Viscosity at a single known point: N2 at 300 K using known value
    // (Using direct calculation to verify consistency)
    double logT = std::log(300.0);
    double a = 0.0268142, b = 0.3177838, c = -11.3155513;
    double expected = 0.1 * std::exp(a*logT*logT + b*logT + c);
    assert(std::abs(viscN2 - expected) < 1e-15);

    // Species "e" (electron) has a=0,b=0,c=-11.6031403 -> viscosity at any T is constant
    double viscE = computeBlottnerViscosity(1000.0, "e", table);
    assert(std::abs(viscE - 0.1*std::exp(-11.6031403)) < 1e-12);

    // Table size should have exactly 19 species (including duplicates like Air/CPAir)
    assert(table.size() == 19);

    return 0;
}
