Write a standalone C++ function that, given a `std::vector<int>` representing the values at each position of a hypothetical index key pattern (where the first element corresponds to the leading field, the second to the next field, etc.), and a separate `std::vector<std::string>` of field paths (each path may be compound with dots, e.g., `"a.b"`), returns a `std::vector<bool>` of the same length as the field-path vector. The `i`-th boolean should be `true` if and only if the field path at that index can be assigned to the given index position based on multikey constraints, following the rules from the plan enumerator: (1) For a non‑multikey index (indicated by a separate `bool multikey` parameter), every path can be assigned; (2) For a multikey index with path‑level information (passed as a `std::vector<std::set<int>>` where the `k`‑th set contains the multikey component indices for the `k`‑th index field, components are 0‑based subpath positions), a path can be assigned at a given position only if for every multikey component `c` in that set, the prefix of the path up to component `c+1` does not conflict with previously assigned paths (tracked in a caller‑supplied map from prefix to an `$elemMatch` context pointer, where `nullptr` means no `$elemMatch` context). Two paths can be assigned to the same index (at the same or different positions) only if any shared multikey prefix is entirely inside the same `$elemMatch` context (identified by a `const void*` pointer passed alongside each path). The function must modify the provided map to record assignments (setting the prefix to the `$elemMatch` context, or `nullptr` if the prefix is outside any `$elemMatch` or the path is not inside an `$elemMatch`). For simplicity, assume each input path has an associated `$elemMatch` context pointer (may be `nullptr`). The function should return `true` for a path if it can be assigned to the given index position, and `false` otherwise, without modifying the map if the assignment is rejected.
#include <cassert>
#include <string>
#include <vector>
#include <set>
#include <map>

// (Include the solution code here, or assume it's in the same translation unit.)

int main() {
    // Example 1: non-multikey index, all paths assignable.
    {
        StringMap used;
        std::vector<PathInfo> paths = {
            {"a", nullptr, 0},
            {"b.c", nullptr, 0}
        };
        std::vector<std::set<int>> mkPaths(2); // two key fields, but not used because multikey=false
        auto res = canAssignPathsToIndex(paths, 0, false, mkPaths, used);
        assert(res == std::vector<bool>({true, true}));
        assert(used.empty());
    }

    // Example 2: multikey index, leading field has multikey component at prefix "a".
    // Paths "a" and "a" (same path) cannot both be assigned without $elemMatch context.
    {
        StringMap used;
        std::vector<PathInfo> paths = {
            {"a", nullptr, 0},
            {"a", nullptr, 0}
        };
        std::vector<std::set<int>> mkPaths = {{0}}; // first field has multikey component 0 (prefix "a")
        // First path should assign successfully
        auto res1 = canAssignPathsToIndex({paths[0]}, 0, true, mkPaths, used);
        assert(res1 == std::vector<bool>({true}));
        // Second path should fail because "a" is already used with nullptr
        auto res2 = canAssignPathsToIndex({paths[1]}, 0, true, mkPaths, used);
        assert(res2 == std::vector<bool>({false}));
        // used should still contain "a" -> nullptr
        assert(used.size() == 1 && used.count("a") == 1 && used["a"] == nullptr);
    }

    // Example 3: Two paths inside the same $elemMatch can share the multikey prefix.
    {
        StringMap used;
        const char* ctx = "ctx";
        std::vector<PathInfo> paths = {
            {"a.b", ctx, 1}, // elemMatch root path has length 1 (just "a")
            {"a.c", ctx, 1}
        };
        std::vector<std::set<int>> mkPaths = {{0}}; // multikey at component 0 (prefix "a")
        auto res1 = canAssignPathsToIndex({paths[0]}, 0, true, mkPaths, used);
        assert(res1 == std::vector<bool>({true}));
        auto res2 = canAssignPathsToIndex({paths[1]}, 0, true, mkPaths, used);
        assert(res2 == std::vector<bool>({true})); // same ctx, allowed
        assert(used.size() == 1 && used["a"] == ctx);
    }

    // Example 4: Different $elemMatch contexts conflict.
    {
        StringMap used;
        const char* ctx1 = "ctx1";
        const char* ctx2 = "ctx2";
        std::vector<PathInfo> paths = {
            {"a.b", ctx1, 1},
            {"a.c", ctx2, 1}
        };
        std::vector<std::set<int>> mkPaths = {{0}};
        auto res1 = canAssignPathsToIndex({paths[0]}, 0, true, mkPaths, used);
        assert(res1 == std::vector<bool>({true}));
        auto res2 = canAssignPathsToIndex({paths[1]}, 0, true, mkPaths, used);
        assert(res2 == std::vector<bool>({false})); // different ctx
    }

    // Example 5: Multikey component inside elemMatch does not set nullptr, allows later top-level path.
    {
        StringMap used;
        const char* ctx = "ctx";
        // Path "a.b" inside elemMatch, multikey component 1 (prefix "a.b") is inside elemMatch (root length 1, component 1 >= 1)
        // So we record "a.b" -> nullptr (since inside elemMatch), but not "a".
        std::vector<PathInfo> paths = {
            {"a.b", ctx, 1},
            {"a", nullptr, 0} // top-level path, should be fine because no prefix "a" was blocked
        };
        std::vector<std::set<int>> mkPaths = {{1}}; // multikey component 1 only
        auto res1 = canAssignPathsToIndex({paths[0]}, 0, true, mkPaths, used);
        assert(res1 == std::vector<bool>({true}));
        assert(used.size() == 1 && used.count("a.b") == 1 && used["a.b"] == nullptr);
        // Now try top-level "a" – no conflict because "a" not in used
        auto res2 = canAssignPathsToIndex({paths[1]}, 0, true, mkPaths, used);
        assert(res2 == std::vector<bool>({true}));
    }

    // Example 6: Multikey component outside elemMatch blocks later top-level path.
    {
        StringMap used;
        const char* ctx = "ctx";
        // Path "a.b" inside elemMatch, but multikey component 0 (prefix "a") is outside (root length 1, component 0 < 1)
        // So we record "a" -> ctx.
        std::vector<PathInfo> paths = {
            {"a.b", ctx, 1},
            {"a", nullptr, 0} // top-level path on "a" – should conflict
        };
        std::vector<std::set<int>> mkPaths = {{0}};
        auto res1 = canAssignPathsToIndex({paths[0]}, 0, true, mkPaths, used);
        assert(res1 == std::vector<bool>({true}));
        assert(used.size() == 1 && used.count("a") == 1 && used["a"] == ctx);
        auto res2 = canAssignPathsToIndex({paths[1]}, 0, true, mkPaths, used);
        assert(res2 == std::vector<bool>({false}));
    }

    return 0;
}
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

// Define a simple string map using std::map for simplicity (could be unordered).
using StringMap = std::map<std::string, const void*>;

// Struct describing a predicate's field path and its $elemMatch context.
struct PathInfo {
    std::string path;                 // full dotted path, e.g. "a.b"
    const void* elemMatch;            // pointer identifying $elemMatch context, or nullptr
    size_t elemMatchRootLength;       // number of components in the $elemMatch root, 0 if not in one
};

// Helper: return the dotted prefix of `path` up to and including component `component` (0-based).
// Example: path="a.b.c", component=1 -> "a.b"
static std::string dottedPrefix(const std::string& path, size_t component) {
    size_t pos = 0;
    size_t current = 0;
    while (current < component) {
        pos = path.find('.', pos);
        if (pos == std::string::npos) break;
        ++pos;
        ++current;
    }
    if (current != component) {
        // component out of range (shouldn't happen), return whole path
        return path;
    }
    // Find the end of the component (next dot or end)
    std::string result = path.substr(0, path.find('.', pos));
    // But if there is no dot after pos, the prefix is the whole string up to the end.
    // Actually we want up to and including component, so:
    // Let's recompute more simply: split by dot and rejoin.
    // Simpler implementation:
    std::vector<std::string> parts;
    size_t start = 0;
    while (start < path.size()) {
        size_t dot = path.find('.', start);
        if (dot == std::string::npos) {
            parts.push_back(path.substr(start));
            break;
        }
        parts.push_back(path.substr(start, dot - start));
        start = dot + 1;
    }
    if (component >= parts.size()) return path; // fallback
    std::string prefix = parts[0];
    for (size_t i = 1; i <= component && i < parts.size(); ++i) {
        prefix += "." + parts[i];
    }
    return prefix;
}

// Determine if a single path can be assigned to the given index position.
// On success, updates `used`; on failure, leaves `used` unchanged.
// `multikey` : overall index is multikey
// `multikeyPaths` : vector of sets per index key position; index position `pos` must be valid.
bool canAssignOnePath(const PathInfo& info, size_t pos, bool multikey,
                      const std::vector<std::set<int>>& multikeyPaths,
                      StringMap& used) {
    // If index is not multikey or this position has no multikey components, always assignable.
    if (!multikey || multikeyPaths[pos].empty()) {
        // no constraints, no need to update used
        return true;
    }

    // Work on a copy to avoid partial updates.
    StringMap temp = used;

    // Process multikey components in ascending order (shortest prefix first).
    for (int multikeyComponent : multikeyPaths[pos]) {
        std::string prefix = dottedPrefix(info.path, static_cast<size_t>(multikeyComponent));
        auto it = temp.find(prefix);
        if (it == temp.end()) {
            // Not seen before.
            if (info.elemMatch != nullptr && static_cast<size_t>(multikeyComponent) < info.elemMatchRootLength) {
                // Prefix is outside the $elemMatch (i.e., at or above the root).
                temp[prefix] = info.elemMatch;
            } else {
                // Either not in an $elemMatch, or prefix is inside the $elemMatch.
                temp[prefix] = nullptr;
                // Once we mark a prefix as nullptr, shorter prefixes have already been recorded,
                // and longer ones are also blocked, so we can break.
                break;
            }
        } else {
            // Already used: can only share if both are part of the same $elemMatch context.
            const void* recorded = it->second;
            if (recorded == nullptr || recorded != info.elemMatch) {
                return false; // conflict
            }
        }
    }

    // All checks passed; commit the temporary map.
    used = std::move(temp);
    return true;
}

// Main function: returns a vector<bool> for each provided path.
std::vector<bool> canAssignPathsToIndex(const std::vector<PathInfo>& paths,
                                        size_t indexPos,
                                        bool multikey,
                                        const std::vector<std::set<int>>& multikeyPaths,
                                        StringMap& used) {
    std::vector<bool> result;
    result.reserve(paths.size());
    for (const auto& info : paths) {
        // We assume indexPos is valid (0 <= indexPos < multikeyPaths.size())
        bool ok = canAssignOnePath(info, indexPos, multikey, multikeyPaths, used);
        result.push_back(ok);
    }
    return result;
}
// The core algorithm mirrors `canAssignPredToIndex` from the provided code. For each path to assign, we iterate over all multikey components for the specified index position, from shortest to longest. For each such component, we compute the dotted prefix (i.e., the substring up to and including that component). We then check if this prefix has been recorded in the caller’s map:
// - If not recorded: we can tentatively assign it. If the path’s `$elemMatch` context is non‑null and the multikey component is outside that `$elemMatch` (i.e., the component index is less than the number of components in the `$elemMatch` root path – but we don’t have the root path in this simplified task, so we treat "outside" as when the component is less than the component count of the `$elemMatch` path, which we assume is given as an extra parameter? Actually to keep it self‑contained, we’ll assume that the `$elemMatch` context pointer is non‑null, and we treat "outside" as when the component is less than a separate integer `elemMatchRootLength` provided along with each path – but to simplify, we can instead instruct that the caller passes a vector of structures containing path, elemMatch pointer, and `elemMatchRootLength`). For simplicity in the task, we will define that each path has an associated `elemMatchRootLength` (number of components in the `elemMatch` root path, or 0 if not in an `elemMatch`). If the component is less than that length, the prefix is "outside" the `elemMatch`, and we record the `elemMatch` context pointer in the map; otherwise, we record `nullptr` and break (since longer prefixes are now protected). If the prefix is already recorded: we can only assign if the recorded pointer is non‑null and equals the current path’s `elemMatch` pointer; otherwise, assignment fails.
// Edge cases: empty multikey components set (always assignable, but must still record the full path? In the real code, when the set is empty, the index isn’t multikey at that position, so we always assign and no map update is needed? Actually we still need to record the full path to avoid future conflicts? The real code for non‑multikey positions just assigns without checking, and doesn’t update `used` for that path? In our simplification, we’ll handle: if `multikey` is false or the specific position’s set is empty, we return `true` without any map modification for that path (since no multikey prefixes exist). For a multikey position with a non‑empty set, we update the map only on success, adding entries for each prefix we processed (but we only add when we encounter an unrecorded prefix; if we later find a conflict, we do not modify the map at all – we must check all prefixes first before committing). Therefore, we first copy the map, attempt to insert all needed prefixes, and if all succeed, we commit by swapping; otherwise, we keep the original map unchanged.
// Time complexity: for each path, we process at most the number of multikey components for that position (typically small), and each lookup/insertion is O(prefix length). Overall O(P * K * L) where P is number of paths, K max multikey components, L path length. Space complexity O(P) for the temporary map copy.
// The function signature: `std::vector<bool> canAssignPathsToIndex(const std::vector<PathInfo>& paths, int indexPos, bool multikey, const std::vector<std::set<int>>& multikeyPaths, StringMap<const void*>& used)` where `PathInfo` is a struct with fields `path` (std::string), `elemMatch` (const void*), `elemMatchRootLength` (size_t). The function returns a boolean for each input path in order, and modifies `used` only for all successfully assigned paths (atomically per call? Actually in the real code, the function is called per predicate one at a time, but here we want a batch? The task says "returns a vector<bool>" and "modify the provided map". To be faithful, we’ll design it to process each path independently, and for each path, it either succeeds and updates the map immediately, or fails and does not. Due to the "check all prefixes before committing" rule, we need to do a dry‑run on a copy for each path separately. We’ll implement that.
// We need to define a helper to split a dotted path into components and get a prefix. We’ll provide a simple `dottedPrefix` function that returns the substring up to a given component index (0‑based). For example, path `"a.b.c"`, component 0 → `"a"`, component 1 → `"a.b"`, component 2 → `"a.b.c"`.
