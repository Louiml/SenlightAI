Given an undirected graph with N vertices and M edges, write a C++ function `void solvePathPairs(int n, int m, const vector<pair<int,int>>& edges)` that determines whether there exist two vertex-disjoint simple paths (sharing no vertices) connecting four distinct vertices a, b, c, d, where the first path goes from a to b and the second from c to d. More precisely, the function must detect if there are two vertex-disjoint path pairs (a→b and c→d) such that all four endpoints are distinct and the paths are internally vertex-disjoint (they cannot share any vertex). If such a pair exists, the function should print on standard output: first a line "YES", then two lines, each containing a path as a space-separated sequence of vertices (the first line is the a-to-b path, the second is the c-to-d path). If no such pair exists, print "NO". The graph is connected? Not necessarily; it may have multiple components. Vertex indices are 1-based. You may assume the graph is simple (no self-loops, no multiple edges). The function must handle graphs up to 200,000 vertices and 400,000 edges, with time complexity O(N+M). If multiple decompositions exist, any valid one is acceptable. The function must be deterministic and not rely on randomness. Print each path's vertices in order from source to destination.

// We need to find two vertex-disjoint paths between four distinct vertices. This is equivalent to detecting a pair of crossing cycles in a depth-first search tree. The key observation: if two cycles in the graph share a vertex, then we can extract two vertex-disjoint paths from them (one "top" and one "bottom" portion). The algorithm performs a DFS on the undirected graph (handling each connected component separately). For each DFS tree edge, we compute the depth of each node. When a back edge (x,y) is encountered (y is an ancestor of x in the DFS tree), we have a cycle determined by the tree path from x up to y plus the back edge. We can "register" this cycle by storing its two endpoints (x and y) on every node along the tree path between x and y, but only the first cycle each node sees. Then, when a new cycle is discovered that shares a node with a previously registered cycle, we can combine them: the two cycles share a vertex, so we can extract a pair of vertex-disjoint paths. The extraction works as follows: suppose we have a cycle (u,v) from back edge and a previously stored cycle (fx, fy) on a node z that lies on the path between u and v. We need to output two vertex-disjoint paths. The construction in the reference solution uses an LCA-based decomposition: it finds the LCA of endpoints and prints three segments such that the middle segment is shared? Wait, careful: The reference prints two paths with three flush calls? Actually the reference prints: first path from e (LCA of a,c) to d, then reversed, then from d to b and a to e, then a third path from d to c to e. That seems to produce 3 lines? But the problem asks for exactly 2 lines after YES. Let's carefully inspect: In the reference, after finding a cycle pair, it calls print(a,b,c,d) where a,b are the stored cycle endpoints and c,d are the new cycle endpoints. The print function computes e = LCA(a,c). Then it prints path e→d (reverse), then path d→b plus a→e (that's two segments flushed together as one line? Actually it does ppath(d,b) then ppath(a,e) and flushes after both, so that's one line containing d...b and then a...e connected? That would be a single path from d to b then a to e? That's not a simple path because it jumps from b to a. Wait, the code: 
// ```
// ppath(e,d), reverse(st+1,st+tp+1), flush();
// ppath(d,b), ppath(a,e), flush();
// st[++tp]=d, ppath(c,e), flush(), exit(0);
// ```
// So first line: reversed path e→d (so d→e). Second line: path d→b then path a→e concatenated, so that's not a single path; it would have d...b a...e, which would have a discontinuity between b and a. That seems wrong unless b and a are the same? Actually hold on: In the context, the two cycles are (a,b) and (c,d) (where a,b are the earlier cycle's back edge endpoints and c,d are the new cycle's back edge endpoints). Both cycles share at least one node. The print function assumes that b is an ancestor of both a and c? Let's reason: In the DFS, when a back edge (x,y) is found, y is an ancestor of x. So the cycle endpoints are (ancestor y, descendant x). The stored fx,fy are such that fx is descendant, fy is ancestor? Actually the code: when a back edge from x to y (y ancestor) is found, it iterates u from x up to y (going up via fa) and stores fx[u]=x, fy[u]=y. So fx is the descendant endpoint (call it a) and fy is the ancestor endpoint (call it b). So for any stored cycle, the first endpoint is deeper (descendant) and second is shallower (ancestor). Similarly, the new back edge (x,y) has x=descendant, y=ancestor, so in print(a,b,c,d) we have a=x (descendant), b=y (ancestor), c=x? No, when we find a new back edge (x,y), we check for nodes u on path from x to y that have fx[u] nonzero, and then call print(fx[u], fy[u], x, y) where fx[u] is the stored descendant, fy[u] is the stored ancestor, and x, y are the new descendant and ancestor respectively. So a=fx[u], b=fy[u], c=x, d=y. Here a and c are descendants (deeper), b and d are ancestors (shallower). Also because u lies on both tree paths from a to b and from c to d, we have that a and c are both descendants of u, and b and d are ancestors of u (or equal to u). The LCA of a and c is u (or perhaps deeper? Since u is on both paths, the LCA of a and c is exactly u if u is the lowest common ancestor? Not necessarily: u could be above the LCA. But because u is on both tree paths from descendant to ancestor, u is a common ancestor of a and c. The LCA of a and c is some node w that is also on both paths, and w is a descendant of or equal to b and d? Actually b and d are ancestors of u, so they are also ancestors of w. We compute e = LCA(a,c). Then we print: path e→d (reversed to d→e), then path d→b and a→e concatenated? That still doesn't make a simple path. Let's manually verify with a small example: Suppose tree edges: root 1, children 2 and 3, and 2 has child 4, 3 has child 5. Also back edges: (4,3) and (5,2). These share no vertex, so not relevant. Need two cycles sharing a vertex. Example: triangle 1-2-3-1 and triangle 1-2-4-1? Actually let's build: vertices 1,2,3,4. Edges: 1-2, 2-3, 3-1 (triangle), and 1-4, 4-2 (another triangle sharing edge 1-2). DFS from 1: visit 2, then 3 (back edge 3-1), then 4 (back edge 4-2? 4 is child of 1? Actually if we visit 1->2->3, back edge 3-1. Then from 2 go to 4, back edge 4-1? Let's set edges: 1-2,2-3,3-1 (cycle1), 1-4,4-2 (cycle2). DFS tree: 1->2, 2->3 (back to 1), 2->4 (back to 1). So cycles: first back edge (3,1) stores on nodes 3 and 2: fx=3,fy=1. Second back edge (4,1) stores on nodes 4 and 2: but node 2 already has fx=3,fy=1, so when processing node 2 for the second cycle, we find fx[2] nonzero, so we call print(fx[2]=3, fy[2]=1, c=4, d=1). So a=3, b=1, c=4, d=1. Here e = LCA(3,4). In tree, 3's parent is 2, 4's parent is 2, LCA is 2. Now print: ppath(e=2,d=1) -> gives path 2->1, reverse -> 1 2. flush line1: "1 2". Then ppath(d=1,b=1) -> path 1->1? Actually ppath(x,y) walks up from x to y, so 1 to 1 gives just [1]. Then ppath(a=3,e=2) -> path 3->2. Then flush line2: "1 3 2"? That's not a simple path because it goes 1,3,2 (that's fine, it's a simple path from 1 to 2? Actually line2 is concatenation of path d→b (1) and path a→e (3->2) giving sequence [1,3,2]. That is a simple path from 1 to 2 via 3? But we have edge 1-3 (triangle), so that's valid path 1-3-2. Then third line: st[++tp]=d (1), ppath(c=4,e=2) -> path 4->2, so sequence [1,4,2]. That's a third path. So we output 3 lines after YES: line1 "1 2", line2 "1 3 2", line3 "1 4 2". That's three paths, but the problem says two paths. Actually the reference code's print function outputs three lines: line1 is a path from d to e (reversed), line2 is path d→b concatenated with a→e, line3 is d→c→e. This is generating three paths, not two. But the original snippet's problem likely asks for three paths? Wait the snippet is from a known Codeforces problem "Three Paths" or something? Actually the snippet appears to be from Codeforces problem 1774E? Not sure. The snippet prints YES then three lines. The task description I wrote originally says "two vertex-disjoint paths", but the snippet actually finds a structure that gives three paths? Let's re-examine: The snippet's purpose is to find if there exist three vertex-disjoint paths connecting certain pairs? Actually I recall a problem: Given an undirected graph, determine if there are two vertex-disjoint cycles. That's different. Here the snippet: it stores cycles and when two cycles share a vertex, it prints a decomposition into three paths. This is reminiscent of a problem "Two Paths" (CF 14D?) No. Actually the original snippet is from Codeforces Round 1730D? Let me think. The code prints "YES" and three lines, each line a path. That suggests the original problem might ask for three vertex-disjoint paths between some pairs? However the task I wrote says two paths. To stay consistent with the snippet, I need to design a task that matches the output format of the snippet. The snippet outputs exactly three lines after YES: line1 is a path from d to e (reversed, so actually e→d reversed is d→e? Wait ppath(e,d) gives path e...d, reverse gives d...e, so line1 is d→e. Line2 is d→b followed by a→e (that's two segments concatenated, but note that b is ancestor of a, and e is descendant of b? Actually e=LCA(a,c) is descendant of both b and d? Because b and d are ancestors of u (the shared node), and e is a descendant of u (or equal), so e is descendant of b and d. So line2 is: first go from d up to b (since d is ancestor of u, and b is another ancestor? Wait d and b are both ancestors of u, but not necessarily comparable. The path d→b goes up from d to b (since b is ancestor of d? Actually in the triangle example, d=1, b=1, so d=b, trivial. In general, d and b could be different ancestors. The path from d to b walks up the tree from d to b, but if b is not an ancestor of d, that path would go up through LCA? The ppath function simply walks up from x to y by repeatedly moving to parent, assuming y is an ancestor of x. So it requires that d is a descendant of b? But in our situation, both d and b are ancestors of u, but one could be above the other. The code does not swap; it assumes b is an ancestor of d. Is that guaranteed? In the triangle example, both are 1, okay. In general, when we stored the first cycle (a,b) on a node u, b is an ancestor of u (the back edge's ancestor endpoint). When we find a new cycle (c,d), d is an ancestor of u as well. So both b and d are ancestors of u, so they are on the same root-to-u path, thus one is an ancestor of the other. So ppath(d,b) works if b is an ancestor of d (i.e., b is above d). If d is above b, then ppath(d,b) would go from d up to b? That would be a path from d up past its parent until reaching b, but b is deeper than d? Actually if d is above b, then b is a descendant of d. The function ppath(d,b) expects b to be an ancestor of d, but here b is a descendant of d, so it would fail (walking up from d would never reach b). However, in the code, before calling print, they do `if(dep[b]>dep[d]) swap(a,c),swap(b,d);` So they ensure that b's depth is not greater than d's depth, i.e., b is not deeper than d, so b is an ancestor of d (or equal). So after the swap, b is an ancestor of d (or equal). So ppath(d,b) is valid: d is descendant of b. Similarly, a is descendant of e? e=LCA(a,c) is ancestor of both a and c. So ppath(a,e) gives a→e. Then line2 is: path d→b (going up from d to b) then immediately path a→e (going up from a to e). But these two segments are concatenated; the last vertex of the first segment is b, and the first vertex of the second segment is a. For this concatenation to form a simple path, we need b == a? That would be a cycle with b=a? Actually not. In the triangle example, b=1, a=3, so after d→b (1→1) we have [1], then a→e (3→2) gives [3,2], so concatenation [1,3,2] is a path from 1 to 2? But it goes from 1 to 3, which is an edge (since 1-3 edge exists). This is fine because we are not required to follow tree edges; the graph has non-tree edges. So the concatenated path uses the back edge (a,d) or something? In our example, the edge 1-3 is a back edge from the first cycle. The path [1,3,2] uses edge 1-3 (which is a back edge) and tree edge 3-2. So that's a valid simple path. Similarly line3: [1,4,2] uses edge 1-4 and tree edge 4-2. So we output three paths: line1 [1,2] (tree edge), line2 [1,3,2], line3 [1,4,2]. These three paths are not vertex-disjoint; they all share 1 and 2. So the original problem likely asks for "three paths that are internally vertex-disjoint" (i.e., the middle vertices are distinct)? Actually line1 [1,2] shares endpoints with line2 and line3. That suggests the problem is about finding two cycles that share a path? I think the original problem is: Given an undirected graph, determine if there exist two vertex-disjoint cycles? No, that would output two cycles. Here it outputs three paths, suggesting the problem is to find "a pair of vertex-disjoint paths between two pairs" but the construction yields three paths, and the third is redundant? Actually the code prints three lines, so the problem statement probably asks for three paths. Let me identify the exact problem: This is Codeforces Round 1774E? No, I recall a problem "Three Paths" (CF 1761E? Not sure). Actually the code appears to be from CF problem "Two Cycles" or "Finding Cycles". However, to make a self-contained task, I can modify the task to ask for three vertex-disjoint (internally) paths with specified endpoints? The snippet outputs three paths: line1 from d to e, line2 from d (via b) to e (via a), line3 from d (via c) to e? Actually let's interpret: Given two cycles that share a vertex, we can extract three vertex-disjoint paths between two "base" vertices. In the reference, d and e are the two base vertices? Line1: d→e (tree path). Line2: d→b (up tree) then a→e (up tree) but a is connected to d via back edge? Actually the path line2 goes from d up to b, then jumps to a (using the back edge between a and b? Because a and b are endpoints of the first cycle, so there is an edge between a and b? Wait the first cycle consists of tree path from a up to b plus the back edge (a,b). So the back edge directly connects a and b. So after reaching b from d (tree path), we can take the back edge from b to a, then tree path from a to e. So line2 is actually a simple path from d to e that uses the back edge (b,a). Similarly line3 uses the back edge (d,c) from the second cycle. So the three paths are: two "side" paths that each use one back edge, and one "middle" tree path. These three paths are internally vertex-disjoint (except at endpoints d and e) because they go through different branches of the tree. Indeed, in the triangle example, the tree path from d=1 to e=2 is [1,2] (via node 2, but actually 1's child is 2? The tree edges: 1-2 is a tree edge, so path 1-2 is direct). The side path using back edge (1,3) goes from 1 to 3 (back edge) then 3 to 2 (tree edge) – internally vertex 3, distinct from the middle path's internal vertices (none). The other side path uses back edge (1,4) internally 4. So indeed, three vertex-disjoint (except endpoints) paths between d and e. The original problem is likely: Given an undirected graph, decide if there exist two vertices (let's call them s and t) such that there are three internally vertex-disjoint paths between them (i.e., the graph contains a subdivision of K_{2,3} or a theta graph). This is a known problem: checking if the graph contains a "theta" subgraph (three internally vertex-disjoint paths between two vertices). The snippet solves exactly that. So the task should be formulated as: Given an undirected graph, determine if there exist two distinct vertices s and t such that there are three pairwise internally vertex-disjoint paths between s and t (a theta subgraph). If yes, output "YES" and print three such paths (each as a sequence of vertices), each path starting at s and ending at t. If no, output "NO". The function must handle up to 200,000 vertices and 400,000 edges. The reference solution uses DFS and cycle detection as described. So I will design the task accordingly.
//
// Time complexity: O(N+M) because each edge is visited once, and the path extraction is linear. Space O(N).
//
// Edge cases: Graph may be disconnected; handle each component. If no theta subgraph, print NO. If the graph has a self-loop or multiple edges? The problem says simple graph, but we can ignore self-loops? The algorithm's DFS assumes no parallel edges between same pair? Actually parallel edges would create 2-cycles; but we assume simple.
//
// Now for the solution: I need to provide a function that takes n, m, and edge list, and prints the result. The function must be self-contained, with no main. I'll implement similar to the snippet but with clean code and descriptive names. I'll include necessary headers. The solution will use iterative DFS? The snippet uses recursion; but for 200k vertices, recursion depth could be up to 200k causing stack overflow. So we need to implement iterative DFS or increase stack size? The snippet uses recursion and might pass on Codeforces because they set stack size large? Usually C++ recursion depth of 200k can segfault. To be safe, I'll implement an iterative DFS using an explicit stack. However, the cycle detection needs to process back edges in post-order. I'll design an iterative DFS that simulates recursion with a stack of pairs (node, iterator index). Alternatively, I can use a vector-based DFS that processes back edges when they are encountered, but the original algorithm relies on the order: when a back edge from descendant to ancestor is found, it registers the cycle on the tree path. That can be done iteratively: when exploring node x, for each neighbor y, if y is not visited, set parent and recurse; after returning, y is marked done. If y is visited and y is an ancestor (i.e., not done), we have a back edge. The registration of the cycle path from x up to y can be done by walking up via parent pointers. This is fine as long as we have the depth and parent arrays. We need to know which nodes are currently on the recursion stack (in the current DFS path). We can maintain a state: 0=unvisited, 1=in stack, 2=done. Iterative DFS: use stack of pairs (node, next edge index). Push the first node with state 1. While stack not empty, take top. If all edges processed, mark state=2, pop. Otherwise process next edge. If neighbor is unvisited, set parent, depth, state=1, push. If neighbor is in stack (state==1), it's a back edge (y is ancestor of x), and we process as in the recursive version. This is doable. However, the original recursive code processes back edges when exploring x, before exploring y? Actually in the recursive version, when at node x, for each neighbor y, first check if y is parent; skip. Then if !v[y], recurse; after recursion, continue. Else if is[y] (meaning y is in the current recursion stack) then it's a back edge to an ancestor, and we process the cycle. So the order is: when we see a visited neighbor that is in stack, we process the back edge immediately (not after recursion). In iterative DFS, we can do the same: when processing edge from x to y, if y is in stack (state==1) and y is not parent, process back edge. That's fine.
//
// Now, the path extraction: We need to store for each node the first cycle (a,b) that passes through it. When a new back edge (x,y) is found, we walk from x up to y (via parent links) and for each node u on that path, if fx[u] is nonzero, then we have a conflict. We call print(fx[u], fy[u], x, y) and exit. Otherwise we set fx[u]=x, fy[u]=y. Note that the same cycle might be stored on many nodes; that's fine.
//
// The print function: It receives a,b,c,d (where a and c are descendants, b and d are ancestors). It ensures b is ancestor of d (or equal) by swapping if needed. Then computes e = LCA(a,c). Then prints three paths from d to e: path1 is the tree path from d to e (reversed to e→d? Actually the snippet prints: ppath(e,d) then reverse, so that's d→e. So path1 is d→e (tree path). Path2: ppath(d,b) (gives d up to b) then ppath(a,e) (gives a up to e) concatenated, but note that b and a are connected by a back edge, so the combined path is d→...→b -backedge- a→...→e. Path3: st[++tp]=d (i.e., start with d) then ppath(c,e) (gives c up to e), so path3 is d -backedge- c→...→e (since d and c are connected by back edge). So these three paths are internally vertex-disjoint except at d and e. We need to output them in order. The snippet flushes three lines. The function must print exactly that. In the solution, I will implement printPath to construct a vector of vertices and print it. The function must output to standard output.
//
// Edge cases: If no conflict found, print "NO". Also, we need to handle the case where the graph has only one component? The algorithm runs DFS from each unvisited vertex. For a theta subgraph, all three paths must be in the same connected component, so it's fine to detect within a component.
//
// One subtlety: The registration of a cycle on the path from x to y may overwrite an earlier cycle? The original code does: `if(fx[u]) print(...); else fx[u]=x, fy[u]=y;` so it only stores the first cycle on each node. If a node already has a cycle, and we find another cycle passing through it, we immediately trigger the print. That's correct.
//
// Another subtlety: The LCA function in the snippet is naive: `for(;x^y;x=fa[x]) if(dep[x]<dep[y]) swap(x,y);` That's O(depth) each call, but it's called only once (when we find the conflict), so O(N) worst-case, acceptable. We'll implement a simple LCA by bringing the deeper node up.
//
// Now, for a self-contained task: I'll write the problem statement as: "Given an undirected connected graph (or possibly disconnected) with N vertices and M edges, write a function that determines whether there exist two distinct vertices s and t such that there are three internally vertex-disjoint paths between s and t (i.e., the graph contains a theta subgraph). If yes, print YES and then three lines, each containing a simple path from s to t (vertices separated by spaces). If no, print NO. The function must run in O(N+M) time and O(N) memory."
//
// I'll also include that vertices are 1-indexed, edges are given as pairs, and the graph is simple. The function signature: `void solve(int n, int m, const vector<pair<int,int>>& edges)`.
//
// Now the solution: I'll write a function that constructs adjacency list, performs iterative DFS as described, and prints result. I'll include necessary headers: bits/stdc++.h or iostream, vector, algorithm, etc. I'll write it as a free function.
//
// Let me design the iterative DFS carefully.
//
// We have arrays:
// - vector<vector<int>> adj (size n+1)
// - vector<int> parent (0), depth (0), state (0 = unvisited, 1 = in stack, 2 = done)
// - vector<int> fx(n+1,0), fy(n+1,0) // store first cycle endpoints per node
// - We'll also have a vector of ints `st` for printing path, but we'll handle printing via a helper.
//
// Iterative DFS:
// For each start node s from 1..n, if state[s]==0:
//   state[s]=1, depth[s]=1, parent[s]=0
//   stack of pairs: (node, index of next edge). Use vector<pair<int,int>> stack; push {s,0}.
//   while stack not empty:
//     auto &[x, idx] = stack.back();
//     if idx < adj[x].size():
//       y = adj[x][idx]; ++idx;
//       if (y == parent[x]) continue;
//       if (state[y]==0) {
//         parent[y]=x; depth[y]=depth[x]+1; state[y]=1; stack.push_back({y,0});
//       } else if (state[y]==1) {
//         // back edge from x to ancestor y
//         // walk from x up to y
//         int u = x;
//         while (u != y) {
//           if (fx[u]) {
//             // found conflict, print and exit
//             printResult(fx[u], fy[u], x, y);
//             return; // but we need to exit entire function
//           } else {
//             fx[u]=x; fy[u]=y;
//           }
//           u = parent[u];
//         }
//         // also check u==y? Actually we haven't checked y itself because while(u!=y) stops when u==y. Should we also check y? In the original code, the loop `for(int u=x;u^y;u=fa[u])` includes both x and y? It starts at u=x and checks u^y, then after body, u=fa[u]. So it includes x, then parent of x, ..., up to but not including y? Because when u becomes y, the condition u^y is false, so it stops before processing y. So it does NOT process y. That's fine because the cycle is (x,y) and the shared node must be strictly between x and y? Actually it could be y itself? If y has a stored cycle, that would also be a conflict, but the original code doesn't check y. However, consider a graph where two cycles share the ancestor y only. In that case, when the second cycle's back edge goes from x to y, the path from x up to y includes y as the endpoint, but the loop stops before y, so it wouldn't detect the conflict at y. Is that a problem? Let's think: Suppose we have two cycles: first cycle (a,b) where b is ancestor of a, and second cycle (c,d) where d is ancestor of c, and both cycles share exactly one vertex which is b=d=? Actually if b==d and that's the only shared vertex, then the two cycles share a single vertex, which is enough to form a theta? Let's see: two cycles sharing a single vertex. Can we extract three internally disjoint paths between two vertices? For example, two triangles sharing only one vertex: vertices 1,2,3 and 1,4,5. The graph has two triangles sharing vertex 1. There are three paths between 1 and 2? Actually to have three internally vertex-disjoint paths between some s and t, we need a theta graph. Two cycles sharing a single vertex give a figure-eight, which does not contain a theta (because any two paths would share that vertex). So we need at least two distinct shared vertices, or the cycles share a path. In the algorithm, when two cycles share a vertex u that is not an endpoint of either back edge, we detect it because u lies strictly between x and y (unless u is y? If u is y, then both cycles have the same ancestor endpoint. But then they share a path? For example, triangle 1-2-3-1 and another triangle 1-2-4-1 share the edge 1-2. Here the two cycles share two vertices 1 and 2. When processing the second back edge, say from 4 to 1, the path from 4 up to 1 includes 4,2,1. The node 2 is strictly between, so we catch it. If they share only the ancestor y, e.g., two cycles that share only the root? Consider a star with three leaves connected in a cycle? Actually for a theta, you need at least two common vertices. So it's fine to not check y. But what if the two cycles share the ancestor y and also another node? Then the other node is strictly between, so we catch it. So it's safe. We'll follow the original code exactly.)
//
// After processing all back edges and recursion, set state[x]=2 and pop.
//
// However, there's a subtlety: In the iterative version, when we process a back edge, we might be modifying fx/fy while we are still in the middle of DFS. That's fine.
//
// Now the printResult function: It takes a, b, c, d (all integers). It must print "YES" and three lines. We'll implement it as a lambda inside solve? But the task requires a free function that does everything. I'll encapsulate the result printing in a helper function that takes a,b,c,d and the parent/depth arrays. However, since it's called once and then we exit, I'll write a separate function `outputResult` that uses global arrays? But for a standalone function, we can pass by reference. To keep it simple, I'll write the solve function with all needed arrays defined inside, and a lambda for printing that captures by reference.
//
// But the instructions for the solution say: "Write a high-quality, self-contained C++ implementation with a descriptively named free function that matches the task specification. Include necessary headers and concise comments describing the function, and apply appropriate const correctness. Output code only." So I'll write a function `void solve(int n, int m, const vector<pair<int,int>>& edges)` that does everything. I'll define a helper function outside? But it's fine to have a helper function that takes the arrays by reference inside an anonymous namespace or as static. Since the solution must be self-contained and no main, I'll put everything in one function, but that might be long. I'll define a struct or use lambdas.
//
// Better: I'll define a class Solver with member arrays and methods, but the instruction says "free function". So I'll write a free function `void solve(int n, int m, const vector<pair<int,int>>& edges)` and inside it define a lambda for printing. That's acceptable.
//
// Let me write the code:
//
// ```cpp
// #include <bits/stdc++.h>
// using namespace std;
//
// void solve(int n, int m, const vector<pair<int,int>>& edges) {
//     vector<vector<int>> adj(n+1);
//     for (auto &e : edges) {
//         adj[e.first].push_back(e.second);
//         adj[e.second].push_back(e.first);
//     }
//     vector<int> parent(n+1, 0), depth(n+1, 0), state(n+1, 0); // 0 unvisited, 1 in stack, 2 done
//     vector<int> fx(n+1, 0), fy(n+1, 0); // first cycle endpoints for each node
//     bool found = false;
//
//     // Lambda to print the three paths
//     auto printResult = [&](int a, int b, int c, int d) {
//         // Ensure b is ancestor of d (not deeper)
//         if (depth[b] > depth[d]) {
//             swap(a, c);
//             swap(b, d);
//         }
//         // Compute LCA of a and c
//         int x = a, y = c;
//         while (x != y) {
//             if (depth[x] < depth[y]) swap(x, y);
//             x = parent[x];
//         }
//         int e = x;
//         // Helper to print a path from u to v (v is ancestor of u) in order from u to v
//         auto printPath = [&](int u, int v) {
//             vector<int> path;
//             while (u != v) {
//                 path.push_back(u);
//                 u = parent[u];
//             }
//             path.push_back(v);
//             for (size_t i = 0; i < path.size(); ++i) {
//                 if (i) cout << ' ';
//                 cout << path[i];
//             }
//         };
//         // Path 1: d -> e (tree path), but need to go up from d to e. Since e is ancestor of d? Actually e is LCA of a and c, and a is descendant of b, c is descendant of d. Both b and d are ancestors of the shared node, but e is descendant of both? Let's verify: e is LCA of a and c. Since a is on path from x to b, and c is on path from y to d, and both b,d are ancestors of the shared node u? Actually u is on both paths, and e is the LCA of a and c, so e is a descendant of u (or equal). So e is ancestor of a and c, but not necessarily ancestor of d. In the snippet, they printed ppath(e,d) then reverse, meaning they construct path from e up to d? But e may not be ancestor of d? Actually in the triangle example, e=2, d=1, and e is descendant of d, so ppath(e,d) goes from e up to d (since d is ancestor of e) and then reverse to get d→e. So we need to print path from d to e, where e is a descendant of d? In general, e is LCA(a,c), and a is descendant of b, c is descendant of d. Since b and d are ancestors of the shared node u, and e is ancestor of a and c, but is e necessarily descendant of both b and d? Yes, because b is ancestor of u, and u is ancestor of e (since e is descendant of u because u is on path from a to b and also on path from c to d, and e is the LCA of a and c, which is on both tree paths, so e must be a descendant of u as well? Actually u is on both tree paths, so u is a common ancestor of a and c, so e (their LCA) is a descendant of u (or equal). Therefore e is descendant of b and d (since b,d are ancestors of u). So d is an ancestor of e? Not necessarily: d is ancestor of u, and e is descendant of u, so d is ancestor of e (since ancestor relation is transitive). So d is ancestor of e. Similarly b is ancestor of e. So ppath(e,d) goes from e up to d (since d is ancestor of e), and reversing gives d→e. So path1 is d to e via tree edges. We can print that with a helper that takes d (descendant? Actually d is ancestor of e, so to print from d to e, we need to go down? But we don't have child pointers. Better to print the reverse of the path from e up to d. So we collect path from e up to d, then reverse.
//         auto printUp = [&](int u, int v) {
//             // prints path from u up to v (v ancestor of u)
//             vector<int> p;
//             while (u != v) {
//                 p.push_back(u);
//                 u = parent[u];
//             }
//             p.push_back(v);
//             for (size_t i = 0; i < p.size(); ++i) {
//                 if (i) cout << ' '; cout << p[i];
//             }
//         };
//         // Path1: from d to e. Since e is descendant of d, we print reverse of e->d.
//         vector<int> p1;
//         {
//             int u = e;
//             while (u != d) {
//                 p1.push_back(u);
//                 u = parent[u];
//             }
//             p1.push_back(d);
//             reverse(p1.begin(), p1.end());
//             for (size_t i = 0; i < p1.size(); ++i) {
//                 if (i) cout << ' '; cout << p1[i];
//             }
//             cout << '\n';
//         }
//         // Path2: d -> b (up tree) then use back edge (b,a) then a -> e (up tree)
//         // This is a simple path: d up to b, then back edge to a, then up to e.
//         {
//             vector<int> p2;
//             int u = d;
//             while (u != b) {
//                 p2.push_back(u);
//                 u = parent[u];
//             }
//             p2.push_back(b); // now at b
//             // Now append a then up to e
//             p2.push_back(a);
//             u = a;
//             while (u != e) {
//                 u = parent[u];
//                 p2.push_back(u);
//             }
//             // Note: if a == e, then we would have added a and then e? Actually if a==e, then after pushing a, we need to avoid duplicate. But in our case a is deeper than e (unless a==e), so it's fine.
//             // Validate: p2 is d...b, then a...e. The edge between b and a is a back edge from first cycle. So it's a valid path.
//             for (size_t i = 0; i < p2.size(); ++i) {
//                 if (i) cout << ' '; cout << p2[i];
//             }
//             cout << '\n';
//         }
//         // Path3: d -> back edge to c -> up to e
//         {
//             vector<int> p3;
//             p3.push_back(d);
//             p3.push_back(c);
//             int u = c;
//             while (u != e) {
//                 u = parent[u];
//                 p3.push_back(u);
//             }
//             for (size_t i = 0; i < p3.size(); ++i) {
//                 if (i) cout << ' '; cout << p3[i];
//             }
//             cout << '\n';
//         }
//         found = true;
//     };
//
//     // Iterative DFS for each component
//     for (int start = 1; start <= n && !found; ++start) {
//         if (state[start]) continue;
//         state[start] = 1;
//         depth[start] = 1;
//         vector<pair<int,int>> stk; // node, next edge index
//         stk.push_back({start, 0});
//         while (!stk.empty() && !found) {
//             int x = stk.back().first;
//             int &idx = stk.back().second;
//             if (idx < (int)adj[x].size()) {
//                 int y = adj[x][idx++];
//                 if (y == parent[x]) continue;
//                 if (state[y] == 0) {
//                     parent[y] = x;
//                     depth[y] = depth[x] + 1;
//                     state[y] = 1;
//                     stk.push_back({y, 0});
//                 } else if (state[y] == 1) {
//                     // back edge x->y (y ancestor)
//                     int u = x;
//                     while (u != y) {
//                         if (fx[u]) {
//                             printResult(fx[u], fy[u], x, y);
//                             found = true;
//                             break;
//                         } else {
//                             fx[u] = x;
//                             fy[u] = y;
//                         }
//                         u = parent[u];
//                     }
//                 }
//             } else {
//                 state[x] = 2;
//                 stk.pop_back();
//             }
//         }
//     }
//     if (!found) cout << "NO\n";
// }
// ```
//
// But I need to verify that the path2 construction is correct: p2 starts at d, goes up to b (ancestor). Then we push a (which is descendant of b). But there is no edge between b and a directly; the back edge is between a and b? Actually in the first cycle, a and b are endpoints of a back edge, so there is a graph edge between a and b. So from b we can go to a via that edge. But our path representation just lists vertices; we don't need to show edges, just consecutive vertices must be adjacent in the graph. Since a and b are adjacent by the back edge, it's fine. Then from a we go up to e via tree edges. So p2 is a valid simple path (provided the tree path from a to e does not intersect the path from d to b except at e? Actually a's ancestor path to e goes through the shared node u and beyond to e, which is also on the path from d to b? The path from d to b goes up from d to b, and b is an ancestor of d. The path from a to e goes up from a to e, and e is a descendant of b. They share only the vertex e? Let's see: in the triangle example, d=1, b=1, a=3, e=2. Path from d to b is just [1]. Then a to e is [3,2]. So p2=[1,3,2]. That's fine. In a more complex case, suppose b is not equal to d. Then path d→b goes up through some nodes. Path a→e goes up from a (which is a descendant of b) to e (which is a descendant of b as well). Since b is ancestor of both, these two paths share only b? Actually they start at different descendants of b and meet at b? But after we go from d to b, we then jump to a (via back edge), then go up to e. The path from a to e goes through some nodes that may include the shared node u and beyond. The path from d to b goes through nodes that are on the path from d to b, which is a subtree of b. The path from a to e is within the subtree of b as well; they share only the vertex b? Not necessarily, they could share more if a and d are in the same branch. But the construction is designed so that the three paths are internally vertex-disjoint. The original code ensures that because the two cycles share a node u, and the three paths use different areas: path1 uses the tree path from d to e (which goes through u), path2 uses the back edge of the first cycle (a-b) and the tree path from a to e (which goes through u and then down to e? Actually a is on one side of u, c is on another side, and the tree path from d to e goes through u and then down to e? Let's trust the original algorithm; it's a known correct construction from a problem solution. Our implementation should mirror it exactly.
//
// I notice that in the original code, path2 is printed as `ppath(d,b), ppath(a,e)` and then flush. This gives a single line: concatenation of two sequences. Our path2 does exactly that. But note: the original code does not check for duplicates if the two sequences share vertices other than possibly endpoints. It assumes the construction is correct. We'll copy that.
//
// Also, path3: starts with d, then c (via back edge), then up to e. That's exactly `st[++tp]=d, ppath(c,e)`.
//
// One more thing: In the original printResult, after printing path2, it does `st[++tp]=d, ppath(c,e), flush(), exit(0);`. So path3 is d then c then up to e. Our code does that.
//
// Now, there is a potential issue with iterative DFS: when we find a back edge and call printResult, we set found=true and break out of the loops. But we still have a stack; we just exit the function after printing. That's fine.
//
// Also, we need to ensure that the `printResult` lambda captures arrays by reference. It uses depth, parent, etc.
//
// Edge case: if `a == e` or similar, the path might have duplicate vertices. The original code would produce a path of length 1 etc. We'll handle it naturally.
//
// Now, regarding the problem statement: The task asks for a function that prints YES and three paths if a theta subgraph exists. The function signature and behavior should match this.
//
// I need to write the

must be a free function with no main. The

will provide a main that calls the function and checks output? But the function prints to stdout; for testing, we need to capture output. The test code can use a helper that runs the function and compares the output string. However, the instruction for
