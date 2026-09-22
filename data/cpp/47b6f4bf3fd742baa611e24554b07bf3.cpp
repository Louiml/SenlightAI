// Write a C++ function `processQueries` that takes an integer `n` (the number of elements in an initially zero-initialized array indexed from 1 to n) and a vector of queries, where each query is a tuple `(type, l, r, x)`. The types are: 1 = bitwise OR, 2 = bitwise AND, 3 = bitwise XOR (all applied to every element in the inclusive range `[l, r]` with the value `x`), 4 = output the sum of elements in `[l, r]`, and 5 = output the bitwise XOR of elements in `[l, r]`. The function should return a vector of results from type 4 and type 5 queries in the order they appear. All operations use 32-bit unsigned integers, and the array is initially all zeros. The input is guaranteed valid: `1 ≤ l ≤ r ≤ n`, and `x` is a non-negative integer that fits in 32 bits. You must implement this efficiently without brute-forcing each element for every query.

// The naïve approach of applying each update by looping from `l` to `r` would be too slow for large constraints (e.g., up to 10^5 elements and 10^5 queries). A better approach is to use a segment tree with lazy propagation, where each node stores the bitwise OR, AND, and XOR of its segment, along with a lazy tag representing pending bitwise operations. However, because the operations are bitwise and applied uniformly to all elements in a range, we can store per-node the aggregate of each bit independently. Each node maintains for each of the 32 bits: the count of zeros and ones in its segment (or equivalently, the bitwise OR and AND as aggregate). When applying an operation with `x` to a node, we can update the aggregates using bitwise logic: for OR, bits that are 1 in `x` become 1 in all elements; for AND, bits that are 0 in `x` become 0; for XOR, bits that are 1 in `x` flip. Lazy propagation ensures we only update when needed. For type 4 (sum) we compute the sum from the OR/AND aggregates (since sum = sum over bits of bit_count * 2^bit), and for type 5 (xor) we use the aggregate XOR. Edge cases include n=1, overlapping ranges, and repeated updates. Time complexity is O((n + q) * 32 * log n) worst-case, but with lazy updates it becomes O((n + q) * 32) for building and each query in average, actually O((n+q) log n * 32) worst-case; space is O(n * 32) for storing per-bit counts per node, but we can compress to OR, AND, and XOR per node (O(1) per node) using bit manipulation. The solution uses a segment tree with lazy tags storing pending OR, AND, and XOR operations, applied in a consistent order.

#include <vector>
#include <cstdint>
#include <tuple>
#include <functional>

// Segment tree supporting range bitwise operations (OR, AND, XOR) and queries (sum, XOR).
class BitwiseSegmentTree {
private:
    int n;
    std::vector<uint32_t> or_val, and_val, xor_val;
    std::vector<uint32_t> lazy_or, lazy_and, lazy_xor;
    std::vector<bool> has_lazy;

    void apply_or(int node, uint32_t x) {
        or_val[node] |= x;
        and_val[node] |= x;
        xor_val[node] |= x; // Actually OR affects xor only if the bits are 0; recompute later.
        // Correctly: for OR, new xor = (xor_val ^ (xor_val & x)) | (x & (~xor_val))? But simpler:
        // We'll handle via recompute from OR and AND in pull.
        // So here we just update lazily.
        lazy_or[node] |= x;
        has_lazy[node] = true;
    }

    void apply_and(int node, uint32_t x) {
        or_val[node] &= x;
        and_val[node] &= x;
        lazy_and[node] &= x;
        has_lazy[node] = true;
    }

    void apply_xor(int node, uint32_t x) {
        // XOR flips bits: for each bit, if x has 1, then 0<->1.
        // We can recompute OR and AND from the segment's element values, but we don't store all.
        // Instead, we maintain xor_val directly, and we need to update OR and AND.
        // Because we don't store individual elements, we keep additional info: for each node, we know
        // the count of each bit being 1 (or equivalently, we can derive sum). But we can store:
        // zeros and ones per bit? That's heavy. Alternative: store sum and xor only; but we need sum for query 4.
        // Actually sum = sum of elements, and xor = xor. When we XOR x to all elements, we can update sum:
        // For each bit, if x has 1, then number of ones becomes (length - ones). So sum changes accordingly.
        // Therefore we need length of segment. Let's store length in a separate array.
        // For simplicity, we'll use 32-bit per-bit counts. But to keep code concise, we can store
        // bit_count[32] per node, but that's 32 integers per node, which is acceptable for 4*n nodes.
        // However, the problem expects a clean solution; I'll implement with per-bit counts.
        // To avoid complexity, I'll write a simpler version that uses per-bit counts.
        // But the instruction says "high-quality" – so let's do it correctly.
        // I'll implement a version using 32-bit arrays for bit counts.
    }
public:
    // For brevity, I'll provide a complete implementation using a different data structure:
    // Instead of segment tree, we can use a sqrt decomposition or a bit-level lazy segment tree.
    // Let's use a segment tree where each node stores for each bit: count of ones. That's 32 integers per node.
    // For n up to 1e5, that's ~4*1e5*32 = 12.8 million integers, which is ~51 MB, acceptable.
    // But to keep code clean, I'll provide a solution using difference arrays with lazy for each bit?
    // Actually the simplest correct approach: maintain 32 fenwick trees or difference arrays for each bit.
    // For range OR: for each bit, if x has 1, set all bits in range to 1. For AND: if x has 0, set all to 0.
    // XOR: if x has 1, flip all bits in range.
    // So we need to maintain for each bit an array of bits (0/1) and support range set to 1, set to 0, and flip.
    // That's a classic lazy segment tree for each bit, but we can do it per bit with binary indexed with lazy tags.
    // Actually we can use a single segment tree with bitwise OR, AND, XOR aggregates and lazy tags.
    // Let me write a correct implementation using per-bit counts.
};

// The function that processes queries.
std::vector<uint64_t> processQueries(int n, const std::vector<std::tuple<int,int,int,uint32_t>>& queries) {
    // We'll implement a segment tree with per-bit counts.
    // For clarity, define a node storing:
    // uint32_t ones[32]; // count of 1s for each bit in segment
    // uint32_t lazy_or, lazy_and, lazy_xor; // pending operations
    // But that's heavy. Since we need to output sum and xor, we can store:
    // uint32_t OR, AND, XOR, and int length; plus lazy tags.
    // When applying OR x: new OR = OR|x, new AND = AND|x, new XOR = (XOR & ~x) | (x & (~AND_original))? 
    // This is messy. Let's use a simpler but correct approach: For each bit independently, we maintain a segment tree
    // that supports range set to 1, set to 0, and flip, and range sum query. That's 32 small segment trees.
    // But for the solution we can just use brute-force with blocking (sqrt decomposition) which is simpler and passes.
    // Since the task says "efficiently" but doesn't specify constraints, I'll implement a sqrt decomposition.
    // Let's write a clean sqrt decomposition.

    int block_size = 500;
    int num_blocks = (n + block_size - 1) / block_size;
    std::vector<uint32_t> arr(n, 0);
    std::vector<uint32_t> block_sum(num_blocks, 0), block_xor(num_blocks, 0);
    std::vector<uint32_t> lazy_or(num_blocks, 0), lazy_and(num_blocks, 0xFFFFFFFF), lazy_xor(num_blocks, 0);

    auto apply_block = [&](int b, int op, uint32_t x) {
        if (op == 1) lazy_or[b] |= x;
        else if (op == 2) lazy_and[b] &= x;
        else lazy_xor[b] ^= x;
        // Recompute block sum and xor lazily? We'll compute on push.
    };

    auto push = [&](int b) {
        int l = b * block_size;
        int r = std::min(n, (b+1)*block_size);
        for (int i = l; i < r; ++i) {
            if (lazy_or[b]) arr[i] |= lazy_or[b];
            if (lazy_and[b] != 0xFFFFFFFF) arr[i] &= lazy_and[b];
            if (lazy_xor[b]) arr[i] ^= lazy_xor[b];
        }
        lazy_or[b] = 0;
        lazy_and[b] = 0xFFFFFFFF;
        lazy_xor[b] = 0;
        // Recompute block aggregates
        block_sum[b] = 0;
        block_xor[b] = 0;
        for (int i = l; i < r; ++i) {
            block_sum[b] += arr[i];
            block_xor[b] ^= arr[i];
        }
    };

    auto rebuild_block = [&](int b) {
        int l = b * block_size;
        int r = std::min(n, (b+1)*block_size);
        block_sum[b] = 0;
        block_xor[b] = 0;
        for (int i = l; i < r; ++i) {
            block_sum[b] += arr[i];
            block_xor[b] ^= arr[i];
        }
    };

    std::vector<uint64_t> results;

    for (const auto& q : queries) {
        int type, l, r;
        uint32_t x;
        std::tie(type, l, r, x) = q;
        l--; r--; // 0-indexed inclusive
        int bl = l / block_size, br = r / block_size;

        if (type <= 3) {
            if (bl == br) {
                push(bl);
                for (int i = l; i <= r; ++i) {
                    if (type == 1) arr[i] |= x;
                    else if (type == 2) arr[i] &= x;
                    else arr[i] ^= x;
                }
                rebuild_block(bl);
            } else {
                // left partial
                push(bl);
                for (int i = l; i < std::min(n, (bl+1)*block_size); ++i) {
                    if (type == 1) arr[i] |= x;
                    else if (type == 2) arr[i] &= x;
                    else arr[i] ^= x;
                }
                rebuild_block(bl);
                // middle full blocks
                for (int b = bl+1; b < br; ++b) {
                    apply_block(b, type, x);
                    // We need to update block_sum and block_xor without pushing.
                    // For OR: sum increases by (len - ones_of_bits_in_x) * value? That's complex.
                    // So we must push and rebuild? Instead we can maintain per-block array? 
                    // Simpler: use a segment tree per bit, but that's long.
                    // Given this is a teaching task, I'll implement a correct segment tree with per-bit counts.
                }
                push(br);
                for (int i = br * block_size; i <= r; ++i) {
                    if (type == 1) arr[i] |= x;
                    else if (type == 2) arr[i] &= x;
                    else arr[i] ^= x;
                }
                rebuild_block(br);
            }
        } else {
            uint64_t sum = 0;
            uint32_t xr = 0;
            if (bl == br) {
                push(bl);
                for (int i = l; i <= r; ++i) {
                    sum += arr[i];
                    xr ^= arr[i];
                }
            } else {
                push(bl);
                for (int i = l; i < std::min(n, (bl+1)*block_size); ++i) {
                    sum += arr[i];
                    xr ^= arr[i];
                }
                for (int b = bl+1; b < br; ++b) {
                    // Need to apply lazy to get actual block sum/xor
                    // This is problematic without actual array. So we need to push b first.
                    // That defeats the purpose. This approach is not viable.
                }
            }
            if (type == 4) results.push_back(sum);
            else results.push_back(xr);
        }
    }
    return results;
}

I realize the above is incomplete. Therefore, I'll provide a clean and correct implementation using a segment tree with per-bit counts, which is the standard approach.

#include <vector>
#include <cstdint>
#include <tuple>

// Segment tree with per-bit counts for each node, supporting range OR/AND/XOR updates and sum/xor queries.
class BitSegmentTree {
private:
    struct Node {
        int ones[32];      // count of 1s for each bit in this segment
        int lazy_or;       // pending OR
        int lazy_and;      // pending AND
        int lazy_xor;      // pending XOR
        int len;           // length of segment
        Node() : lazy_or(0), lazy_and(0xFFFFFFFF), lazy_xor(0), len(0) {
            for (int i = 0; i < 32; ++i) ones[i] = 0;
        }
    };
    int n;
    std::vector<Node> tree;

    void apply_operation(int node, int op, uint32_t x) {
        if (op == 1) { // OR
            tree[node].lazy_or |= x;
            for (int b = 0; b < 32; ++b) {
                if (x & (1u << b)) {
                    tree[node].ones[b] = tree[node].len;
                }
            }
        } else if (op == 2) { // AND
            tree[node].lazy_and &= x;
            for (int b = 0; b < 32; ++b) {
                if (!(x & (1u << b))) {
                    tree[node].ones[b] = 0;
                }
            }
        } else { // XOR
            tree[node].lazy_xor ^= x;
            for (int b = 0; b < 32; ++b) {
                if (x & (1u << b)) {
                    tree[node].ones[b] = tree[node].len - tree[node].ones[b];
                }
            }
        }
    }

    void push(int node) {
        if (tree[node].lazy_or != 0) {
            apply_operation(node*2, 1, tree[node].lazy_or);
            apply_operation(node*2+1, 1, tree[node].lazy_or);
            tree[node].lazy_or = 0;
        }
        if (tree[node].lazy_and != 0xFFFFFFFF) {
            apply_operation(node*2, 2, tree[node].lazy_and);
            apply_operation(node*2+1, 2, tree[node].lazy_and);
            tree[node].lazy_and = 0xFFFFFFFF;
        }
        if (tree[node].lazy_xor != 0) {
            apply_operation(node*2, 3, tree[node].lazy_xor);
            apply_operation(node*2+1, 3, tree[node].lazy_xor);
            tree[node].lazy_xor = 0;
        }
    }

    void pull(int node) {
        for (int b = 0; b < 32; ++b) {
            tree[node].ones[b] = tree[node*2].ones[b] + tree[node*2+1].ones[b];
        }
        tree[node].len = tree[node*2].len + tree[node*2+1].len;
    }

    void build(int node, int l, int r) {
        tree[node].len = r - l + 1;
        if (l == r) {
            for (int b = 0; b < 32; ++b) tree[node].ones[b] = 0;
            return;
        }
        int mid = (l + r) / 2;
        build(node*2, l, mid);
        build(node*2+1, mid+1, r);
        pull(node);
    }

    void update(int node, int l, int r, int ql, int qr, int op, uint32_t x) {
        if (ql <= l && r <= qr) {
            apply_operation(node, op, x);
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        if (ql <= mid) update(node*2, l, mid, ql, qr, op, x);
        if (qr > mid) update(node*2+1, mid+1, r, ql, qr, op, x);
        pull(node);
    }

    uint64_t query_sum(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            uint64_t sum = 0;
            for (int b = 0; b < 32; ++b) {
                sum += (uint64_t)tree[node].ones[b] * (1ull << b);
            }
            return sum;
        }
        push(node);
        int mid = (l + r) / 2;
        uint64_t res = 0;
        if (ql <= mid) res += query_sum(node*2, l, mid, ql, qr);
        if (qr > mid) res += query_sum(node*2+1, mid+1, r, ql, qr);
        return res;
    }

    uint32_t query_xor(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            uint32_t xr = 0;
            for (int b = 0; b < 32; ++b) {
                if (tree[node].ones[b] % 2) xr |= (1u << b);
            }
            return xr;
        }
        push(node);
        int mid = (l + r) / 2;
        uint32_t res = 0;
        if (ql <= mid) res ^= query_xor(node*2, l, mid, ql, qr);
        if (qr > mid) res ^= query_xor(node*2+1, mid+1, r, ql, qr);
        return res;
    }

public:
    BitSegmentTree(int n) : n(n) {
        tree.resize(4 * n + 5);
        build(1, 0, n-1);
    }

    void range_update(int l, int r, int op, uint32_t x) {
        update(1, 0, n-1, l, r, op, x);
    }

    uint64_t range_sum(int l, int r) {
        return query_sum(1, 0, n-1, l, r);
    }

    uint32_t range_xor(int l, int r) {
        return query_xor(1, 0, n-1, l, r);
    }
};

// Function that processes queries and returns results for type 4 and 5.
std::vector<uint64_t> processQueries(int n, const std::vector<std::tuple<int,int,int,uint32_t>>& queries) {
    BitSegmentTree st(n);
    std::vector<uint64_t> results;
    for (const auto& q : queries) {
        int type, l, r;
        uint32_t x;
        std::tie(type, l, r, x) = q;
        --l; --r;  // convert to 0-indexed
        if (type >= 1 && type <= 3) {
            st.range_update(l, r, type, x);
        } else if (type == 4) {
            results.push_back(st.range_sum(l, r));
        } else if (type == 5) {
            results.push_back(st.range_xor(l, r));
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <tuple>
#include <cstdint>

// Assume processQueries is defined above (in the same file).

int main() {
    // Basic test: n=5, initial zeros.
    {
        std::vector<std::tuple<int,int,int,uint32_t>> queries = {
            {1, 1, 5, 1},   // OR 1 to [1,5] => all become 1
            {4, 1, 5, 0},   // sum = 5
            {5, 1, 5, 0},   // xor = 1
            {2, 1, 5, 0},   // AND 0 => all become 0
            {4, 1, 5, 0},   // sum = 0
            {3, 1, 5, 3},   // XOR 3 => all become 3
            {4, 1, 5, 0},   // sum = 15
            {5, 1, 5, 0}    // xor = 3
        };
        auto res = processQueries(5, queries);
        std::vector<uint64_t> expected = {5, 1, 0, 15, 3};
        assert(res == expected);
    }

    // Test overlapping ranges and partial updates.
    {
        std::vector<std::tuple<int,int,int,uint32_t>> queries = {
            {1, 1, 3, 4},      // OR 4 (0100) to [1,3] => arr[1]=4, arr[2]=4, arr[3]=4, arr[4]=0, arr[5]=0
            {4, 2, 4, 0},     // sum of [2,4] = 4+4+0=8
            {3, 2, 4, 2},     // XOR 2 (0010) to [2,4] => arr[2]=6, arr[3]=6, arr[4]=2
            {5, 1, 5, 0},     // xor of all = 4^6^6^2^0 = 6
            {2, 1, 5, 7},     // AND 7 (0111) => arr[1]=4, arr[2]=6, arr[3]=6, arr[4]=2, arr[5]=0
            {4, 1, 5, 0}      // sum = 18
        };
        auto res = processQueries(5, queries);
        std::vector<uint64_t> expected = {8, 6, 18};
        assert(res == expected);
    }

    // Test single element and XOR property.
    {
        std::vector<std::tuple<int,int,int,uint32_t>> queries = {
            {1, 1, 1, 255},
            {4, 1, 1, 0},
            {5, 1, 1, 0},
            {3, 1, 1, 255},
            {4, 1, 1, 0},
            {5, 1, 1, 0}
        };
        auto res = processQueries(1, queries);
        std::vector<uint64_t> expected = {255, 255, 0, 0};
        assert(res == expected);
    }

    // Test with larger n and many updates to ensure no overflow.
    {
        int n = 100;
        std::vector<std::tuple<int,int,int,uint32_t>> queries;
        // OR 1 to all
        queries.push_back({1, 1, n, 1});
        // XOR 2 to subrange
        queries.push_back({3, 1, 50, 2});
        // Sum query
        queries.push_back({4, 1, n, 0});
        auto res = processQueries(n, queries);
        // After OR 1: all are 1. After XOR 2 to [1,50]: those become 3, others remain 1.
        // Sum = 50*3 + 50*1 = 200.
        assert(res.size() == 1);
        assert(res[0] == 200);
    }

    // Test AND with all-zero mask and XOR of pairs.
    {
        std::vector<std::tuple<int,int,int,uint32_t>> queries = {
            {1, 1, 4, 15},    // all 15
            {2, 2, 3, 0},     // set [2,3] to 0
            {4, 1, 4, 0},     // sum = 30
            {5, 1, 4, 0}      // xor = 15^0^0^15 = 0
        };
        auto res = processQueries(4, queries);
        std::vector<uint64_t> expected = {30, 0};
        assert(res == expected);
    }

    return 0;
}
