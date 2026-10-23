# Changelog

All notable changes to this project are documented in this file.
Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

---

## [Unreleased]

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
