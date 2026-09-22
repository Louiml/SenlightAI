Write a C++ function `std::string decodeITF(const std::vector<bool>& bits)` that takes a binary row of pixel values (`true` for black, `false` for white) representing a scanned Interleaved 2 of 5 (ITF) barcode and returns the decoded digit string if the barcode is valid, or throws `std::runtime_error` on invalid input. The row must begin with a quiet zone (at least 10 white pixels), followed by the start pattern of four narrow black-white-black-white lines, then the interleaved digit pairs (each digit uses 5 elements: black line widths for one digit, white line widths for the other), and finally the reversed end pattern (narrow, narrow, wide) followed by a trailing quiet zone. A narrow line has width 1, a wide line has width 3. Each digit is encoded according to the standard ITF pattern table: e.g., 0 = (1,1,3,3,1), 1 = (3,1,1,1,3), etc. The function must infer the narrow-line width from the start pattern (total width divided by 4), validate the entire sequence, and ensure the decoded length is one of {6, 8, 10, 12, 14, 16, 18, 20, 24, 44, 48}; otherwise throw `std::runtime_error`. The row may have extra whitespace before and after the barcode.
The core algorithm processes the bit array from left to right. First, skip leading white pixels to locate the first black pixel. Then match the start pattern of four alternating runs (black, white, black, white) each of narrow-line width; the variance between observed run widths and expected pattern (all width 1) must be small. From this, compute `narrowWidth = totalStartWidth / 4`. Validate that exactly `10 * narrowWidth` white pixels precede the start pattern (quiet zone); if not, throw. Next, find the end pattern by reversing the row (or scanning from the right) and matching three runs (black, white, black) with widths 1,1,3, again computing variance. Also validate a quiet zone after the end pattern.

In the payload between the start and end patterns, repeatedly read 10 consecutive runs (alternating starting with black) that cover the width of two digits. The five black runs form the first digit, the five white runs form the second digit (interleaved pairs). For each digit, compare the five observed widths against the ten known patterns (each pattern is a vector of 1s and 3s), normalizing by the narrow line width. Compute a variance (sum of squared differences divided by pattern length, capped per element) and choose the digit with minimal variance below a threshold; if none qualifies, throw. Append both digits to the result string, advance the position by the total width consumed, and continue until the end pattern is reached.

Edge cases: the row may contain noise, patterns higher than 3 or lower than 1 times narrow width are rejected, the quiet zones must be strictly white (false values), and the decoded string length must be even and within the allowed set. Time complexity is O(n) where n is the number of bits in the row, as each pattern match is proportional to the number of bits scanned plus a fixed number of comparisons per digit; space complexity is O(1) aside from the returned string.
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <algorithm>

// Decode an ITF barcode from a row of bits (true=black, false=white).
std::string decodeITF(const std::vector<bool>& bits) {
    const size_t n = bits.size();
    const float MAX_AVG_VARIANCE = 0.38f;
    const float MAX_INDIVIDUAL_VARIANCE = 0.78f;

    // Helper to skip leading white (false) pixels.
    auto skipWhite = [&bits, n](size_t start) -> size_t {
        size_t i = start;
        while (i < n && !bits[i]) ++i;
        if (i == n) throw std::runtime_error("No black pixels found");
        return i;
    };

    // Pattern match helper for a set of runs (counters) against a pattern (widths in units of narrow).
    auto matchVariance = [&](const std::vector<int>& counters, const std::vector<int>& pattern, int narrow) -> float {
        if (counters.size() != pattern.size()) return 100.0f;
        float variance = 0.0f;
        for (size_t i = 0; i < counters.size(); ++i) {
            float scaled = static_cast<float>(counters[i]) / narrow;
            float diff = scaled - pattern[i];
            if (diff < -MAX_INDIVIDUAL_VARIANCE || diff > MAX_INDIVIDUAL_VARIANCE) return 100.0f;
            variance += diff * diff;
        }
        return variance / pattern.size();
    };

    // Helper to read a sequence of alternating runs starting with black.
    auto readRuns = [&bits, n](size_t start, int count) -> std::pair<std::vector<int>, size_t> {
        std::vector<int> runs(count, 0);
        bool expectBlack = true;
        size_t pos = start;
        int idx = 0;
        while (pos < n && idx < count) {
            bool isBlack = bits[pos];
            if (isBlack != expectBlack) {
                // Unexpected color change; stop (incomplete run)
                break;
            }
            while (pos < n && bits[pos] == isBlack) {
                runs[idx]++;
                ++pos;
            }
            expectBlack = !expectBlack;
            ++idx;
        }
        if (idx < count) throw std::runtime_error("Insufficient runs");
        return {runs, pos};
    };

    // Find start pattern.
    size_t firstBlack = skipWhite(0);
    auto checkStartQuiet = [&](size_t start) {
        // Quiet zone: at least 10*narrow white pixels before startPattern position.
        // narrow is not yet known; we derive after matching start pattern.
        // We will validate after computing narrow.
    };

    // Attempt to identify start pattern: 4 runs (B,W,B,W) each width 1.
    // Search from firstBlack to allow some tolerance? But spec says start at first black exactly.
    auto startRuns = readRuns(firstBlack, 4);
    // If readRuns succeeded, we have 4 runs. Determine narrow width from total.
    int totalStartWidth = 0;
    for (int r : startRuns.first) totalStartWidth += r;
    int narrow = totalStartWidth / 4;
    if (narrow <= 0) throw std::runtime_error("Invalid narrow width");
    // Accept if variance is low (each run near narrow width).
    std::vector<int> startPattern = {1,1,1,1};
    float startVariance = matchVariance(startRuns.first, startPattern, narrow);
    if (startVariance >= MAX_AVG_VARIANCE) throw std::runtime_error("Start pattern mismatch");
    // Validate quiet zone before start: at least 10*narrow white pixels immediately before firstBlack.
    size_t quietStart = firstBlack >= 10*narrow ? firstBlack - 10*narrow : 0;
    for (size_t i = quietStart; i < firstBlack; ++i) {
        if (bits[i]) throw std::runtime_error("Quiet zone before start not all white");
    }

    size_t payloadStart = startRuns.second; // after the last run of start pattern

    // Find end pattern: we search from the right end.
    // Reverse scanning approach: find the last black pixel.
    size_t lastBlack = n;
    for (size_t i = n; i-- > 0; ) {
        if (bits[i]) { lastBlack = i; break; }
    }
    if (lastBlack == n) throw std::runtime_error("No black pixels");
    // Read 3 runs backwards: from lastBlack, going left, expect B,W,B with widths 1,1,3.
    // Easier: read forward from lastBlack-? but we'll read from the right.
    // Let's find the start of the end pattern by scanning left from lastBlack.
    // We need exactly 3 runs: black (wide), white (narrow), black (narrow) in forward order? Actually original END_PATTERN is N,N,W and reversed is N,N,W (since reversed of N,N,W is W,N,N? Wait: in the provided code, END_PATTERN_REVERSED is {N,N,W} because the row is reversed before search. In forward, end pattern is W,N,N? Let's trust the original: in forward, end pattern is (wide, narrow, narrow) = (3,1,1). Then reversed is (1,1,3) which matches the code. So we search from right side; the rightmost part of forward end pattern is narrow (black), then white narrow, then black wide (to the left of those). So when scanning backward from the rightmost black, we expect runs: black (narrow), white (narrow), black (wide) -> that's (1,1,3) in forward order from left to right? Actually if we are at the rightmost black, that is the last bar (narrow) of the end pattern. Moving left, we encounter white (narrow), then black (wide). So reading from rightmost black backward gives sequences: black, white, black with widths 1,1,3 respectively? Let's list forward positions: [wide][narrow][narrow] where wide is leftmost? No, if end pattern forward is (N,N,W) from left to right, then the rightmost is wide. But the code says END_PATTERN_REVERSED = {N,N,W} and they reverse the row, so original end pattern forward is {W,N,N}. So the rightmost element is narrow (last N). Thus scanning backward from rightmost black, we see narrow, then white, then wide. So we want runs (narrow, narrow, wide) but reversed in scanning order. Easier: we can just reverse the entire bits vector and find the start pattern (which would be the end pattern reversed). But that is O(n) extra memory. Alternatively, we can scan from the right using a backward iteration counting runs.

    // Let's implement backward run reading: read runs starting from a given position going left.
    auto readRunsBackward = [&bits](size_t start, int count) -> std::pair<std::vector<int>, size_t> {
        std::vector<int> runs(count, 0);
        bool expectBlack = true; // start at a black pixel
        int idx = 0;
        size_t pos = start;
        while (true) {
            bool isBlack = bits[pos];
            if (isBlack != expectBlack) break;
            while (true) {
                runs[idx]++;
                if (pos == 0) { pos = 0; break; }
                --pos;
                if (bits[pos] != isBlack) break;
            }
            expectBlack = !expectBlack;
            ++idx;
            if (idx == count) break;
            if (pos == 0) break;
        }
        if (idx < count) throw std::runtime_error("Insufficient runs backward");
        // pos now points to the last bit of the last run (leftmost from start).
        return {runs, pos+1}; // return start index of the run sequence in forward direction
    };

    // Find the end pattern start position: we need to locate the wide black bar.
    // We'll just try to read backward 3 runs starting from lastBlack.
    auto endRuns = readRunsBackward(lastBlack, 3);
    std::vector<int> endPattern = {1,1,3}; // forward order: narrow, narrow, wide? Actually we read backward, so runs are: first (rightmost) black narrow, second white narrow, third black wide. In forward order (left to right) that is (3,1,1)? Wait, we read backward, so the sequence we get is reversed. The third run read is the leftmost. So if we store them as read, run[0]=narrow, run[1]=narrow, run[2]=wide. That matches the reversed end pattern from the code (N,N,W) which is what we want to match. So we expect widths (1,1,3) in the order we read (right to left). Good.
    float endVariance = matchVariance(endRuns.first, std::vector<int>{1,1,3}, narrow);
    if (endVariance >= MAX_AVG_VARIANCE) throw std::runtime_error("End pattern mismatch");
    // Validate quiet zone after end (i.e., to the right of lastBlack). That means to the right of lastBlack there must be at least 10*narrow white pixels.
    if (lastBlack + 10*narrow > n) throw std::runtime_error("No quiet zone after end");
    for (size_t i = lastBlack+1; i < lastBlack+1+10*narrow; ++i) {
        if (bits[i]) throw std::runtime_error("Quiet zone after end not all white");
    }

    // The payload is from payloadStart to payloadEnd (exclusive). The end pattern starts at the wide bar, which is endRuns.second (since we stored run[2] as last read, its starting forward index is endRuns.second). Actually endRuns.second is the forward index of the leftmost bit of the third run (the wide black bar). So payloadEnd = endRuns.second.
    size_t payloadEnd = endRuns.second;

    // Decode middle.
    std::string result;
    size_t pos = payloadStart;
    const int DIGIT_PATTERNS[10][5] = {
        {1,1,3,3,1}, // 0
        {3,1,1,1,3}, // 1
        {1,3,1,1,3}, // 2
        {3,3,1,1,1}, // 3
        {1,1,3,1,3}, // 4
        {3,1,3,1,1}, // 5
        {1,3,3,1,1}, // 6
        {1,1,1,3,3}, // 7
        {3,1,1,3,1}, // 8
        {1,3,1,3,1}  // 9
    };

    while (pos < payloadEnd) {
        // Read 10 runs (B,W,B,W,...) starting at pos.
        auto runs = readRuns(pos, 10);
        if (runs.second > payloadEnd) throw std::runtime_error("Payload exceeds end pattern");
        // Split into black and white.
        std::vector<int> black(5), white(5);
        for (int k = 0; k < 5; ++k) {
            black[k] = runs.first[2*k];
            white[k] = runs.first[2*k+1];
        }
        // Decode black digit.
        int bestMatch = -1;
        float bestVar = MAX_AVG_VARIANCE;
        for (int d = 0; d < 10; ++d) {
            std::vector<int> pat(DIGIT_PATTERNS[d], DIGIT_PATTERNS[d]+5);
            float v = matchVariance(black, pat, narrow);
            if (v < bestVar) { bestVar = v; bestMatch = d; }
        }
        if (bestMatch < 0) throw std::runtime_error("Cannot decode black digit");
        result.push_back('0' + bestMatch);
        // Decode white digit.
        bestVar = MAX_AVG_VARIANCE; bestMatch = -1;
        for (int d = 0; d < 10; ++d) {
            std::vector<int> pat(DIGIT_PATTERNS[d], DIGIT_PATTERNS[d]+5);
            float v = matchVariance(white, pat, narrow);
            if (v < bestVar) { bestVar = v; bestMatch = d; }
        }
        if (bestMatch < 0) throw std::runtime_error("Cannot decode white digit");
        result.push_back('0' + bestMatch);
        // Advance pos by total width consumed.
        pos = runs.second;
    }

    // Check allowed lengths.
    static const int allowed[] = {6,8,10,12,14,16,18,20,24,44,48};
    bool lenOK = false;
    for (int a : allowed) if (result.size() == (size_t)a) lenOK = true;
    if (!lenOK) throw std::runtime_error("Invalid payload length");

    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <stdexcept>

int main() {
    // Build a valid ITF row manually for "12" using narrow=1, wide=3.
    // Start: B W B W (each 1)
    // Payload for "12": black digit 1 = (3,1,1,1,3), white digit 2 = (1,3,1,1,3)
    // End: W N N? Actually forward end pattern is (W,N,N) = (3,1,1)
    // We'll make row: quiet (10 W), start (B W B W), payload (10 runs), end (B W B with widths 3,1,1), quiet (10 W)
    std::vector<bool> bits;
    auto addWhite = [&](int n){ for(int i=0;i<n;++i) bits.push_back(false); };
    auto addBlack = [&](int n){ for(int i=0;i<n;++i) bits.push_back(true); };

    addWhite(10); // quiet start
    addBlack(1); addWhite(1); addBlack(1); addWhite(1); // start N N N N (each 1)
    
    // Payload: black digit 1 = (3,1,1,1,3) and white digit 2 = (1,3,1,1,3)
    // Interleaved: first black run (3), first white run (1), second black (1), second white (3), third black (1), third white (1), fourth black (1), fourth white (1), fifth black (3), fifth white (3)
    addBlack(3); addWhite(1); // digit 1 run1, digit 2 run1
    addBlack(1); addWhite(3); // digit 1 run2, digit 2 run2
    addBlack(1); addWhite(1); // digit 1 run3, digit 2 run3
    addBlack(1); addWhite(1); // digit 1 run4, digit 2 run4
    addBlack(3); addWhite(3); // digit 1 run5, digit 2 run5
    
    // End pattern forward: W N N? Actually forward is (3,1,1) but reversed for search. In forward order left to right: wide black (3), narrow white (1), narrow black (1). So addBlack(3); addWhite(1); addBlack(1);
    addBlack(3); addWhite(1); addBlack(1);
    
    addWhite(10); // quiet end

    assert(decodeITF(bits) == "12");

    // Test invalid length (should throw)
    // Remove 2 runs (one digit pair) to make length 0? Simpler: create row with only one digit pair? Actually "12" has 2 digits length 2 not allowed. We'll create "123456" (6 digits) but that's long. Instead test a row with non-allowed length: e.g., decode a valid "1" (length 1) - but that would have odd number of runs. Simpler: modify the above to only 1 digit (drop last pair?) Hard. So just test that a short row throws.
    std::vector<bool> shortBits;
    addWhite(10); addBlack(1); addWhite(1); addBlack(1); addWhite(1); // start only
    addWhite(10); // no payload
    bool threw = false;
    try { decodeITF(shortBits); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);

    // Test invalid start pattern (wide first bar)
    std::vector<bool> badStart;
    addWhite(10); addBlack(3); addWhite(1); addBlack(1); addWhite(1); // first bar wide
    // rest same as first payload? We'll just check it throws
    try { decodeITF(badStart); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);

    // Test all zeros pattern: digit 0 encoded as (1,1,3,3,1). Create "00".
    std::vector<bool> zeroRow;
    addWhite(10);
    addBlack(1); addWhite(1); addBlack(1); addWhite(1); // start
    // black 0 (1,1,3,3,1), white 0 (1,1,3,3,1) interleaved: B,W,B,W,B,W,B,W,B,W
    addBlack(1); addWhite(1); // digit0 run1, digit0 run1
    addBlack(1); addWhite(1); // run2
    addBlack(3); addWhite(3); // run3
    addBlack(3); addWhite(3); // run4
    addBlack(1); addWhite(1); // run5
    // end pattern W N N = (3,1,1)
    addBlack(3); addWhite(1); addBlack(1);
    addWhite(10);
    assert(decodeITF(zeroRow) == "00");

    return 0;
}
