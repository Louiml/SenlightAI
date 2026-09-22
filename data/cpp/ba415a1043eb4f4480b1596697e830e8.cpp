Write a standalone C++ function that computes the per-atom coefficients (betas) and optional per-atom energies for a linear machine-learning interatomic potential model. The function takes the number of atoms, the number of descriptors (features) per atom, a flat 2D coefficient matrix (stored row-major with one bias term per element plus `ndescriptors` weights per element), a flat 2D descriptor matrix (row-major, size `natoms * ndescriptors`), an integer array specifying each atom's element type (zero-based), and a boolean flag indicating whether energy computation is requested. The function must fill two output arrays: `betas` (size `natoms * ndescriptors`), where each atom’s row is copied from the corresponding element’s coefficient row (skipping the bias), and `eatoms` (size `natoms`), where each atom’s energy is the bias plus the dot product of the element’s weights with that atom’s descriptors. The function should also return the total energy (sum of all per-atom energies) if the flag is true, otherwise return 0.0. Edge cases: handle zero descriptors gracefully (betas empty, energy equals bias), and ensure the code is robust to any valid sizes.

The core algorithm is straightforward: iterate over each atom, determine its element type, copy the element-specific weights from the coefficient matrix into the corresponding beta row, and if energy is requested, compute `bias + sum(weights[j] * descriptors[i][j])`. Since the coefficient matrix stores bias at column 0 and weights from column 1 onward, the copy uses an offset. The total energy is accumulated in a simple summation. Complexity: O(natoms * ndescriptors) time and O(1) extra space (excluding output arrays). No special edge cases beyond handling `ndescriptors == 0` (the dot product loop is skipped) and ensuring the `energy_flag` controls whether the total is computed. The function should use raw pointers for outputs to match a low-level API style, but we can use `std::vector` internally for clarity. The solution uses `const` for all read-only inputs and `double*` for mutable outputs.

#include <vector>
#include <numeric> // for std::inner_product

// Compute per-atom betas and optional energies for a linear MLIAP model.
// coeffs: row-major, size num_elements * (1 + ndescriptors), bias then weights per element.
// descriptors: row-major, size natoms * ndescriptors.
// element_of_atom: size natoms, zero-based element indices.
// betas: output, row-major, size natoms * ndescriptors.
// eatoms: output, size natoms, only filled if compute_energy is true.
// Returns total energy if compute_energy, otherwise 0.0.
double compute_linear_model(
    int natoms,
    int ndescriptors,
    int num_elements,
    const double* coeffs,
    const double* descriptors,
    const int* element_of_atom,
    bool compute_energy,
    double* betas,
    double* eatoms)
{
    double total_energy = 0.0;

    for (int i = 0; i < natoms; ++i) {
        const int elem = element_of_atom[i];
        const double* elem_coeffs = coeffs + static_cast<long long>(elem) * (ndescriptors + 1);
        const double* desc_row = descriptors + static_cast<long long>(i) * ndescriptors;
        double* beta_row = betas + static_cast<long long>(i) * ndescriptors;

        // Copy weights (skip bias) to betas
        for (int j = 0; j < ndescriptors; ++j) {
            beta_row[j] = elem_coeffs[j + 1];
        }

        if (compute_energy) {
            double energy = elem_coeffs[0]; // bias
            for (int j = 0; j < ndescriptors; ++j) {
                energy += elem_coeffs[j + 1] * desc_row[j];
            }
            eatoms[i] = energy;
            total_energy += energy;
        }
    }
    return compute_energy ? total_energy : 0.0;
}

#include <cassert>
#include <vector>

// Solution function declaration (included directly above in real usage)
// double compute_linear_model(...);

int main() {
    // Simple case: 2 atoms, 2 elements, 3 descriptors
    const int natoms = 2;
    const int ndesc = 3;
    const int nelem = 2;
    // Element 0: bias=1.0, weights=[2,3,4]; Element 1: bias=5.0, weights=[6,7,8]
    std::vector<double> coeffs = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    // Atom 0: element 0, descriptors [0.1, 0.2, 0.3]; atom 1: element 1, descriptors [1.0, 0.0, -1.0]
    std::vector<double> desc = {0.1, 0.2, 0.3, 1.0, 0.0, -1.0};
    std::vector<int> elem = {0, 1};
    std::vector<double> betas(6, 0.0);
    std::vector<double> eatoms(2, 0.0);

    double total = compute_linear_model(natoms, ndesc, nelem, coeffs.data(), desc.data(), elem.data(), true, betas.data(), eatoms.data());
    // Check betas
    assert(betas[0] == 2.0 && betas[1] == 3.0 && betas[2] == 4.0);
    assert(betas[3] == 6.0 && betas[4] == 7.0 && betas[5] == 8.0);
    // Check energies: atom0: 1 + 2*0.1+3*0.2+4*0.3 = 1+0.2+0.6+1.2=3.0; atom1: 5+6*1+7*0+8*(-1)=5+6-8=3.0
    assert(eatoms[0] == 3.0);
    assert(eatoms[1] == 3.0);
    assert(total == 6.0);

    // Test with energy flag false: total should be 0, eatoms unchanged
    std::fill(eatoms.begin(), eatoms.end(), -1.0);
    double total2 = compute_linear_model(natoms, ndesc, nelem, coeffs.data(), desc.data(), elem.data(), false, betas.data(), eatoms.data());
    assert(total2 == 0.0);
    assert(eatoms[0] == -1.0 && eatoms[1] == -1.0); // not modified

    // Test zero descriptors
    const int ndesc0 = 0;
    std::vector<double> coeffs0 = {2.5, 7.5}; // bias for element0 and element1
    std::vector<double> desc0; // empty
    std::vector<int> elem0 = {0, 1, 0};
    std::vector<double> betas0(3*0, 0.0); // empty
    std::vector<double> eatoms0(3, 0.0);
    double total0 = compute_linear_model(3, ndesc0, 2, coeffs0.data(), desc0.data(), elem0.data(), true, betas0.data(), eatoms0.data());
    assert(eatoms0[0] == 2.5 && eatoms0[1] == 7.5 && eatoms0[2] == 2.5);
    assert(total0 == 2.5+7.5+2.5);

    // Test single atom, single descriptor, same element
    const int na = 1, nd = 1, ne = 1;
    std::vector<double> c = {0.0, 5.0}; // bias 0, weight 5
    std::vector<double> d = {2.0};
    std::vector<int> e = {0};
    std::vector<double> b(1), ea(1);
    double tot = compute_linear_model(na, nd, ne, c.data(), d.data(), e.data(), true, b.data(), ea.data());
    assert(b[0] == 5.0);
    assert(ea[0] == 0.0 + 5.0*2.0);
    assert(tot == 10.0);

    return 0;
}
