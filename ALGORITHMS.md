# Algorithm Reference Guide

> Beginner-friendly explanations of all six graph algorithms implemented
> in this DSA project, with pseudocode, examples, and complexity analysis.

---

## Table of Contents

1. [BFS — Breadth-First Search](#1-bfs--breadth-first-search)
2. [DFS — Depth-First Search](#2-dfs--depth-first-search)
3. [Dijkstra's Shortest Path](#3-dijkstras-shortest-path)
4. [Prim's MST](#4-prims-mst)
5. [Kruskal's MST](#5-kruskals-mst)
6. [Floyd-Warshall All-Pairs Shortest Path](#6-floyd-warshall-all-pairs-shortest-path)
7. [Complexity Summary Table](#7-complexity-summary-table)

---

## 1. BFS — Breadth-First Search

### What problem does it solve?
Given a starting vertex, visit every reachable vertex in the graph.
BFS processes vertices **level by level** — first all neighbours of the
start, then their neighbours, and so on.

### Real-world analogy
Imagine you are at "Machine Learning" and want to find all related topics.
BFS first finds all topics directly connected to ML (Deep Learning, NLP,
Computer Vision, Recommendation Systems), then expands outward.

### Pseudocode
```
BFS(start):
  mark start as visited
  enqueue start
  while queue is not empty:
    current = dequeue
    print current
    for each neighbour of current:
      if neighbour is not visited:
        mark neighbour as visited
        enqueue neighbour
```

### Key Data Structure
**Queue** (FIFO — First In, First Out). In this project implemented as
a simple array with `front` and `rear` pointers.

### Why does it give level-order traversal?
Because a queue processes vertices in the order they were discovered.
Vertices at distance 1 from the source are enqueued before vertices
at distance 2, so they are processed first.

### Complexity
| | Value |
|--|--|
| **Time** | O(V²) with adjacency matrix |
| **Space** | O(V) — visited array + queue |

---

## 2. DFS — Depth-First Search

### What problem does it solve?
Like BFS, it visits every reachable vertex. But DFS goes **as deep as
possible** along one path before backtracking.

### Real-world analogy
Like exploring a maze: keep going straight until you hit a dead end,
then backtrack to the last junction and try another path.

### Pseudocode (recursive)
```
DFS(current):
  mark current as visited
  print current
  for each neighbour of current:
    if neighbour is not visited:
      DFS(neighbour)
```

### Key mechanism
**Call stack** (recursion). When DFS calls itself, the call is pushed
onto the system stack. When there are no more unvisited neighbours,
the function returns (pops the stack) — this is the "backtrack".

### BFS vs DFS comparison

| Property | BFS | DFS |
|----------|-----|-----|
| Data structure | Queue | Stack / Recursion |
| Order | Level by level | Deep first |
| Good for | Shortest path (unweighted) | Cycle detection, topology |
| Memory | More (stores all level nodes) | Less (only current path) |

### Complexity
| | Value |
|--|--|
| **Time** | O(V²) with adjacency matrix |
| **Space** | O(V) — visited array + call stack |

---

## 3. Dijkstra's Shortest Path

### What problem does it solve?
Finds the **minimum cost path** from one source vertex to a chosen
destination vertex. Requires all edge weights to be **positive**.

### Core idea — Relaxation
"Relaxing" an edge (u → v) means:  
*If going through u gives a shorter path to v than what we currently know,
update the shortest known distance to v.*

```
if dist[u] + weight(u,v) < dist[v]:
    dist[v] = dist[u] + weight(u,v)
    parent[v] = u
```

### Pseudocode (array-based, O(V²))
```
Dijkstra(source, destination):
  dist[source] = 0; dist[all others] = INF
  visited[all] = false

  repeat V times:
    u = unvisited vertex with smallest dist[u]
    mark u as visited
    for each neighbour v of u:
      relax edge (u, v)

  trace parent[] array from destination to source to get path
```

### Path reconstruction
After the algorithm, `parent[dest]` tells us which vertex comes before
`dest` on the shortest path. We follow `parent[parent[dest]]` and so on
until we reach the source. The path is then printed in reverse.

### Why doesn't it work with negative weights?
Dijkstra assumes that once a vertex is marked visited, its distance is
final. A negative edge to an already-visited vertex could make a shorter
path that Dijkstra would miss. Use **Bellman-Ford** for negative weights.

### Complexity (this implementation)
| | Value |
|--|--|
| **Time** | O(V²) — two nested loops over V vertices |
| **Space** | O(V) — dist, visited, parent arrays |

A heap-based Dijkstra runs in O((V+E) log V) but is more complex.

---

## 4. Prim's MST

### What problem does it solve?
Finds a **Minimum Spanning Tree (MST)** — a subset of edges that:
- Connects all V vertices
- Has no cycles
- Has the minimum possible total edge weight

An MST always has exactly **V − 1** edges.

### Core idea
Grow the MST one vertex at a time. At each step, pick the vertex NOT
yet in the MST that can be attached via the **cheapest available edge**.

### Key arrays
| Array | Meaning |
|-------|---------|
| `key[v]` | Cheapest edge weight that can connect v to the current MST |
| `parent[v]` | Which MST vertex offers that cheap edge to v |
| `inMST[v]` | Whether v has been added to the MST |

### Pseudocode
```
Prim(start = 0):
  key[0] = 0; key[all others] = INF
  parent[all] = -1

  repeat V times:
    u = vertex not in MST with smallest key[u]
    add u to MST; record edge (parent[u], u)
    for each neighbour v of u:
      if graph[u][v] < key[v]:
        key[v] = graph[u][v]
        parent[v] = u
```

### Complexity
| | Value |
|--|--|
| **Time** | O(V²) — two nested loops |
| **Space** | O(V) — key, parent, inMST arrays |

---

## 5. Kruskal's MST

### What problem does it solve?
Also finds an MST, but uses a **completely different approach**:
sort all edges by weight and greedily pick the cheapest edge that
does not create a cycle.

### Union-Find (Disjoint Set Union — DSU)
The key supporting data structure. Maintains a collection of disjoint
sets. Two vertices are in the same set if they are already connected
in the partial MST.

```
find(x):              // returns root/representative of x's set
  if parent[x] != x:
    parent[x] = find(parent[x])  // path compression
  return parent[x]

union(a, b):          // merges sets of a and b
  root_a = find(a)
  root_b = find(b)
  if root_a != root_b:
    parent[root_a] = root_b
```

**Path compression** flattens the tree so future `find()` calls are fast.

### Pseudocode
```
Kruskal():
  extract all edges from adjacency matrix
  sort edges by ascending weight (bubble sort here)
  initialise Union-Find: each vertex is its own set

  for each edge (u, v, w) in sorted order:
    if find(u) != find(v):  // different sets → no cycle
      add edge to MST
      union(u, v)
      if MST has V-1 edges: stop
```

### Why bubble sort here?
This is a beginner project, so bubble sort is used for readability.
Its O(E²) cost dominates the O(E · α(V)) union-find cost.
In practice, use `qsort` (O(E log E)) for better performance.

### Prim vs Kruskal comparison

| Property | Prim | Kruskal |
|----------|------|---------|
| Approach | Vertex-by-vertex | Edge-by-edge |
| Starts from | A specific vertex | All edges (sorted) |
| Data structure | key/parent arrays | Union-Find |
| Time (this impl.) | O(V²) | O(E²) |
| Best for | Dense graphs | Sparse graphs |
| MST cost result | Same ✅ | Same ✅ |

### Complexity (this implementation)
| | Value |
|--|--|
| **Time** | O(E²) — bubble sort dominates |
| **Space** | O(E + V) — edge array + Union-Find parent array |

---

## 6. Floyd-Warshall All-Pairs Shortest Path

### What problem does it solve?
Dijkstra finds the shortest path **from one source** to all others.
Floyd-Warshall finds the shortest path **between every pair** of vertices
in a single O(V³) pass.

### Core idea — Dynamic Programming
For each intermediate vertex `k`, check if routing through `k` gives a
shorter path between every pair `(i, j)`:

```
for k = 0 to V-1:
  for i = 0 to V-1:
    for j = 0 to V-1:
      if dist[i][k] + dist[k][j] < dist[i][j]:
        dist[i][j] = dist[i][k] + dist[k][j]
```

After the outer loop finishes for all values of `k`, `dist[i][j]` holds
the shortest path between every pair `(i, j)`.

### Initialisation
```
dist[i][i] = 0                    // distance to self
dist[i][j] = graph[i][j]          // direct edge if it exists
dist[i][j] = INF                  // no direct edge
```

### Cross-check with Dijkstra
Floyd-Warshall row `i`, column `j` must equal what Dijkstra reports
for source `i` and destination `j`.

In this project:
- `dist[0][4]` (ML → Medical AI) = **6** ✅ matches Dijkstra

### Complexity
| | Value |
|--|--|
| **Time** | O(V³) — three nested loops |
| **Space** | O(V²) — the V×V distance matrix |
| **For V=8** | 8³ = 512 iterations; runs instantly |

---

## 7. Complexity Summary Table

| Algorithm | Time Complexity | Space | What it finds |
|-----------|----------------|-------|--------------|
| BFS | O(V²) | O(V) | All reachable vertices, level order |
| DFS | O(V²) | O(V) | All reachable vertices, depth first |
| Dijkstra | O(V²) | O(V) | Shortest path, one source to one dest |
| Prim | O(V²) | O(V) | Minimum Spanning Tree (vertex-by-vertex) |
| Kruskal | O(E²) | O(E+V) | Minimum Spanning Tree (edge-by-edge) |
| Floyd-Warshall | O(V³) | O(V²) | All-pairs shortest paths |

> **Note:** All complexities above assume an adjacency matrix representation.
> With adjacency lists + priority queues, Dijkstra and Prim improve to
> O((V+E) log V), and Kruskal improves to O(E log E).
