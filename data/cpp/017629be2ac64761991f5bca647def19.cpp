// Design a C++ class `TagAllocator` that manages a pool of integer tags in the range from a large initial `bottom` value down to `INT_MIN` (inclusive). The class must provide a mechanism to allocate tags (each allocation returns a token object representing ownership of that tag) and automatically recycle tags when the token goes out of scope. Specifically, implement a `getTag()` member function that returns a `Token` object. The `Token` class must support copy semantics (transferring ownership from the source to the destination, invalidating the source), and its destructor must return the tag to the allocator. The allocator must optimize tag reuse: it keeps a stack of freed tags for tags that are not currently the lowest allocated tag; if the freed tag is the lowest allocated tag (i.e., one greater than the current `bottom`), it should simply increment `bottom` instead of pushing onto the freed stack. The tag values must never be reused while a token still owns them, and the token must be able to read its tag value. Provide a free function `simulateTagAllocation` that takes a sequence of operations (e.g., a vector of pairs where each pair is an operation code: 0=allocate, 1=copy, 2=destroy) and returns a vector of integers representing the tag values observed after each allocate operation (in order), where destroyed tokens release their tags. The function must ensure correctness under the described behavior.
// The core challenge is implementing an ownership-transferring token that mimics the behavior of the given snippet. The allocator maintains two data members: an integer `bottom` (initially set to a large value like `INT_MAX` or a specified start) and a vector `freed` that stores tags that were previously used and released but are not currently the lowest allocated. When allocating, if `freed` is non-empty, we pop the last element (top of stack) and return a token owning that tag. Otherwise, we return a token owning the current `bottom` and decrement `bottom` (so the next allocation uses a smaller tag). The token stores a pointer to the allocator and an integer `tag_`; the copy constructor copies the allocator pointer and tag, then sets the source's `tag_` to -1 (invalid) to transfer ownership. The destructor checks if the token still owns a valid tag (not -1): if the tag equals `allocator->bottom + 1` (meaning it is the lowest currently allocated tag), then we increment `bottom` (effectively freeing it without pushing to the stack); otherwise, we push the tag onto `freed`. For the simulation function, we process operations sequentially. For allocate (op=0), we call `getTag()` and store the returned token in an external container (e.g., a vector of tokens), then record the token's tag value. For copy (op=1), we take the token at a given index and copy-construct a new token, storing it in the container (the original becomes invalid). For destroy (op=2), we erase the token at a given index (or set it to a default invalid token) so the destructor runs and releases the tag. Edge cases include copying an already-invalid token (should result in a token with tag -1, but that should not normally happen if operations are well-formed), and destroying a token that has already been copied (only one owner exists). The time complexity is O(1) per allocation (amortized) and O(1) per destruction, but copying is O(1) as well. Space complexity is O(number of freed tags) for the freed stack, plus O(number of live tokens) for the container.
#include <vector>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <limits>
#include <cassert>

class TagAllocator {
public:
    class Token {
    public:
        Token() : allocator(nullptr), tag_(-1) {}
        Token(TagAllocator* alloc, int t) : allocator(alloc), tag_(t) {}
        Token(const Token& other) : allocator(other.allocator), tag_(other.tag_) {
            other.tag_ = -1; // transfer ownership
        }
        Token& operator=(const Token& other) {
            if (this != &other) {
                if (tag_ != -1 && allocator) {
                    release();
                }
                allocator = other.allocator;
                tag_ = other.tag_;
                other.tag_ = -1;
            }
            return *this;
        }
        Token(Token&& other) noexcept : allocator(other.allocator), tag_(other.tag_) {
            other.tag_ = -1;
        }
        Token& operator=(Token&& other) noexcept {
            if (this != &other) {
                if (tag_ != -1 && allocator) {
                    release();
                }
                allocator = other.allocator;
                tag_ = other.tag_;
                other.tag_ = -1;
            }
            return *this;
        }
        ~Token() {
            if (tag_ != -1 && allocator) {
                release();
            }
        }
        int getTag() const { return tag_; }
        bool isValid() const { return tag_ != -1; }
    private:
        void release() {
            if (tag_ == allocator->bottom + 1) {
                ++allocator->bottom;
            } else {
                allocator->freed.push_back(tag_);
            }
            tag_ = -1;
        }
        TagAllocator* allocator;
        mutable int tag_;
    };

    TagAllocator(int start = std::numeric_limits<int>::max()) : bottom(start) {}
    Token getTag() {
        int tag;
        if (!freed.empty()) {
            tag = freed.back();
            freed.pop_back();
        } else {
            tag = bottom--;
        }
        return Token(this, tag);
    }
    int getBottom() const { return bottom; }
private:
    int bottom;
    std::vector<int> freed;
};

// Simulate a sequence of operations: each op is {code, index}
// code 0: allocate -> returns new token, index ignored
// code 1: copy from token at index -> creates new token
// code 2: destroy token at index
// Returns vector of tag values from allocate operations in order.
std::vector<int> simulateTagAllocation(const std::vector<std::pair<int, int>>& ops, int startTag = 1000) {
    TagAllocator alloc(startTag);
    std::vector<TagAllocator::Token> tokens;
    std::vector<int> results;
    for (const auto& op : ops) {
        int code = op.first;
        int idx = op.second;
        if (code == 0) {
            tokens.push_back(alloc.getTag());
            results.push_back(tokens.back().getTag());
        } else if (code == 1) {
            // Copy token at idx; original becomes invalid, new token appended.
            tokens.push_back(tokens[idx]);
        } else if (code == 2) {
            // Destroy token at idx; we replace with a default token to trigger destructor.
            tokens[idx] = TagAllocator::Token();
        }
    }
    return results;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    using Op = std::pair<int, int>;
    // Test 1: Simple allocations without any free: tags decrease from start
    {
        std::vector<Op> ops = {{0,0},{0,0},{0,0}};
        auto res = simulateTagAllocation(ops, 100);
        assert((res == std::vector<int>{100, 99, 98}));
    }
    // Test 2: Allocate, destroy lowest, then allocate again: reuse via bottom bump
    {
        std::vector<Op> ops = {{0,0},{2,0},{0,0}};
        auto res = simulateTagAllocation(ops, 10);
        assert((res == std::vector<int>{10, 10})); // freed tag 10 is bottom+1 after destroy
    }
    // Test 3: Allocate two, destroy first (non-bottom), then allocate: reuse from freed stack
    {
        std::vector<Op> ops = {{0,0},{0,0},{2,0},{0,0}};
        auto res = simulateTagAllocation(ops, 100);
        // First allocate 100, second allocate 99, destroy first (tag 100), allocate reuses 100
        assert((res == std::vector<int>{100, 99, 100}));
    }
    // Test 4: Copy semantics: allocate, copy, destroy original, then destroy copy
    {
        std::vector<Op> ops = {{0,0},{1,0},{2,0},{2,1},{0,0}};
        auto res = simulateTagAllocation(ops, 50);
        // allocate: 50, copy token 0 -> copy holds 50, original invalid, destroy original no-op, destroy copy releases 50, allocate gets 50
        assert((res == std::vector<int>{50, 50}));
    }
    // Test 5: Multiple frees and reuses in a stack-like manner
    {
        std::vector<Op> ops = {{0,0},{0,0},{0,0},{2,1},{2,2},{0,0},{0,0}};
        auto res = simulateTagAllocation(ops, 1000);
        // allocate: 1000, 999, 998; destroy token1 (999), destroy token2 (998) -> freed stack [999,998] (998 top)
        // allocate: pops 998 -> res add 998, allocate: pops 999 -> res add 999
        assert((res == std::vector<int>{1000, 999, 998, 998, 999}));
    }
    // Test 6: Edge: startTag = 0, allocation below zero? Should work with negatives
    {
        std::vector<Op> ops = {{0,0},{0,0},{0,0}};
        auto res = simulateTagAllocation(ops, -5);
        assert((res == std::vector<int>{-5, -6, -7}));
    }
    return 0;
}
