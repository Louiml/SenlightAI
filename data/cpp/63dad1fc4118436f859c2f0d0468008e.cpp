// Write a C++ function `int readMultipleTimes(char *buf, int n, int read4Callback)` that simulates the behavior of repeatedly calling an external `read4` API to read up to `n` characters into a destination buffer. The function must maintain internal state across multiple calls (buffering any unread characters from previous `read4` calls) and handle cases where `read4` returns fewer than 4 characters (indicating the end of the stream). The function should return the total number of characters actually read into `buf`, which may be less than `n` if the stream ends early. The `read4Callback` is a function pointer that accepts a character pointer and returns the number of characters written (0-4). The function must correctly handle: reading up to `n` characters across multiple invocations without losing data, correctly returning partial reads when the buffer ends, and not reading beyond the destination buffer size. The internal buffering must persist between calls to the function.

#include <cassert>
#include <cstring>

// Mock read4 that simulates a stream of known content
static const char* testStream = "HelloWorld";
static int streamPos = 0;

int mockRead4(char* buf) {
    int count = 0;
    while (count < 4 && testStream[streamPos] != '\0') {
        buf[count++] = testStream[streamPos++];
    }
    // If the last read returned fewer than 4, next call returns 0
    if (count < 4) {
        // In a real API, we'd mark EOF by returning less than 4, but here we'll just return 0 next time
        // We'll handle by setting a flag externally if needed, but for simplicity we just let it return 0 next call.
    }
    return count;
}

int main() {
    // Reset stream position for each test
    // We need a way to reset, so we'll use a single stream and sequential calls

    // Test 1: Read exactly 5 characters in one call
    streamPos = 0;
    char buf1[20] = {0};
    int n1 = readMultipleTimes(buf1, 5, mockRead4);
    assert(n1 == 5);
    assert(strncmp(buf1, "Hello", 5) == 0);

    // Test 2: Read 3 more characters (should continue from stream)
    char buf2[20] = {0};
    int n2 = readMultipleTimes(buf2, 3, mockRead4);
    assert(n2 == 3);
    assert(strncmp(buf2, "Wor", 3) == 0);

    // Test 3: Read 10 more (stream has "ld" left, then EOF)
    char buf3[20] = {0};
    int n3 = readMultipleTimes(buf3, 10, mockRead4);
    assert(n3 == 2);  // only "ld" remains
    assert(strncmp(buf3, "ld", 2) == 0);

    // Test 4: Read more after EOF should return 0
    char buf4[20] = {0};
    int n4 = readMultipleTimes(buf4, 5, mockRead4);
    assert(n4 == 0);

    // Test 5: n=0 returns 0
    streamPos = 0; // reset for a fresh stream test
    // Need a fresh reader, but static won't reset. So we'll just test that n=0 with current state returns 0
    int n5 = readMultipleTimes(buf4, 0, mockRead4);
    assert(n5 == 0);

    // Test 6: Read larger than buffer but stream ends exactly at boundary
    // Reset by reinitializing would require a new static, so skip or handle careful
    // Instead, test with a fresh mock that returns all 4s then ends
    // We'll use a separate mock function
    struct LocalMock {
        static int count;
        static int pos;
        static const char* data;
        static int read4(char* buf) {
            int c = 0;
            while (c < 4 && data[pos] != '\0') {
                buf[c++] = data[pos++];
            }
            return c;
        }
    };
    LocalMock::data = "123456789";  // 9 characters, read4 will return 4,4,1
    LocalMock::pos = 0;

    // We need to reset the static reader, but it's static. For testing, we can't reset easily.
    // So we rely on the sequential tests above. The function is designed to work across calls.

    return 0;
}

#include <cstring>

// Forward declaration of read4 API (mocked via callback)
using Read4Callback = int (*)(char*);

class BufferedRead4 {
private:
    char buffer[4];
    int bufferSize = 0;
    int bufferPos = 0;
    bool finished = false;

public:
    int read(char* buf, int n, Read4Callback read4) {
        if (n <= 0) {
            return 0;
        }

        int totalRead = 0;

        // First consume any buffered characters from previous calls
        while (totalRead < n && bufferPos < bufferSize) {
            buf[totalRead++] = buffer[bufferPos++];
        }
        // Reset buffer if fully consumed
        if (bufferPos == bufferSize) {
            bufferSize = 0;
            bufferPos = 0;
        }

        // Then read more from the stream if needed
        while (totalRead < n && !finished) {
            char temp[4];
            int count = read4(temp);
            if (count == 0) {
                finished = true;
                break;
            }

            // How many characters we can copy now
            int toCopy = std::min(count, n - totalRead);
            std::memcpy(buf + totalRead, temp, toCopy);
            totalRead += toCopy;

            // If we got more than we need, buffer the excess
            if (count > toCopy) {
                bufferSize = count - toCopy;
                std::memcpy(buffer, temp + toCopy, bufferSize);
                bufferPos = 0;
            }

            // If read4 returned fewer than 4, stream is finished
            if (count < 4) {
                finished = true;
            }
        }

        return totalRead;
    }
};

// Free function wrapper for the task
int readMultipleTimes(char* buf, int n, Read4Callback read4) {
    static BufferedRead4 reader;
    return reader.read(buf, n, read4);
}

// The solution maintains a static or class-level internal buffer of size up to 4 characters (since `read4` returns at most 4 at a time). On each call, we first consume any remaining buffered characters from previous calls, then if more characters are needed, we call `read4` repeatedly to fill the destination buffer up to `n` characters. If `read4` returns fewer than 4 characters, we mark the stream as finished and stop reading. The key algorithm:  
// 1. Start with `totalRead = 0`.  
// 2. First, copy any remaining buffered characters (from previous call) into `buf` up to `n` characters, updating `totalRead`.  
// 3. If `totalRead < n` and the stream is not finished, call `read4` into a temporary buffer, copy as many characters as needed into `buf` (up to `n - totalRead`), and store any excess (if `read4` returned more than we need) into the internal buffer for the next call.  
// 4. If `read4` returns less than 4, mark `finished = true`.  
// 5. Repeat step 3 until `totalRead == n` or `finished`.  
//
// Edge cases:  
// - `n == 0` returns 0 immediately.  
// - If `read4` returns 0 at the very start, the stream is empty and we return 0.  
// - The internal buffer must be cleared if all buffered characters are consumed.  
// - The function must handle multiple calls where each call may have a different `n`.  
//
// Time complexity: Each character is copied at most once from `read4` into the destination or internal buffer, so \(O(n)\) per call (or \(O(4)\) per `read4` call, but total is linear in characters read). Space complexity: \(O(1)\) auxiliary (a fixed 4-character buffer).
