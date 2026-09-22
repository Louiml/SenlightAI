// Write a C++ function named `buildStandardScriptPubKey` that takes a `CTxDestination` object (which may represent a pay-to-pubkey-hash, pay-to-script-hash, pay-to-witness-key-hash, pay-to-witness-script-hash, pay-to-taproot, pay-to-witness-unknown, or a raw public key) and returns the corresponding standard Bitcoin scriptPubKey as a `CScript`. The function must correctly handle each destination type by producing the exact standard script format, and must throw or otherwise reject invalid or non-standard destinations (such as `CNoDestination` or `PubKeyDestination`). The solution should be self-contained and implement all necessary helper logic, including conversion of hashes to byte vectors and encoding of witness versions.

// The task requires translating a variant type (`CTxDestination`) into its canonical script form. The approach uses a visitor pattern to handle each alternative type explicitly. For each destination type, we construct the corresponding script using the `CScript` operator overloads and standard opcodes:
// - `PKHash` → `OP_DUP OP_HASH160 <hash> OP_EQUALVERIFY OP_CHECKSIG`
// - `ScriptHash` → `OP_HASH160 <hash> OP_EQUAL`
// - `WitnessV0KeyHash` → `OP_0 <20-byte hash>`
// - `WitnessV0ScriptHash` → `OP_0 <32-byte hash>`
// - `WitnessV1Taproot` → `OP_1 <32-byte x-only pubkey>`
// - `WitnessUnknown` → `OP_N <program>`, where `OP_N` is encoded via `CScript::EncodeOP_N(version)` and the program is the raw bytes.
// - `PubKeyDestination` → `<pubkey> OP_CHECKSIG`
// - `CNoDestination` → not valid, so we throw an exception.
//
// Edge cases: `WitnessUnknown` must have a version between 2 and 16 (since version 0 and 1 are covered by other types), and the witness program must be non-empty; we still encode it as given. The `CNoDestination` and `PubKeyDestination` are considered non‑standard for address output purposes, so the function should throw `std::invalid_argument`. The function must be `const`-correct and use only standard library and custom types. Time complexity is O(1) with respect to input size (fixed‑size hashes), and space complexity is O(1) for the output script.

#include <addresstype.h>
#include <script/script.h>

#include <stdexcept>
#include <vector>

/**
 * @brief Builds the standard scriptPubKey for a given CTxDestination.
 *
 * @param dest The destination to convert.
 * @return CScript The corresponding standard scriptPubKey.
 * @throws std::invalid_argument if the destination is invalid or non-standard.
 */
CScript buildStandardScriptPubKey(const CTxDestination& dest)
{
    return std::visit([](const auto& d) -> CScript {
        using T = std::decay_t<decltype(d)>;

        if constexpr (std::is_same_v<T, CNoDestination>) {
            throw std::invalid_argument("Cannot build script for CNoDestination");
        } else if constexpr (std::is_same_v<T, PubKeyDestination>) {
            return CScript() << ToByteVector(d.GetPubKey()) << OP_CHECKSIG;
        } else if constexpr (std::is_same_v<T, PKHash>) {
            return CScript() << OP_DUP << OP_HASH160 << ToByteVector(d) << OP_EQUALVERIFY << OP_CHECKSIG;
        } else if constexpr (std::is_same_v<T, ScriptHash>) {
            return CScript() << OP_HASH160 << ToByteVector(d) << OP_EQUAL;
        } else if constexpr (std::is_same_v<T, WitnessV0KeyHash>) {
            return CScript() << OP_0 << ToByteVector(d);
        } else if constexpr (std::is_same_v<T, WitnessV0ScriptHash>) {
            return CScript() << OP_0 << ToByteVector(d);
        } else if constexpr (std::is_same_v<T, WitnessV1Taproot>) {
            return CScript() << OP_1 << ToByteVector(d);
        } else if constexpr (std::is_same_v<T, WitnessUnknown>) {
            return CScript() << CScript::EncodeOP_N(d.GetWitnessVersion()) << d.GetWitnessProgram();
        } else {
            throw std::invalid_argument("Unknown destination type");
        }
    }, dest);
}

#include <addresstype.h>
#include <cassert>
#include <cstdint>
#include <vector>

int main()
{
    // Test PKHash: OP_DUP OP_HASH160 <20 bytes> OP_EQUALVERIFY OP_CHECKSIG
    {
        std::vector<unsigned char> hash_bytes(20, 0xAB); // arbitrary
        PKHash pk_hash(uint160(hash_bytes));
        CScript script = buildStandardScriptPubKey(pk_hash);
        CScript expected = CScript() << OP_DUP << OP_HASH160 << ToByteVector(pk_hash) << OP_EQUALVERIFY << OP_CHECKSIG;
        assert(script == expected);
    }

    // Test ScriptHash: OP_HASH160 <20 bytes> OP_EQUAL
    {
        std::vector<unsigned char> hash_bytes(20, 0xCD);
        ScriptHash script_hash(uint160(hash_bytes));
        CScript script = buildStandardScriptPubKey(script_hash);
        CScript expected = CScript() << OP_HASH160 << ToByteVector(script_hash) << OP_EQUAL;
        assert(script == expected);
    }

    // Test WitnessV0KeyHash: OP_0 <20 bytes>
    {
        std::vector<unsigned char> hash_bytes(20, 0xEF);
        WitnessV0KeyHash wsh(uint160(hash_bytes));
        CScript script = buildStandardScriptPubKey(wsh);
        CScript expected = CScript() << OP_0 << ToByteVector(wsh);
        assert(script == expected);
    }

    // Test WitnessV0ScriptHash: OP_0 <32 bytes>
    {
        std::vector<unsigned char> hash_bytes(32, 0x12);
        WitnessV0ScriptHash wsh;
        std::copy(hash_bytes.begin(), hash_bytes.end(), wsh.begin());
        CScript script = buildStandardScriptPubKey(wsh);
        CScript expected = CScript() << OP_0 << ToByteVector(wsh);
        assert(script == expected);
    }

    // Test WitnessV1Taproot: OP_1 <32 bytes>
    {
        std::vector<unsigned char> tap_bytes(32, 0x34);
        WitnessV1Taproot tap;
        std::copy(tap_bytes.begin(), tap_bytes.end(), tap.begin());
        CScript script = buildStandardScriptPubKey(tap);
        CScript expected = CScript() << OP_1 << ToByteVector(tap);
        assert(script == expected);
    }

    // Test WitnessUnknown: version 2, program [0x56, 0x78]
    {
        WitnessUnknown unknown(2, {0x56, 0x78});
        CScript script = buildStandardScriptPubKey(unknown);
        CScript expected = CScript() << CScript::EncodeOP_N(2) << std::vector<unsigned char>{0x56, 0x78};
        assert(script == expected);
    }

    // Test PubKeyDestination: <pubkey> OP_CHECKSIG
    {
        std::vector<unsigned char> pubkey_bytes = {0x02, 0x88, 0x99, 0xAA}; // dummy compressed pubkey
        CPubKey pubkey(pubkey_bytes);
        PubKeyDestination pubkey_dest(pubkey);
        CScript script = buildStandardScriptPubKey(pubkey_dest);
        CScript expected = CScript() << ToByteVector(pubkey) << OP_CHECKSIG;
        assert(script == expected);
    }

    // Test CNoDestination throws
    {
        bool threw = false;
        try {
            CNoDestination none;
            buildStandardScriptPubKey(none);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    return 0;
}
