/*
Write a C++ function named `molecularEnergyDifference` that, given three container-like inputs representing the molecular formula of a reactant (a string of element symbols and their counts, e.g., "C2H2"), a second reactant (e.g., "H2"), and a product (e.g., "C2H4"), along with a precomputed per-atom energy table (e.g., unordered_map from element symbol to energy in eV), computes the energy difference (product energy minus sum of reactant energies) in eV, and returns it as a double. The energy of a molecule is calculated by summing the per-atom energies for each atom in the formula; each formula string is composed of uppercase letters for elements, followed by an optional integer count (defaulting to 1). You must parse the formula correctly, handle multi-letter element symbols (e.g., "C", "H", "O", "N", "Cl"), and assume all referenced elements exist in the energy table. The function should be `const`-correct, avoid modifying inputs, and use appropriate data structures for efficiency.
*/

#include <string>
#include <unordered_map>
#include <cctype>
#include <vector>

// Helper to parse a molecular formula into element counts.
std::unordered_map<std::string, int> parseFormula(const std::string& formula) {
    std::unordered_map<std::string, int> counts;
    size_t i = 0;
    const size_t n = formula.size();
    while (i < n) {
        // Parse element symbol: starts with uppercase letter, optionally followed by lowercase letter.
        if (!isupper(formula[i])) {
            // Malformed input: skip or handle, but assume valid per task.
            ++i;
            continue;
        }
        std::string element(1, formula[i]);
        ++i;
        if (i < n && islower(formula[i])) {
            element += formula[i];
            ++i;
        }
        // Parse count as digits.
        int count = 0;
        while (i < n && isdigit(formula[i])) {
            count = count * 10 + (formula[i] - '0');
            ++i;
        }
        if (count == 0) count = 1; // no digits means count=1
        counts[element] += count;
    }
    return counts;
}

// Compute energy of a single molecule formula.
double moleculeEnergy(const std::string& formula, const std::unordered_map<std::string, double>& atomEnergy) {
    auto counts = parseFormula(formula);
    double total = 0.0;
    for (const auto& pair : counts) {
        auto it = atomEnergy.find(pair.first);
        if (it != atomEnergy.end()) {
            total += pair.second * it->second;
        }
        // If element missing, we could throw, but per spec assume present.
    }
    return total;
}

// Compute energy difference: product - (reactant1 + reactant2), in eV.
double molecularEnergyDifference(const std::string& reactant1,
                                 const std::string& reactant2,
                                 const std::string& product,
                                 const std::unordered_map<std::string, double>& atomEnergy) {
    double e1 = moleculeEnergy(reactant1, atomEnergy);
    double e2 = moleculeEnergy(reactant2, atomEnergy);
    double e3 = moleculeEnergy(product, atomEnergy);
    return e3 - (e1 + e2);
}

#include <cassert>
#include <cmath>

int main() {
    // Energies in eV per atom (simplified).
    std::unordered_map<std::string, double> energyTable;
    energyTable["H"] = -13.6;
    energyTable["C"] = -50.0;
    energyTable["O"] = -70.0;
    energyTable["Cl"] = -90.0;

    // H2 + H2 -> ? not a real reaction, but test formula parsing and sums.
    assert(std::abs(molecularEnergyDifference("H2", "H2", "H4", energyTable) - 0.0) < 1e-9);
    // H2 + H -> H3 (hypothetical) energy diff = (3*-13.6) - (2*-13.6 + 1*-13.6) = 0
    assert(std::abs(molecularEnergyDifference("H2", "H", "H3", energyTable) - 0.0) < 1e-9);
    // Reaction: H2 + O -> H2O (using simple energies: product - reactants)
    // Product H2O: 2*H + 1*O = 2*(-13.6) + (-70) = -97.2
    // Reactants: H2 (2*-13.6=-27.2) + O (-70) = -97.2 -> diff 0
    assert(std::abs(molecularEnergyDifference("H2", "O", "H2O", energyTable) - 0.0) < 1e-9);
    // Different reaction: C + H4 -> CH4, using C=-50, H=-13.6, product = -50 + 4*(-13.6) = -104.4, reactants = -50 + 4*(-13.6) = -104.4 -> diff 0
    assert(std::abs(molecularEnergyDifference("C", "H4", "CH4", energyTable) - 0.0) < 1e-9);
    // Test multi-digit counts: C12H26 + O2 -> products? Not a real balanced eq, just test parsing.
    assert(std::abs(molecularEnergyDifference("C12H26", "O2", "C12H26O2", energyTable) - (0.0)) < 1e-9); // sum same atoms
    // Test two-letter element: Cl + H -> HCl, product = -90 + -13.6 = -103.6, reactants = -90 + -13.6 = -103.6 diff 0
    assert(std::abs(molecularEnergyDifference("Cl", "H", "HCl", energyTable) - 0.0) < 1e-9);
    // Non-zero case: suppose product has extra H, energy diff = -13.6.
    assert(std::abs(molecularEnergyDifference("H2", "H", "H4", energyTable) - (-13.6)) < 1e-9);
    // Another: C2H2 + H2 -> C2H4, product energy = 2*(-50)+4*(-13.6) = -154.4, reactants = 2*(-50)+2*(-13.6) + 2*(-13.6) = -154.4 -> diff 0
    assert(std::abs(molecularEnergyDifference("C2H2", "H2", "C2H4", energyTable) - 0.0) < 1e-9);
    // Order of reactants shouldn't matter.
    assert(std::abs(molecularEnergyDifference("H2", "C2H2", "C2H4", energyTable) - 0.0) < 1e-9);
    return 0;
}

// The solution parses each formula string into a map from element symbol to its count. The parsing algorithm reads characters sequentially: when a letter is encountered, it collects all consecutive uppercase letters (and an optional lowercase letter for two-letter symbols like "Cl") to form the element symbol. Then it parses any following digits as an integer count; if no digits are present, count is 1. After building the counts map for a molecule, its total energy is computed by iterating over the map entries and multiplying each count by the corresponding energy from the provided map. The energy difference is then productEnergy minus the sum of the two reactant energies.
//
// Edge cases: elements with count 1, multi-digit counts (e.g., "C12H26"), elements appearing more than once (not typical, but handle by summing counts), and ensuring that all elements in the formula are found in the energy table (we assume they are, but could add a check). Time complexity is O(L) where L is the total length of all formula strings, plus O(E) for iterating over distinct elements in each molecule’s map, so overall O(L + E) per molecule. Space complexity is O(E) for storing counts and for the energy table, which is given as input.
