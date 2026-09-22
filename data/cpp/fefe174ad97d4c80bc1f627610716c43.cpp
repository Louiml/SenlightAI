Write a C++ function `countTagMatches` that takes a single string representing a sequence of XML-like tags (with opening tags like `<div>`, closing tags like `</div>`, and self-closing tags like `<br/>`), and a vector of query strings, where each query is a sequence of tag names separated by spaces (e.g., `"div p"`). For each query, the function must return the number of positions in the tag stream where the query appears as a contiguous subsequence of matching open tags, considering proper nesting (i.e., an opening tag can only match a query element if it is currently inside the matching parent from the previous query element). Tags are case-sensitive, names contain only letters and digits (up to 10 characters), and the input tag stream is well-formed (every closing tag matches the most recent unmatched opening tag, except self-closing tags which immediately open and close). A self-closing tag does not start a new context for matching a query (it cannot be part of a match because it opens and closes immediately, so it never remains open). The function should return a `std::vector<int>` of counts, one per query, in the order given.
// The solution first parses the tag stream into a vector of events, each being an opening or closing tag with a name. Parsing is done by scanning characters: on `<`, detect whether it is a closing tag (next char `/`) or opening; for self-closing (`/>`), emit both an open and a close event for the same name. After parsing, each query is processed independently. For a query with `L` tag names, maintain a 1-indexed array `depthAtMatch` that records the depth at which each prefix of the query was matched, and a pointer `uk` (1-based) indicating the next needed query index. For every opening event, increase the depth, and if the event’s name matches `query[uk-1]`, either increment the counter (if `uk == L`) or store the current depth in `depthAtMatch[uk]` and advance `uk`. For every closing event, if `uk > 1` and `depthAtMatch[uk-1]` equals the current depth (before decrementing), then retreat `uk` by one; then decrease the depth. This correctly counts each occurrence where a full sequence of query tags is opened in nested order. Edge cases include: self-closing tags (open and close immediately, so they can only complete a match if the query has length 1), overlapping matches with the same starting tag (handled because `uk` does not reset after a count), and matches that start after a previous match has completed (handled by retreating `uk` only when the closing tag actually closes the last matched prefix). Time complexity is O(T + Q·S) where T is the total number of tag events (at most twice the number of tags), Q is the number of queries, and S is the total number of tag names across all queries (since each query scan runs in O(number of tag events)). Space complexity is O(T + maximum query length) for the parsed events and the `depthAtMatch` array.
#include <string>
#include <vector>
#include <sstream>

struct ParsedTag {
    bool open;
    std::string name;
};

// Count occurrences of each query sequence in the tag stream.
std::vector<int> countTagMatches(const std::string& tagStream,
                                 const std::vector<std::string>& queries) {
    // Parse the tag stream into a list of open/close events.
    std::vector<ParsedTag> events;
    int i = 0;
    int n = static_cast<int>(tagStream.size());
    while (i < n) {
        if (tagStream[i] == '<') {
            ++i;  // skip '<'
            bool isClosing = (i < n && tagStream[i] == '/');
            if (isClosing) ++i;  // skip '/'
            
            std::string name;
            while (i < n && tagStream[i] != '>' && tagStream[i] != '/') {
                name += tagStream[i];
                ++i;
            }
            // At this point we are at '>' or '/'
            bool selfClosing = false;
            if (i < n && tagStream[i] == '/') {
                selfClosing = true;
                ++i;  // skip '/'
            }
            if (i < n && tagStream[i] == '>') ++i;  // skip '>'
            
            if (!isClosing) {
                events.push_back({true, name});
                if (selfClosing) {
                    events.push_back({false, name});
                }
            } else {
                events.push_back({false, name});
            }
        } else {
            ++i;  // ignore any stray characters (shouldn't happen)
        }
    }
    
    std::vector<int> result;
    result.reserve(queries.size());
    
    for (const std::string& query : queries) {
        // Split query into tag names
        std::istringstream iss(query);
        std::vector<std::string> q;
        std::string token;
        while (iss >> token) {
            q.push_back(token);
        }
        int L = static_cast<int>(q.size());
        
        // depthAtMatch[k] = depth at which the k-th query tag was matched (1-indexed)
        std::vector<int> depthAtMatch(L + 1, -1);
        int uk = 1;          // next needed query index (1-based)
        int currdepth = 0;
        int ways = 0;
        
        for (const ParsedTag& t : events) {
            if (t.open) {
                ++currdepth;
                if (uk <= L && t.name == q[uk - 1]) {
                    if (uk == L) {
                        ++ways;  // full query matched
                    } else {
                        depthAtMatch[uk] = currdepth;
                        ++uk;
                    }
                }
            } else {  // closing tag
                if (uk > 1 && depthAtMatch[uk - 1] == currdepth) {
                    --uk;  // the last matched tag is closing
                }
                --currdepth;
            }
        }
        
        result.push_back(ways);
    }
    
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be included above.

int main() {
    // Basic nested match
    assert(countTagMatches("<a><b></b></a>", {"a b"}) == std::vector<int>({1}));
    // Multiple child matches with same parent
    assert(countTagMatches("<a><b></b><b></b></a>", {"a b"}) == std::vector<int>({2}));
    // Single tag query
    assert(countTagMatches("<a><a></a></a>", {"a"}) == std::vector<int>({2}));
    // Self-closing tag matches a leaf query
    assert(countTagMatches("<br/>", {"br"}) == std::vector<int>({1}));
    // Self-closing tag cannot be a parent
    assert(countTagMatches("<br/><a></a>", {"br a"}) == std::vector<int>({0}));
    // Nested but overlapping start not counted (per algorithm)
    assert(countTagMatches("<a><a><b></b></a></a>", {"a b"}) == std::vector<int>({1}));
    // Multiple queries
    assert(countTagMatches("<x><y></y></x>", {"x y", "x"}) == std::vector<int>({1, 1}));
    // Empty query? Not allowed, but we test non-spaced
    assert(countTagMatches("<p></p>", {"p"}) == std::vector<int>({1}));
    return 0;
}
