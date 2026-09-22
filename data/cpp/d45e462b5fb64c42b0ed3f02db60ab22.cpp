/*
Write a C++ function `bool VerifyBlockSignature(const std::vector<unsigned char>& scriptPubKey, const std::vector<unsigned char>& blockHash, const std::vector<unsigned char>& vchBlockSig, bool isPubKeyHash)` that determines whether a given block signature is valid for a specified output script type. The function should support two script types: `TX_PUBKEY` (pay-to-pubkey) and `TX_PUBKEYHASH` (pay-to-pubkey-hash). For `TX_PUBKEY`, the scriptPubKey contains the full public key, and the signature must be verified directly against that key. For `TX_PUBKEYHASH`, the scriptPubKey contains a 20-byte hash of the public key; the function must parse the public key from the signature (assume the signature includes a recovery ID byte at the beginning), compute its hash, compare it to the given hash, and then verify the signature with that public key. Return `false` if the script type is unsupported, the signature is empty, the public key is invalid, or the hash does not match. Use a simplified SHA-256 and secp256k1 verification simulation for test purposes (you may assume the keys and signatures are precomputed and provided as byte arrays). The function should not depend on external cryptographic libraries; instead, use a simple placeholder that checks structural validity and predefined constants for demonstration.
*/

#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>

// Placeholder for real cryptographic verification.
// In a real system, this would use an ECDSA verify function.
static bool SimulatedVerify(const std::vector<unsigned char>& pubkey,
                            const std::vector<unsigned char>& hash,
                            const std::vector<unsigned char>& sig) {
    // Simulate that a valid signature is non-empty and the key is one of two known keys.
    const std::vector<unsigned char> knownKey1 = {0x02, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
                                                  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33,
                                                  0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD,
                                                  0xEE, 0xFF, 0x00};
    const std::vector<unsigned char> knownKey2 = {0x03, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12,
                                                  0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56,
                                                  0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A,
                                                  0xBC, 0xDE, 0xF0};
    if (pubkey == knownKey1 || pubkey == knownKey2) {
        return !sig.empty() && !hash.empty();
    }
    return false;
}

// Simplified hash function: returns the last 20 bytes of the key in reverse order.
static std::vector<unsigned char> SimplifiedHash(const std::vector<unsigned char>& key) {
    std::vector<unsigned char> hash;
    if (key.size() < 20) return hash;
    for (size_t i = 0; i < 20; ++i) {
        hash.push_back(key[key.size() - 1 - i]);
    }
    return hash;
}

// Verify a block signature against a scriptPubKey.
// isPubKeyHash: true if the script is TX_PUBKEYHASH, false if TX_PUBKEY.
bool VerifyBlockSignature(const std::vector<unsigned char>& scriptPubKey,
                          const std::vector<unsigned char>& blockHash,
                          const std::vector<unsigned char>& vchBlockSig,
                          bool isPubKeyHash) {
    if (vchBlockSig.empty()) return false;
    if (blockHash.empty()) return false;

    if (!isPubKeyHash) {
        // TX_PUBKEY: scriptPubKey is the full public key.
        if (scriptPubKey.size() != 33 && scriptPubKey.size() != 65) return false;
        return SimulatedVerify(scriptPubKey, blockHash, vchBlockSig);
    } else {
        // TX_PUBKEYHASH: scriptPubKey is a 20-byte hash.
        if (scriptPubKey.size() != 20) return false;
        // The signature's first byte is a recovery ID, the rest is the public key.
        if (vchBlockSig.size() < 2) return false;
        std::vector<unsigned char> pubkey(vchBlockSig.begin() + 1, vchBlockSig.end());
        if (pubkey.size() != 33 && pubkey.size() != 65) return false;
        std::vector<unsigned char> computedHash = SimplifiedHash(pubkey);
        if (computedHash.size() != 20) return false;
        if (!std::equal(computedHash.begin(), computedHash.end(), scriptPubKey.begin())) return false;
        return SimulatedVerify(pubkey, blockHash, vchBlockSig);
    }
}

#include <cassert>
#include <vector>

int main() {
    // Known valid pubkey hash (simplified hash of knownKey1)
    std::vector<unsigned char> key1 = {0x02, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
                                       0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33,
                                       0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD,
                                       0xEE, 0xFF, 0x00};
    std::vector<unsigned char> key2 = {0x03, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12,
                                       0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56,
                                       0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A,
                                       0xBC, 0xDE, 0xF0};
    // Compute simplified hashes
    std::vector<unsigned char> hash1;
    for (int i = 0; i < 20; ++i) hash1.push_back(key1[key1.size()-1-i]);
    std::vector<unsigned char> hash2;
    for (int i = 0; i < 20; ++i) hash2.push_back(key2[key2.size()-1-i]);

    std::vector<unsigned char> blockHash = {0x01, 0x02, 0x03, 0x04};
    std::vector<unsigned char> sigDummy = {0xAA, 0xBB, 0xCC};

    // Case 1: TX_PUBKEY with valid key1
    assert(VerifyBlockSignature(key1, blockHash, sigDummy, false) == true);
    // Case 2: TX_PUBKEY with invalid key (random length)
    std::vector<unsigned char> badKey = {0x01, 0x02, 0x03};
    assert(VerifyBlockSignature(badKey, blockHash, sigDummy, false) == false);
    // Case 3: TX_PUBKEYHASH with correct hash and matching key in sig
    std::vector<unsigned char> sigWithRecovery1 = {0x00};
    sigWithRecovery1.insert(sigWithRecovery1.end(), key1.begin(), key1.end());
    assert(VerifyBlockSignature(hash1, blockHash, sigWithRecovery1, true) == true);
    // Case 4: TX_PUBKEYHASH with wrong hash
    assert(VerifyBlockSignature(hash2, blockHash, sigWithRecovery1, true) == false);
    // Case 5: Empty signature
    std::vector<unsigned char> emptySig;
    assert(VerifyBlockSignature(key1, blockHash, emptySig, false) == false);
    assert(VerifyBlockSignature(hash1, blockHash, emptySig, true) == false);
    // Case 6: Empty block hash
    std::vector<unsigned char> emptyHash;
    assert(VerifyBlockSignature(key1, emptyHash, sigDummy, false) == false);
    // Case 7: Signature too short for pubkeyhash (only recovery id)
    std::vector<unsigned char> shortSig = {0x00};
    assert(VerifyBlockSignature(hash1, blockHash, shortSig, true) == false);
    // Case 8: Incorrect scriptPubKey length for pubkeyhash
    std::vector<unsigned char> wrongLenHash = {0x01, 0x02};
    assert(VerifyBlockSignature(wrongLenHash, blockHash, sigWithRecovery1, true) == false);

    return 0;
}

// The core approach is to first decode the script type from an explicit boolean flag `isPubKeyHash`. If `isPubKeyHash` is false, the script is `TX_PUBKEY`, so we extract the public key directly from `scriptPubKey` (which is the entire byte array representing a valid public key). Then we ensure the signature is non-empty and call a placeholder verification function that simulates a cryptographic check (e.g., compares a predefined valid key-hash pair). If `isPubKeyHash` is true, the `scriptPubKey` is treated as a 20-byte RIPEMD-160 hash of a public key. The signature's first byte is a recovery ID (we ignore it for simplicity), and the remaining bytes are the public key. We compute a simplified hash (e.g., last 20 bytes of the byte-reversed key) and compare to `scriptPubKey`. If they match, we proceed to verify the signature. Edge cases include empty signature, invalid public key length (must be 33 or 65 for compressed/uncompressed), and hash mismatch. Time complexity is O(1) since we process fixed-size arrays, and space complexity is O(1) aside from temporary copies. The placeholder verification returns `true` only if the provided key matches a hardcoded known-good key and the signature bytes are non-empty.
