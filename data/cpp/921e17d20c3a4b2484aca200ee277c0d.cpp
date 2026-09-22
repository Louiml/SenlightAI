/*
Write a C++ function named `canonicalizePath` that takes a non-empty string representing an absolute Unix-like file path (starting with `/`) and returns its simplified canonical form. The path may contain single or multiple consecutive slashes, single dots (`.`), double dots (`..`), and regular directory or file names. The canonical form must start with a single `/`, have no trailing slash (except for the root path `/`), treat consecutive slashes as one slash, resolve `.` to the current directory (ignored), and resolve `..` to the parent directory (pop the last component, or if at root, ignore it entirely). If the result is empty, return `/`. The input may contain any combination of uppercase/lowercase letters, digits, underscores, hyphens, and spaces (spaces are valid in names). The function must be `const`-correct and should not modify the input string.
*/

#include <string>
#include <stack>

// Simplified canonical form of an absolute Unix-like path.
std::string canonicalizePath(const std::string& path) {
    std::stack<std::string> components;
    const int n = path.size();

    for (int i = 0; i < n; ++i) {
        if (path[i] == '/') {
            continue;
        }

        std::string current;
        while (i < n && path[i] != '/') {
            current += path[i];
            ++i;
        }

        if (current == ".") {
            continue;
        } else if (current == "..") {
            if (!components.empty()) {
                components.pop();
            }
        } else {
            components.push(current);
        }
    }

    std::string result;
    while (!components.empty()) {
        result = "/" + components.top() + result;
        components.pop();
    }

    return result.empty() ? "/" : result;
}

#include <cassert>

int main() {
    assert(canonicalizePath("/") == "/");
    assert(canonicalizePath("/home/") == "/home");
    assert(canonicalizePath("/a/./b/../../c/") == "/c");
    assert(canonicalizePath("/../") == "/");
    assert(canonicalizePath("/home//foo/") == "/home/foo");
    assert(canonicalizePath("/a/b/c/..") == "/a/b");
    assert(canonicalizePath("/a/../../b") == "/b");
    assert(canonicalizePath("/.../a") == "/.../a");
    assert(canonicalizePath("/a/./b/./c/./d/") == "/a/b/c/d");
    assert(canonicalizePath("/a/b/../c/./d/..") == "/a/c");
    return 0;
}

// The solution uses a stack to track the valid path components. Iterate through the input string character by character. When a slash is encountered, it is skipped. Otherwise, extract the full component until the next slash. If the component is exactly `"."`, skip it. If it is `".."`, pop the top of the stack if non-empty (this handles moving to the parent; if the stack is empty, the path is already at the root so the `..` is ignored). For any other non-empty component, push it onto the stack. After processing the entire string, pop all elements from the stack and build the result by prepending each component with a `/`. If the resulting string is empty, return `/`. Edge cases: root path `"/"` yields empty stack, multiple slashes are naturally skipped, a trailing slash leaves stack unchanged, and `..` at the root is ignored. Time complexity is O(n) where n is the length of the input, as each character is processed once. Space complexity is O(k) for the stack, where k is the number of components in the canonical path.
