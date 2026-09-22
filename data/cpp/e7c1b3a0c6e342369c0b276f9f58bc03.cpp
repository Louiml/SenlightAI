// Write a C++ function `int32_t countMatchingValues(const std::u16string& key, const std::vector<std::u16string>& candidates)` that simulates a simplified compressed trie lookup. The trie stores mappings from UTF-16 strings to 32-bit signed integer values (non-negative). Given a key string and a list of candidate strings, return the number of candidates that are **not** prefixes of the key and whose value, when looked up in the trie, is exactly `7`. The trie is represented by an array of 16-bit units (a flat byte-code format) that follows a specific encoding:  
// - A node with value `< 0x30` (i.e., `< kMinLinearMatch`) is a branch node. The first unit (if `0x00`) is a length byte; otherwise that unit itself gives `length-1` branch options. Branch encoding uses binary search: for groups larger than 5, compare the current unit with the stored unit; if less, go left (`length >>= 1` and jump by delta), else go right (subtract half length and skip delta). For 5 or fewer options, linear scan: each option is a unit, followed by a value; if the value's high bit (`0x8000`) is set, it is final (value = `node & 0x7FFF`); otherwise it is an intermediate delta that must be skipped after decoding.  
// - A node with value in `[0x30, 0x3000)` is a linear-match node: the actual match length is `node - 0x30 + 1`. If the next units match exactly, the trie advances; if the match ends, it checks whether the next node is a value node (≥ `0x3000`).  
// - A value node: if the high bit (`0x8000`) is set, it is final; the value is `node & 0x7FFF` (but note that values are non-negative and fit in 32 bits, so treat it as `int32_t`). If not final, the value is an intermediate jump delta (skip `skipNodeValue` logic: for node `< 0x3020` delta = node & 0x7FFF; for `< 0x3040` use two units; else three units).  
// - The trie uses an internal `pos_` pointer and a `remainingMatchLength_` (where -1 means not in a linear match). If the lookup fails (no match), the result is `USTRINGTRIE_NO_MATCH` (0); if it matches but has no value at that exact point, the result is `USTRINGTRIE_NO_VALUE` (1); if it has a final value, the result is `USTRINGTRIE_FINAL_VALUE` (2).  
// The trie data is given as a `std::u16string` (the flat array). The function must implement a **simplified** version of `UCharsTrie::next(int32_t uchar)` and `UCharsTrie::current()` only, not the multi-unit `next(ptr, sLength)` version. For each candidate, start a fresh traversal from the trie root. For each unit `c` in the candidate, call `next(c)`. After consuming all candidate units, call `current()` to see if it returns `USTRINGTRIE_FINAL_VALUE` and the value equals 7. If any `next(c)` returns `NO_MATCH` (0) or `FINAL_VALUE` (2) before consuming the full candidate, stop early and count that candidate as not matching (because it is either not found or is a prefix with a final value, but we need the exact key to have value 7). Only count candidates that fully match and whose final value is exactly 7. Return the count.

int main() {
    // Build a simple trie: mapping "ab" -> 7, "ac" -> 8, "ad" -> 7, "ae" -> 7
    // We'll construct a flat array manually.
    // Root is a linear-match node for 'a' (length=1): node = kMinLinearMatch + 0 = 0x30
    // Then branch node with 3 options: b, c, d (but we want 4? keep simple)
    // Actually, let's use a simple case: root linear-match "a", then branch with b->7, c->8, d->7
    // Encoding:
    // pos0: 0x30 (linear match length 1)
    // pos1: 'a'
    // pos2: branch node length (for 3 options, length=3-1=2? Actually branch node: if node < kMinLinearMatch, that value is length-1. So node=2 for 3 options? length=node+1=3. But if node==0, read length from next byte. We'll use node=2.
    // pos2: 0x02 (branch with 3 options)
    // Then options:
    // pos3: 'b', pos4: final value node 0x8007 (final value 7)
    // pos5: 'c', pos6: final value node 0x8008
    // pos7: 'd', pos8: final value node 0x8007
    std::u16string trie;
    trie.push_back(0x30); // linear match 'a'
    trie.push_back(u'a');
    trie.push_back(0x02); // branch length 3 (node=2 means length=3)
    trie.push_back(u'b');
    trie.push_back(0x8007);
    trie.push_back(u'c');
    trie.push_back(0x8008);
    trie.push_back(u'd');
    trie.push_back(0x8007);

    std::vector<std::u16string> candidates = {u"ab", u"ac", u"ad", u"a", u"b", u"abc"};
    // Expected: "ab" -> value 7, counts; "ac" -> 8, no; "ad" -> 7, counts; "a" -> not final value at that point (it's a branch, no value) -> current returns NO_VALUE, so not count; "b" -> no match; "abc" -> after "ab" final value, next 'c' would return NO_MATCH (since final value node stops), so not count.
    int32_t result = countMatchingValues(trie, candidates);
    assert(result == 2);

    // Another test: empty candidates
    assert(countMatchingValues(trie, {}) == 0);

    // Test with a key that is not in trie
    std::vector<std::u16string> cand2 = {u"zz", u"", u"ab"};
    // "zz" -> NO_MATCH, "" -> current() at root: root is linear-match node, so NO_VALUE (1), not count; "ab" -> 7 counts.
    assert(countMatchingValues(trie, cand2) == 1);
}

#include <cstdint>
#include <string>
#include <vector>

// Simplified UCharsTrie constants
constexpr int32_t kMinLinearMatch = 0x30;
constexpr int32_t kMinValueLead = 0x3000;
constexpr int32_t kValueIsFinal = 0x8000;
constexpr int32_t kMaxBranchLinearSubNodeLength = 5;
constexpr int32_t kMinTwoUnitValueLead = 0x3020;
constexpr int32_t kThreeUnitValueLead = 0x3040;

struct TrieState {
    const std::u16string& data;
    int32_t pos;                // current index into data; -1 means stopped
    int32_t remainingMatchLength; // -1 means not in linear match
};

inline int32_t readValue(const std::u16string& data, int32_t& pos, int32_t node) {
    // node already has kValueIsFinal bit stripped
    if (node < kMinTwoUnitValueLead) {
        return node;
    } else if (node < kThreeUnitValueLead) {
        int32_t delta = ((node - kMinTwoUnitValueLead) << 16) | data[pos++];
        return delta;
    } else {
        int32_t delta = (data[pos] << 16) | data[pos + 1];
        pos += 2;
        return delta;
    }
}

inline int32_t skipValue(const std::u16string& data, int32_t pos, int32_t node) {
    // node already has kValueIsFinal bit stripped
    if (node < kMinTwoUnitValueLead) {
        return pos;
    } else if (node < kThreeUnitValueLead) {
        return pos + 1;
    } else {
        return pos + 2;
    }
}

inline int32_t skipNodeValue(const std::u16string& data, int32_t pos, int32_t node) {
    // node includes the intermediate value node (without final bit)
    if (node < kMinTwoUnitValueLead) {
        return pos;
    } else if (node < kThreeUnitValueLead) {
        return pos + 1;
    } else {
        return pos + 2;
    }
}

inline int32_t jumpByDelta(const std::u16string& data, int32_t pos, int32_t delta) {
    return pos + delta;
}

inline int32_t skipDelta(const std::u16string& data, int32_t pos, int32_t delta) {
    return pos + delta;
}

int32_t valueResult(int32_t node) {
    // node is a value node (>= kMinValueLead)
    if (node & kValueIsFinal) {
        return 2; // USTRINGTRIE_FINAL_VALUE
    }
    return 1; // USTRINGTRIE_NO_VALUE
}

int32_t branchNext(TrieState& state, int32_t pos, int32_t length, int32_t uchar) {
    const std::u16string& data = state.data;
    if (length == 0) {
        length = data[pos++];
    }
    ++length;
    // Binary search for large branches
    while (length > kMaxBranchLinearSubNodeLength) {
        if (uchar < data[pos++]) {
            length >>= 1;
            int32_t delta = 0;
            // read delta (treated as a jump delta; simplified: use readValue with node=0)
            // In real code, delta is encoded after the comparison unit.
            // For this simplified simulation, we just assume delta is stored as a value.
            // Use a placeholder: read a value node starting at pos.
            int32_t node = data[pos];
            if (node & kValueIsFinal) {
                // shouldn't happen in branch jump
                return 0;
            }
            pos = skipNodeValue(data, pos, node & 0x7FFF);
            pos = jumpByDelta(data, pos, node & 0x7FFF);
        } else {
            length = length - (length >> 1);
            int32_t node = data[pos];
            // skip delta
            pos = skipNodeValue(data, pos, node & 0x7FFF);
            pos = skipDelta(data, pos, node & 0x7FFF);
        }
    }
    // Linear scan
    do {
        if (uchar == data[pos++]) {
            int32_t node = data[pos];
            int32_t result;
            if (node & kValueIsFinal) {
                result = 2; // FINAL_VALUE
                // leave node at pos+1 for getValue()
                state.pos = pos + 1;
                return result;
            } else {
                ++pos; // skip the intermediate value lead
                int32_t delta = readValue(data, pos, node);
                pos += delta;
                state.pos = pos;
                node = data[pos];
                result = (node >= kMinValueLead) ? valueResult(node) : 1; // NO_VALUE
                return result;
            }
        }
        --length;
        // skip value of this option
        int32_t vnode = data[pos];
        pos = skipValue(data, pos, vnode & 0x7FFF);
        pos = (vnode & kValueIsFinal) ? pos : skipDelta(data, pos, (vnode & 0x7FFF));
    } while (length > 1);
    if (uchar == data[pos++]) {
        state.pos = pos;
        int32_t node = data[pos];
        return (node >= kMinValueLead) ? valueResult(node) : 1;
    } else {
        state.pos = -1;
        return 0;
    }
}

int32_t nextImpl(TrieState& state, int32_t pos, int32_t uchar) {
    const std::u16string& data = state.data;
    int32_t node = data[pos++];
    for (;;) {
        if (node < kMinLinearMatch) {
            return branchNext(state, pos, node, uchar);
        } else if (node < kMinValueLead) {
            int32_t length = node - kMinLinearMatch;
            if (uchar == data[pos++]) {
                state.remainingMatchLength = --length;
                state.pos = pos;
                if (length < 0 && data[pos] >= kMinValueLead) {
                    return valueResult(data[pos]);
                }
                return 1; // NO_VALUE (still matching linear node)
            } else {
                break;
            }
        } else if (node & kValueIsFinal) {
            break;
        } else {
            pos = skipNodeValue(data, pos, node & 0x7FFF);
            node = node & 0x7FFF & 0x00FF; // just get node type (simplified as low byte)
            // In real code it's node &= kNodeTypeMask (which is 0x1F or similar)
            // For simplicity, we break to stop matching.
            break;
        }
    }
    state.pos = -1;
    return 0;
}

int32_t nextUnit(TrieState& state, int32_t uchar) {
    if (state.pos < 0) {
        return 0;
    }
    int32_t length = state.remainingMatchLength;
    if (length >= 0) {
        if (uchar == state.data[state.pos]) {
            state.pos++;
            state.remainingMatchLength = --length;
            if (length < 0 && state.data[state.pos] >= kMinValueLead) {
                return valueResult(state.data[state.pos]);
            }
            return 1; // NO_VALUE
        } else {
            state.pos = -1;
            return 0;
        }
    }
    return nextImpl(state, state.pos, uchar);
}

int32_t current(const TrieState& state) {
    if (state.pos < 0) {
        return 0;
    }
    if (state.remainingMatchLength >= 0) {
        return 1; // NO_VALUE (still in linear match)
    }
    int32_t node = state.data[state.pos];
    if (node >= kMinValueLead) {
        return valueResult(node);
    }
    return 1; // NO_VALUE
}

int32_t getValue(const TrieState& state) {
    // Call only after current() returns FINAL_VALUE (2)
    int32_t node = state.data[state.pos];
    return node & 0x7FFF;
}

// The function requested by the task
int32_t countMatchingValues(const std::u16string& key, const std::vector<std::u16string>& candidates) {
    // We ignore the key because the simulation only requires the trie data.
    // The trie is given in `key`? No, but for the task, we need the trie data.
    // To make this self-contained, we assume the trie data is the first element? No.
    // The task is ambiguous: the "trie" is represented by `key`? Let's interpret:
    // The task says "The trie is represented by an array of 16-bit units" and the function signature has `const std::u16string& key`.
    // So `key` is the flat trie data. We use it as `state.data`.
    TrieState state{key, 0, -1};
    int32_t count = 0;
    for (const auto& candidate : candidates) {
        TrieState s = state;
        bool ok = true;
        for (char16_t c : candidate) {
            int32_t result = nextUnit(s, c);
            if (result == 0 || result == 2) {
                ok = false;
                break;
            }
        }
        if (ok && current(s) == 2 && getValue(s) == 7) {
            ++count;
        }
    }
    return count;
}

// The solution must interpret the trie’s compact encoding. The core is to implement a state machine that mimics `UCharsTrie::next`. The state consists of a position pointer `pos` (index into the flat array) and a `remainingMatchLength` (starts at -1). The `next(char16_t)` function first checks if `pos` is null (represented by a sentinel like -1 or by using a boolean `stopped`). If `remainingMatchLength >= 0`, we are inside a linear-match node; if the input unit equals the current unit at `pos`, advance `pos` and decrement; if the remaining length becomes -1, we need to check if the next node at `pos` is a value node (≥ 0x3000) to return `NO_VALUE` or `FINAL_VALUE`. Otherwise, we call `nextImpl(pos, uchar)` which reads a node and loops: branch nodes (node < 0x30) call `branchNext`; linear-match nodes (0x30 ≤ node < 0x3000) match the first unit; value nodes (≥ 0x3000) indicate no further match. The `branchNext` logic must correctly handle binary search for large branches (length > 5) and linear scan for small ones. For linear scan, each option is a unit followed by a value; the value node uses the same value decoding as elsewhere. Critical edge cases: the branch length byte when node == 0; handling of intermediate values (delta skip) when the value node is not final; ensuring that after a branch match, if the result is `NO_VALUE` (non-final value), the `pos_` is correctly advanced to the node after the delta, and then we continue matching the rest of the candidate. The `current()` method checks if `pos` is valid and `remainingMatchLength < 0` and the node at `pos` is a value node; if final, return `FINAL_VALUE`; if non-final, return `NO_VALUE`. For a candidate to count, we must fully consume it without hitting `NO_MATCH` or an early `FINAL_VALUE`, then call `current()` and check that it returns `FINAL_VALUE` and the value is 7. Time complexity: for each candidate of length L, each `next` step is O(1) amortized (branch steps are constant because the trie depth is bounded), so total O(total length of all candidates). Space: O(1) extra per traversal beyond the trie.
