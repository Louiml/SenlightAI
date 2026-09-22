Implement a C++ function that simulates a simplified text-based browser rendering system. Given a description of HTML-like markup, a set of clickable elements (links and buttons), and a sequence of screen dimensions and click coordinates, the function must output the final rendered screen after processing all clicks. The markup uses tags `<br>` (line break), `<script>` (loads a script file), `<link>` (navigates to another page), and `<button>` (triggers a script). The input format is: first an integer `n` (number of files), followed by `n` lines each starting with a filename (ending `.dml` or `.ds`) and then the file content on the next line. Then an integer `m` (number of testcases), and for each testcase: a line with `width height K filename`, followed by `K` lines of click coordinates `x y`. The function must return a single string containing the final screens of all testcases separated by newlines, where each screen is a grid of characters, rows separated by newlines.

// The solution must parse the `.dml` and `.ds` files into internal tree and function structures. The `.dml` parser builds a tree of `Node` objects with tags like `$text`, `script`, `link`, `button`, `br`, or arbitrary container tags. The `.ds` parser parses a list of functions, each with a name and a list of assignments like `path.to.element.visible = true/false`. The rendering process starts with an initial file, executes all `init()` (which loads script functions), then handles clicks: if a click hits a link, it navigates to that target page (after clearing functions and reinitializing); if it hits a button, it executes the associated function and re-renders the current page. The screen is a 2D character array initialized to dots, and drawing uses `draw()` which handles line wrapping and boundary clipping. Key edge cases: empty text, tags with children, visibility flags, multiple assignments in one expression, reversing assignments (using `!` or `=`), and ensuring root visibility is always true. Time complexity is \(O(T \cdot K \cdot S)\) where `T` is number of testcases, `K` is clicks per testcase, `S` is the size of the rendered tree (for applying visibility changes), and space complexity is \(O(T \cdot S)\).

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <cctype>
#include <cstdlib>
using namespace std;

// Simulate the simplified browser rendering system.
// Input: multiline string as per problem spec.
// Output: concatenated final screens for all testcases, each screen's rows separated by '\n', testcases separated by '\n'.
string simulateBrowser(const string& input) {
    // Local typedefs
    struct Node;
    struct Fun;

    // State variables
    int screen_h = 0, screen_w = 0;
    int cur_row = 0, cur_col = 0;
    vector<string> screen;
    vector<vector<Node*>> link_cell;
    vector<vector<Fun*>> evt_cell;

    // Global maps
    map<string, vector<pair<string, Fun*>>> scripts;
    map<string, Node*> pages;
    map<string, Fun*> funcs;
    Node* act = nullptr;

    // Helper functions (lambda captures)
    auto newline = [&]() {
        cur_row++;
        cur_col = 0;
    };

    auto draw = [&](const string& s, Node* hl, Fun* fn) {
        for (int i = 0; i < (int)s.size(); ++i) {
            if (cur_row >= 0 && cur_row < screen_h && cur_col >= 0 && cur_col < screen_w) {
                screen[cur_row][cur_col] = s[i];
                link_cell[cur_row][cur_col] = hl;
                evt_cell[cur_row][cur_col] = fn;
                cur_col++;
            }
            if (cur_col == screen_w) newline();
        }
    };

    struct Node {
        string tag;
        string text;
        vector<Node*> cs;
        bool visible;

        Node(const string& t) : tag(t), visible(true) {}

        // Render this node into screen
        function<void()> render = [&]() {
            if (!visible) return;
            if (tag == "$text") {
                draw(text, nullptr, nullptr);
            } else if (tag == "script") {
                // content only, do not render
            } else if (tag == "link") {
                assert(cs.size() == 1 && cs[0]->tag == "$text");
                assert(pages.count(cs[0]->text));
                draw(cs[0]->text, pages[cs[0]->text], nullptr);
            } else if (tag == "button") {
                assert(cs.size() == 1 && cs[0]->tag == "$text");
                draw(cs[0]->text, nullptr, funcs[cs[0]->text]);
            } else if (tag == "br") {
                newline();
            } else {
                for (auto c : cs) c->render();
            }
        };

        // Initialize scripts and recurse
        function<void()> init = [&]() {
            visible = true;
            if (tag == "script") {
                assert(cs.size() == 1 && cs[0]->tag == "$text");
                string file = cs[0]->text;
                for (auto& p : scripts[file]) {
                    funcs[p.first] = p.second;
                }
            }
            for (auto c : cs) c->init();
        };

        // Apply visibility assignments
        function<void(const vector<string>&, int, bool)> apply = [&](const vector<string>& vs, int k, bool visi) {
            if (vs[k] == tag) {
                if (k == (int)vs.size() - 1) {
                    visible = visi;
                } else {
                    k++;
                }
            }
            for (auto c : cs) c->apply(vs, k, visi);
        };
    };

    struct Fun {
        vector<pair<vector<string>, bool>> asn;
        void exec() {
            for (auto& a : asn) {
                act->apply(a.first, 0, a.second);
            }
        }
    };

    // Lexer for .dml
    auto lex_dml = [](const string& s) {
        vector<string> ts;
        int pos = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (s[i] == '<') {
                ts.push_back(s.substr(pos, i - pos));
                pos = i;
            } else if (s[i] == '>') {
                ts.push_back(s.substr(pos, i + 1 - pos));
                pos = i + 1;
            }
        }
        ts.push_back(s.substr(pos));
        return ts;
    };

    auto isbegin = [](const string& t) {
        if (t.size() < 3) return false;
        if (t[0] != '<' || t.back() != '>') return false;
        for (int i = 1; i < (int)t.size() - 1; ++i) {
            if (!isalnum(t[i])) return false;
        }
        return true;
    };

    auto isend = [](const string& t) {
        if (t.size() < 4) return false;
        if (t[0] != '<' || t[1] != '/' || t.back() != '>') return false;
        for (int i = 2; i < (int)t.size() - 1; ++i) {
            if (!isalnum(t[i])) return false;
        }
        return true;
    };

    // Parser for .dml
    auto parse_dml = [&](const vector<string>& ts) {
        Node* root = new Node("$root");
        vector<Node*> stk;
        stk.push_back(root);
        for (auto& t : ts) {
            if (t.empty()) continue;
            if (isbegin(t)) {
                Node* node = new Node(t.substr(1, t.size() - 2));
                stk.back()->cs.push_back(node);
                if (t != "<br>") stk.push_back(node);
            } else if (isend(t)) {
                assert(stk.back()->tag == t.substr(2, t.size() - 3));
                stk.pop_back();
            } else {
                Node* node = new Node("$text");
                node->text = t;
                stk.back()->cs.push_back(node);
            }
        }
        assert(stk.size() == 1);
        return root;
    };

    // Parse a property path like "a.b.c.visible"
    auto parse_prop = [](const string& s) {
        vector<string> ps;
        int pos = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (s[i] == '.') {
                ps.push_back(s.substr(pos, i - pos));
                pos = i + 1;
            }
        }
        assert(s.substr(pos) == "visible");
        return ps;
    };

    // Parser for .ds
    string _s;
    unsigned _ix;

    function<vector<pair<vector<string>, bool>>()> parse_expr = [&]() {
        unsigned pos = _ix;
        vector<string> props;
        vector<bool> rev;
        while (_s[_ix] != ';') {
            if (_s[_ix] == '!' || _s[_ix] == '=') {
                props.push_back(_s.substr(pos, _ix - pos));
                if (_s[_ix] == '!') {
                    _ix += 2;
                    pos = _ix;
                    rev.push_back(true);
                } else {
                    _ix += 1;
                    pos = _ix;
                    rev.push_back(false);
                }
            } else {
                _ix++;
            }
        }
        string val = _s.substr(pos, _ix - pos);
        bool cur = val == "true";
        vector<pair<vector<string>, bool>> rs;
        for (int i = (int)props.size() - 1; i >= 0; --i) {
            if (rev[i]) cur = !cur;
            rs.push_back(make_pair(parse_prop(props[i]), cur));
        }
        _ix++;
        return rs;
    };

    function<pair<string, Fun*>()> parse_fun = [&]() {
        Fun* fun = new Fun();
        const unsigned st = _ix;
        while (_s[_ix] != '{') _ix++;
        const string id = _s.substr(st, _ix - st);
        _ix++;
        while (_s[_ix] != '}') {
            auto es = parse_expr();
            for (auto& e : es) fun->asn.push_back(e);
        }
        _ix++;
        return make_pair(id, fun);
    };

    auto parse_ds = [&](const string& s) {
        _s = s;
        _ix = 0;
        vector<pair<string, Fun*>> fs;
        while (_ix < _s.size()) {
            fs.push_back(parse_fun());
        }
        return fs;
    };

    // Render a file into screen
    auto render = [&](Node* file) {
        screen.assign(screen_h, string(screen_w, '.'));
        link_cell.assign(screen_h, vector<Node*>(screen_w, nullptr));
        evt_cell.assign(screen_h, vector<Fun*>(screen_w, nullptr));
        cur_row = cur_col = 0;
        file->render();
        act = file;
    };

    // Click handler
    auto click = [&](int x, int y) {
        if (link_cell[y][x]) {
            funcs.clear();
            link_cell[y][x]->init();
            render(link_cell[y][x]);
        } else if (evt_cell[y][x]) {
            evt_cell[y][x]->exec();
            render(act);
        }
    };

    // Parse input lines
    istringstream input_stream(input);
    string line;
    getline(input_stream, line);
    int n = atoi(line.c_str());
    for (int i = 0; i < n; ++i) {
        getline(input_stream, line);
        // Remove trailing \r if present (common on Windows)
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.size() >= 4 && line.substr(line.size() - 4) == ".dml") {
            string file = line.substr(0, line.size() - 4);
            getline(input_stream, line);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            auto ts = lex_dml(line);
            Node* root = parse_dml(ts);
            pages[file] = root;
        } else if (line.size() >= 3 && line.substr(line.size() - 3) == ".ds") {
            string file = line.substr(0, line.size() - 3);
            getline(input_stream, line);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            auto fs = parse_ds(line);
            scripts[file] = fs;
        } else {
            assert(false);
        }
    }

    getline(input_stream, line);
    int m = atoi(line.c_str());
    string result;
    for (int tc = 0; tc < m; ++tc) {
        getline(input_stream, line);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        int w, h, K;
        char buf[32];
        sscanf(line.c_str(), "%d %d %d %s", &w, &h, &K, buf);
        screen_w = w;
        screen_h = h;
        pages[buf]->init();
        render(pages[buf]);
        for (int cl = 0; cl < K; ++cl) {
            getline(input_stream, line);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            int x, y;
            sscanf(line.c_str(), "%d %d", &x, &y);
            click(x, y);
        }
        // Append screen to result
        for (int i = 0; i < h; ++i) {
            if (i > 0) result += '\n';
            result += screen[i];
        }
        if (tc < m - 1) result += '\n';
    }
    return result;
}

#include <cassert>
#include <string>
using namespace std;

string simulateBrowser(const string& input);

int main() {
    // Test 1: Simple two pages with link navigation
    string input1 = "2\npage.dml\n<root>Hello <link>next</link></root>\nnext.dml\n<root>World<br>Done</root>\n1\n5 3 1 page.dml\n3 1\n";
    string out1 = simulateBrowser(input1);
    // Expected: after clicking at (3,1) which is inside "next", navigate to next.dml
    // page.dml renders "Hello next" with "next" as link. Click at (3,1) coords: 0-indexed row=1, col=3 => within "next" (cols 6-9? Actually width=5, so "Hello" cols0-4, newline at col5, then "next" at row1 cols0-3, so (3,1) inside "next")
    // next.dml renders "World" on row0 and "Done" on row1.
    assert(out1 == "World\nDone");

    // Test 2: Button visibility script
    string input2 = "2\npage2.dml\n<root><script>hide.ds</script><div><text>Visible</text></div><button>Press</button></root>\nhide.ds\nPress{toggle.visible=false;}\n1\n10 2 1 page2.dml\n0 0\n";
    // After clicking button at (0,0) - but button text "Press" at row0 col0-4? We'll just place button at row0 col0, so click hits it -> exec Press -> hide "toggle" (which doesn't exist, but apply just traverses tree, no visible change). Index: out2 should still show "Press" and "Visible"
    string out2 = simulateBrowser(input2);
    assert(out2 == "Press     \nVisible   ");

    // Test 3: Br and line wrap
    string input3 = "1\np.dml\n<root>123<br>456</root>\n1\n3 2 0 p.dml\n";
    string out3 = simulateBrowser(input3);
    assert(out3 == "123\n456");

    // Test 4: Clipping at screen edge
    string input4 = "1\np4.dml\n<root>ABCDEF</root>\n1\n3 1 0 p4.dml\n";
    string out4 = simulateBrowser(input4);
    assert(out4 == "ABC");

    // Test 5: Multiple assignments and negation
    string input5 = "2\np5.dml\n<root><script>s.ds</script><a><b>C</b></a><button>B</button></root>\ns.ds\nB{a.b.visible=false; a.visible=true;}\n1\n5 2 1 p5.dml\n0 0\n";
    // Click at B (row0) -> hides a.b, so "C" disappears, leaving only "B"
    string out5 = simulateBrowser(input5);
    assert(out5 == "B    \n     ");

    return 0;
}
