Given vectors of doubles `a` and `b` of equal nonzero length, and a desired CKKS encryption precision scale, write a C++ function that computes their approximate dot product using the Microsoft SEAL library. The function must encode both vectors, encrypt `a`, element-wise multiply the ciphertext by the plaintext encoding of `b` (with rescaling), then sum all slots via iterative rotations (doubling the rotation amount each step). The function should return the decrypted and decoded first slot as a double, throwing an exception if the input vectors are empty or have mismatched sizes. Use CKKS with polynomial modulus degree 8192, coefficients moduli {60, 40, 40, 60}, and a scale of `2^40`. The function must handle vectors of any length that fits in 8192/2 slots and must not hard-code a specific vector length.

The main algorithm follows the CKKS pipeline: set up encryption parameters, generate keys, encode both vectors with the given scale, encrypt the first vector, then perform a plaintext multiplication in-place followed by a rescale to the next level. To sum all elements, perform a tree-like rotation reduction: for `shift = 1, 2, 4, ...` up to the next power of two ≥ vector length, rotate the ciphertext by `shift` and add it to the accumulator. After decryption and decoding, the first slot contains the sum of all products (since CKKS encodes slot-wise operations). Important edge cases: empty vectors must be rejected; vectors longer than the slot count (poly_modulus_degree/2 = 4096) will cause an error—check this and throw. Also ensure the rescale is applied after each multiply_plain because the plaintext multiplication increases the scale. Time complexity is O(n) for encoding/decoding plus O(n) rotations (since each rotation is O(n) multiplication), so overall O(n log n) due to the number of rotations (log2(n)). Space is O(n) for the vectors and ciphertexts.

#include <seal/seal.h>
#include <vector>
#include <stdexcept>
#include <cmath>

// Computes approximate dot product of two equal-length vectors using CKKS.
// Returns the first decoded slot (sum of products).
double ckksDotProduct(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.empty() || b.empty()) {
        throw std::invalid_argument("Vectors must not be empty");
    }
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must have equal size");
    }

    const size_t poly_modulus_degree = 8192;
    const size_t slot_count = poly_modulus_degree / 2;
    if (a.size() > slot_count) {
        throw std::invalid_argument("Vector length exceeds CKKS slot capacity");
    }

    // CKKS parameters
    seal::EncryptionParameters parms(seal::scheme_type::ckks);
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(seal::CoeffModulus::Create(poly_modulus_degree, {60, 40, 40, 60}));

    seal::SEALContext context(parms);
    const double scale = std::pow(2.0, 40);

    // Key generation
    seal::KeyGenerator keygen(context);
    seal::PublicKey public_key;
    keygen.create_public_key(public_key);
    seal::SecretKey secret_key = keygen.secret_key();
    seal::GaloisKeys gal_keys;
    keygen.create_galois_keys(gal_keys);

    seal::Encryptor encryptor(context, public_key);
    seal::Evaluator evaluator(context);
    seal::Decryptor decryptor(context, secret_key);
    seal::CKKSEncoder encoder(context);

    // Encode both vectors
    seal::Plaintext pt_a, pt_b;
    encoder.encode(a, scale, pt_a);
    encoder.encode(b, scale, pt_b);

    // Encrypt vector a
    seal::Ciphertext ct_a;
    encryptor.encrypt(pt_a, ct_a);

    // Multiply ciphertext by plaintext b and rescale
    evaluator.multiply_plain_inplace(ct_a, pt_b);
    evaluator.rescale_to_next_inplace(ct_a);

    // Sum all slots via rotations (tree reduction)
    for (size_t shift = 1; shift < a.size(); shift <<= 1) {
        seal::Ciphertext rotated;
        evaluator.rotate_vector(ct_a, static_cast<int>(shift), gal_keys, rotated);
        evaluator.add_inplace(ct_a, rotated);
    }

    // Decrypt and decode
    seal::Plaintext plain_result;
    decryptor.decrypt(ct_a, plain_result);
    std::vector<double> result;
    encoder.decode(plain_result, result);

    return result[0];
}

#include <cassert>
#include <cmath>
#include <vector>
#include "ckks_dot_product.h" // Assuming function is declared here

int main() {
    // Basic test with known values
    std::vector<double> a1 = {1.5, 2.0, 3.5, 4.2};
    std::vector<double> b1 = {0.5, 1.5, 1.0, 0.8};
    double expected1 = 1.5*0.5 + 2.0*1.5 + 3.5*1.0 + 4.2*0.8;
    double result1 = ckksDotProduct(a1, b1);
    assert(std::abs(result1 - expected1) < 0.01);

    // Single-element vectors
    std::vector<double> a2 = {3.0};
    std::vector<double> b2 = {4.0};
    double result2 = ckksDotProduct(a2, b2);
    assert(std::abs(result2 - 12.0) < 0.01);

    // All zeros
    std::vector<double> a3 = {0.0, 0.0, 0.0};
    std::vector<double> b3 = {1.0, 2.0, 3.0};
    double result3 = ckksDotProduct(a3, b3);
    assert(std::abs(result3) < 0.01);

    // Longer vector (e.g., 8 elements)
    std::vector<double> a4 = {1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -8.0};
    std::vector<double> b4 = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    double expected4 = 0.0;
    for (size_t i = 0; i < a4.size(); ++i) expected4 += a4[i] * b4[i];
    double result4 = ckksDotProduct(a4, b4);
    assert(std::abs(result4 - expected4) < 0.1);

    // Empty vectors should throw
    bool threw = false;
    std::vector<double> empty;
    std::vector<double> nonEmpty = {1.0};
    try {
        ckksDotProduct(empty, nonEmpty);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Mismatched sizes should throw
    threw = false;
    std::vector<double> shortVec = {1.0, 2.0};
    std::vector<double> longVec = {1.0, 2.0, 3.0};
    try {
        ckksDotProduct(shortVec, longVec);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Very large size (but within slots) should work
    std::vector<double> aLarge(4096, 0.5);
    std::vector<double> bLarge(4096, 0.2);
    double expectedLarge = 4096 * 0.5 * 0.2;
    double resultLarge = ckksDotProduct(aLarge, bLarge);
    assert(std::abs(resultLarge - expectedLarge) < 1.0);

    return 0;
}
