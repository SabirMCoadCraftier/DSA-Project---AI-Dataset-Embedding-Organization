# Changelog

All notable changes to this project are documented in this file.
Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

---

## [Unreleased]

---

## [1.1.0] — 2026-10-24

### Added
- **Floyd-Warshall all-pairs shortest path** as menu option 9.
  Computes shortest distances between every pair of the 8 topics using
  O(V³) dynamic programming. Prints a formatted 8×8 distance table.
  Cross-verified: `dist[0][4]` = 6 matches Dijkstra (ML → Medical AI).
- **Vertex degree statistics** shown after the adjacency matrix edge list
  (option 2). Displays degree (number of neighbours) and total edge-weight
  sum per topic, plus sum of all graph edges.
- **Startup graph connectivity validator** (`check_graph_connected()`):
  BFS-based check that confirms all 8 vertices are reachable before the
  menu appears. Prints `[OK]` or `[WARNING]` accordingly.
- **`ALGORITHMS.md`** — comprehensive beginner reference covering all six
  algorithms with pseudocode, data structure explanations, real-world
  analogies, and a full complexity summary table.
- **`CHANGELOG.md`** — this file; documents project history and planned work.

### Fixed
- **`select_topic()` EOF handling** — previously used `!read_int()` which
  conflated EOF (rc==-1) with bad-format input (rc==0), sometimes printing
  a misleading "invalid input" message on Ctrl+D / Ctrl+Z.  
  Now returns -2 on EOF; all callers use `if (start < 0) break` to handle
  both -1 and -2 cleanly.

### Changed
- **Menu expanded from 8 to 9 options** (0 to 9).
- **`default` case error message** updated from "0 to 8" to "0 to 9".
- **README.md** updated to reflect all new features, algorithm table,
  project structure, and complexity table.

### Verified
- All 5 original algorithms still produce correct results.
- Prim MST = Kruskal MST = **17** ✅
- Dijkstra ML → Medical AI = **6** ✅
- Floyd-Warshall `dist[0][4]` = **6** ✅ (matches Dijkstra)
- Zero compiler warnings with `gcc -std=c11 -Wall -Wextra -pedantic`.

---

## [1.0.0] — 2026-10-24

### Added
- **`main.c`** — Complete C11 console application implementing five graph
  algorithms on 8 AI research topics represented as a weighted, undirected
  adjacency matrix:
  - BFS (Breadth-First Search) with array-based queue
  - DFS (Depth-First Search) using recursion
  - Dijkstra's shortest path (array-based, O(V²))
  - Prim's Minimum Spanning Tree (array-based, O(V²))
  - Kruskal's Minimum Spanning Tree (Union-Find + bubble sort, O(E²))
- **`README.md`** — Full project documentation: problem statement,
  objectives, graph representation, algorithm explanations, time/space
  complexity table, compilation guide, demo walkthrough, limitations,
  and future improvements.
- **`sample_dataset.csv`** — Dataset of 8 AI topic names and sample keywords
  (Machine Learning, Deep Learning, Computer Vision, NLP, Medical AI,
  Robotics, Speech Recognition, Recommendation Systems).
- **`sample_output.txt`** — Representative terminal output showing all
  five algorithms in action, captured from a real run of the program.
- **`.gitignore`** — Excludes compiled binaries (`*.exe`, `a.out`, `*.o`),
  IDE/OS artifacts; preserves all source, documentation, and data files.

### Graph Properties (v1.0.0)
- 8 vertices (AI topics), 15 edges (undirected, all weights distinct)
- All vertices connected (verified by BFS covering all 8 nodes)
- MST total weight: **17** (confirmed by both Prim and Kruskal)
- Shortest path example: Machine Learning → Medical AI = cost **6**
  (via Deep Learning)

### Technical Notes
- Safe input handling via `fgets` + `strtol`; handles non-numeric input,
  out-of-range values, and EOF (Ctrl+D / Ctrl+Z) without infinite loops.
- Compiled and tested with GCC 6.3.0 (MinGW) using:
  `gcc -std=c11 -Wall -Wextra -pedantic main.c -o project`
  Result: **zero warnings, zero errors**.

---

## [Future]

### Planned
- Option 9: Floyd-Warshall all-pairs shortest path display
- Degree statistics shown alongside the adjacency matrix
- Load topics and weights from CSV at runtime
- Adjacency list representation for larger graphs
- Min-heap based Dijkstra / Prim for O((V+E) log V) performance
- DOT-format graph export for Graphviz visualization
