/*
Write a C++ function `pruneStaleEntries` that takes a reference to a codebook structure (defined below) and removes entries whose `stale` counter exceeds half of the book's current `t` value, mirroring the provided `cvClearStaleEntries` logic. The codebook holds a dynamically allocated array of pointers to `code_element` objects, each storing a `stale` and `t_last_update` integer. After pruning, the function must compact the remaining entries into a new array, reset `t` to 0 and reset each kept entry's `t_last_update` to 0, and return the number of entries removed. The structure definitions and supporting types are provided; implement the function exactly. Assume inputs are valid (non-null array, non-negative counts, and `t` may be 0, in which case the stale threshold is 0 and all entries with `stale > 0` are removed). Ensure proper memory management (delete the old array, not the pointed-to elements) and handle the case where all entries are pruned (resulting array has size 0 and must be set to `nullptr`).
*/

#include <cstddef>   // for size_t (optional, not needed here)
#include <cstdlib>   // for nullptr

// Structures as described
struct code_element {
    int stale;
    int t_last_update;
};

struct codeBook {
    code_element** cb;   // array of pointers to code_elements
    int numEntries;
    int t;
};

// Removes entries with stale > half of t, compacts, resets timers, returns number removed
int pruneStaleEntries(codeBook& c) {
    if (c.numEntries == 0) {
        return 0;
    }

    // stale threshold
    int staleThresh = c.t / 2;

    // First pass: count how many to keep
    int keepCnt = 0;
    for (int i = 0; i < c.numEntries; ++i) {
        if (c.cb[i]->stale <= staleThresh) {
            ++keepCnt;
        }
    }

    // Allocate new array, possibly of size 0
    code_element** newCb = nullptr;
    if (keepCnt > 0) {
        newCb = new code_element*[keepCnt];
        int k = 0;
        for (int i = 0; i < c.numEntries; ++i) {
            if (c.cb[i]->stale <= staleThresh) {
                newCb[k] = c.cb[i];
                newCb[k]->t_last_update = 0;   // refresh
                ++k;
            }
        }
    }

    // Clean up old array but not the elements themselves
    delete[] c.cb;

    // Update book
    c.cb = newCb;
    int numCleared = c.numEntries - keepCnt;
    c.numEntries = keepCnt;
    c.t = 0;

    return numCleared;
}

#include <cassert>

int main() {
    // Helper to create a codebook with given stale values
    auto makeBook = [](std::initializer_list<int> stales, int t) {
        codeBook book;
        book.numEntries = static_cast<int>(stales.size());
        book.t = t;
        book.cb = (book.numEntries > 0) ? new code_element*[book.numEntries] : nullptr;
        int idx = 0;
        for (int s : stales) {
            book.cb[idx] = new code_element;
            book.cb[idx]->stale = s;
            book.cb[idx]->t_last_update = 10; // arbitrary initial value
            ++idx;
        }
        return book;
    };

    // All entries kept: t=10, staleThresh=5, all stale <=5
    {
        codeBook b = makeBook({1,2,3,4,5}, 10);
        int cleared = pruneStaleEntries(b);
        assert(cleared == 0);
        assert(b.numEntries == 5);
        assert(b.t == 0);
        for (int i = 0; i < b.numEntries; ++i) {
            assert(b.cb[i]->t_last_update == 0);
        }
        // Cleanup
        for (int i = 0; i < b.numEntries; ++i) delete b.cb[i];
        delete[] b.cb;
    }

    // Some pruned: t=10, staleThresh=5, stale>5 gone
    {
        codeBook b = makeBook({1,6,2,7,3}, 10);
        int cleared = pruneStaleEntries(b);
        assert(cleared == 2); // 6 and 7 removed
        assert(b.numEntries == 3);
        assert(b.t == 0);
        int sum = 0;
        for (int i = 0; i < b.numEntries; ++i) {
            assert(b.cb[i]->t_last_update == 0);
            sum += b.cb[i]->stale;
        }
        assert(sum == 1+2+3);
        // Cleanup
        for (int i = 0; i < b.numEntries; ++i) delete b.cb[i];
        delete[] b.cb;
    }

    // t=0, staleThresh=0, any stale>0 pruned
    {
        codeBook b = makeBook({0,1,0,2}, 0);
        int cleared = pruneStaleEntries(b);
        assert(cleared == 2); // 1 and 2 removed
        assert(b.numEntries == 2);
        assert(b.t == 0);
        for (int i = 0; i < b.numEntries; ++i) {
            assert(b.cb[i]->stale == 0);
            assert(b.cb[i]->t_last_update == 0);
        }
        for (int i = 0; i < b.numEntries; ++i) delete b.cb[i];
        delete[] b.cb;
    }

    // All pruned: t=10, staleThresh=5, all stale>5
    {
        codeBook b = makeBook({6,7,8}, 10);
        int cleared = pruneStaleEntries(b);
        assert(cleared == 3);
        assert(b.numEntries == 0);
        assert(b.cb == nullptr);
        assert(b.t == 0);
        // No elements to delete because they were not owned by book (per spec, we don't delete the elements themselves, but here they leak in test; that's fine)
    }

    // Empty book
    {
        codeBook b;
        b.numEntries = 0;
        b.cb = nullptr;
        b.t = 5;
        assert(pruneStaleEntries(b) == 0);
        assert(b.numEntries == 0);
        assert(b.cb == nullptr);
        assert(b.t == 0);
    }

    return 0;
}

// The function must iterate through the existing `cb` array, counting how many entries have `stale <= staleThresh` (where `staleThresh = c.t / 2`). Those entries are "good" and will be kept. We allocate a new array of pointers of exactly that count, copy the good pointers over, and set each kept entry's `t_last_update` to 0. We then reset `c.t` to 0, delete the old `cb` array (not the pointed-to elements, since they are owned elsewhere), assign the new array, update `numEntries`, and return the difference. Edge cases: (1) if `staleThresh` is negative (which happens if `c.t` is 0 and stale values are positive), any entry with `stale > 0` is pruned—this naturally works because `stale > staleThresh` with `staleThresh=0` means `stale>0` pruned; if all stale are 0, nothing is pruned. (2) If all entries are pruned, allocate an array of size 0 (or better, set `c.cb` to `nullptr` and `numEntries` to 0). Time complexity is O(n) where n is `numEntries`, space complexity O(n) for the new array, plus O(n) for the temporary `keep` array (though we can avoid `keep` by counting good entries first, then allocating and copying). We also handle the edge case that `numEntries` could be 0, in which case we immediately return 0 without touching the null pointer.
