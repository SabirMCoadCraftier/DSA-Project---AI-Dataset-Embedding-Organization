# AI Dataset Embedding Organization

> **A beginner-friendly DSA project in C** — representing 8 AI research topics as a weighted undirected graph and applying **six** classic graph algorithms.

---

## Table of Contents

1. [Introduction](#introduction)  
2. [Problem Statement](#problem-statement)  
3. [Objectives](#objectives)  
4. [Graph Representation](#graph-representation)  
5. [Dataset Topics](#dataset-topics)  
6. [Features](#features)  
7. [Algorithm Explanations](#algorithm-explanations)  
8. [Time & Space Complexity](#time--space-complexity)  
9. [Requirements](#requirements)  
10. [Compilation & Execution](#compilation--execution)  
11. [How to Run the Demo](#how-to-run-the-demo)  
12. [Example Output](#example-output)  
13. [Why Prim and Kruskal Give the Same MST Cost](#why-prim-and-kruskal-give-the-same-mst-cost)  
14. [Limitations](#limitations)  
15. [Future Improvements](#future-improvements)  
16. [Project Structure](#project-structure)  
17. [Team Members](#team-members)  

---

## Introduction

This project models a small collection of **AI research topics** as a **weighted, undirected graph**. Each topic (Machine Learning, Deep Learning, Computer Vision, etc.) is a **vertex**, and a connection between two related topics is an **edge**. The weight of an edge represents an illustrative "connection cost" — a lower weight means the two topics share a closer relationship in this prototype.

Six foundational graph algorithms are implemented in pure C (C11 standard):

| Algorithm | Purpose |
|-----------|---------|
| BFS | Explore all topics reachable from a starting topic, level by level |
| DFS | Explore topics by going as deep as possible before backtracking |
| Dijkstra | Find the minimum-cost path between any two topics |
| Prim | Build a Minimum Spanning Tree (MST) starting from one topic |
| Kruskal | Build a Minimum Spanning Tree (MST) by greedily adding cheapest edges |
| Floyd-Warshall | Find shortest paths between ALL pairs of topics in one pass |

---

## Problem Statement

When working with AI research topics, it is useful to understand their relationships — which topics are closely connected, how to navigate from one topic to another with minimal conceptual distance, and how to find a minimal set of connections that keeps all topics linked.

This project simulates those relationships as a graph and demonstrates how DSA algorithms can answer real questions:
- **BFS / DFS**: Which topics can we reach from "Machine Learning"?  
- **Dijkstra**: What is the cheapest path from "Machine Learning" to "Robotics"?  
- **Prim / Kruskal**: What is the minimal set of connections that keeps all 8 topics reachable from each other?

---

## Objectives

1. Represent a real-world problem (AI topic relationships) using a graph data structure.  
2. Implement BFS, DFS, Dijkstra's algorithm, Prim's algorithm, and Kruskal's algorithm from scratch in C.  
3. Use an adjacency matrix as the graph representation — appropriate for a small, dense, fixed-size graph.  
4. Practice safe input handling and menu-driven console applications in C.  
5. Verify that Prim and Kruskal both find the same MST cost for the same connected graph.

---

## Graph Representation

### Why an Adjacency Matrix?

An **adjacency matrix** is a 2D array `graph[V][V]` where:
- `graph[i][j] > 0` means there is an edge between vertex `i` and vertex `j` with that weight.
- `graph[i][j] == 0` means no direct edge exists.
- The graph is **undirected**, so `graph[i][j] == graph[j][i]` always.
- `graph[i][i] == 0` (no self-loops).

For this project, an adjacency matrix is a good choice because:
- The number of vertices is small and **fixed** (V = 8).
- The matrix is easy to define in code as a 2D array initializer.
- Every algorithm in this project runs in O(V²) time, which is fast enough for 8 vertices.
- The matrix is easy to display on screen for the instructor.

### Vertices (Topics)

| Index | Topic |
|-------|-------|
| 1 (0) | Machine Learning |
| 2 (1) | Deep Learning |
| 3 (2) | Computer Vision |
| 4 (3) | Natural Language Processing |
| 5 (4) | Medical AI |
| 6 (5) | Robotics |
| 7 (6) | Speech Recognition |
| 8 (7) | Recommendation Systems |

### Edges & Weights

The predefined edge weights represent **illustrative connection costs** designed to show how the algorithms work. They are **not** derived from real AI embeddings or any machine learning model. Lower weight = closer conceptual relationship in this prototype.

```
Machine Learning    --[2]--  Deep Learning
Machine Learning    --[4]--  Computer Vision
Machine Learning    --[3]--  Natural Language Processing
Machine Learning    --[5]--  Recommendation Systems
Deep Learning       --[1]--  Computer Vision
Deep Learning       --[3]--  Natural Language Processing
Deep Learning       --[4]--  Medical AI
Deep Learning       --[2]--  Speech Recognition
Computer Vision     --[5]--  Medical AI
Computer Vision     --[3]--  Robotics
Natural Language Processing  --[4]--  Speech Recognition
Natural Language Processing  --[2]--  Recommendation Systems
Medical AI          --[6]--  Robotics
Robotics            --[5]--  Speech Recognition
Speech Recognition  --[3]--  Recommendation Systems
```

Total: **15 edges**, all vertices connected.

---

## Dataset Topics

| # | Topic | Sample Keywords |
|---|-------|----------------|
| 1 | Machine Learning | supervised, unsupervised, regression, classification |
| 2 | Deep Learning | neural networks, backpropagation, CNN, RNN, transformers |
| 3 | Computer Vision | image recognition, object detection, segmentation, OpenCV |
| 4 | Natural Language Processing | tokenization, BERT, GPT, sentiment analysis |
| 5 | Medical AI | disease prediction, medical imaging, drug discovery |
| 6 | Robotics | autonomous navigation, sensors, control systems |
| 7 | Speech Recognition | ASR, phonemes, acoustic models, CTC |
| 8 | Recommendation Systems | collaborative filtering, matrix factorization |

---

## Features

- ✅ Interactive console menu (options 0–9)
- ✅ Display all topics with keywords
- ✅ Display the adjacency matrix and edge list
- ✅ **Vertex degree statistics** shown below the edge list (degree, weight sum per topic)
- ✅ **Startup connectivity check**: BFS-based validator confirms all 8 topics reachable before the menu appears
- ✅ BFS traversal from a user-selected topic
- ✅ DFS traversal from a user-selected topic
- ✅ Dijkstra's shortest path with full path reconstruction
- ✅ Prim's MST with edge-by-edge output
- ✅ Kruskal's MST showing accepted and skipped edges
- ✅ Side-by-side MST cost comparison (Prim vs Kruskal)
- ✅ **Floyd-Warshall** all-pairs shortest path table (option 9)
- ✅ Safe input validation (non-numeric input, out-of-range values, EOF)
- ✅ Zero compiler warnings with `-Wall -Wextra -pedantic`

---

## Algorithm Explanations

### 1. BFS — Breadth-First Search

**What it does:** Starting from a chosen topic, BFS visits all directly connected topics first, then their neighbors, and so on — like ripples spreading outward on water.

**How it works in code:**
1. Mark the starting topic as visited and add it to a queue (implemented as a simple array).
2. While the queue is not empty, take the front item out, print it, and enqueue all its unvisited neighbors.

**Analogy:** Imagine telling a friend about Machine Learning. They tell their closest friends (Deep Learning, NLP, etc.), and those friends tell their friends. BFS models the spread.

---

### 2. DFS — Depth-First Search

**What it does:** Starting from a chosen topic, DFS dives as deep as possible along one path before backtracking to explore another branch.

**How it works in code:**
1. Mark the current topic as visited and print it.
2. Recursively call DFS on the first unvisited neighbor.
3. When no unvisited neighbors remain, return (backtrack) to the previous call.

**Analogy:** Like exploring a maze — you go as far as you can in one direction before turning back.

---

### 3. Dijkstra's Algorithm

**What it does:** Finds the **shortest (minimum cost) path** between a source topic and a destination topic.

**How it works in code (no priority queue — simple arrays):**
1. Set `dist[source] = 0` and `dist[all others] = infinity`.
2. Repeat V times: pick the unvisited vertex with the smallest `dist` value, mark it visited, then "relax" all its neighbors — if going through this vertex offers a shorter path, update the neighbor's distance and record the parent.
3. After V iterations, trace back through the `parent[]` array from destination to source to reconstruct the path.

**Important:** All edge weights must be positive — they are in this project.

---

### 4. Prim's Algorithm (MST)

**What it does:** Builds a **Minimum Spanning Tree** (MST) — the smallest set of edges that connects all 8 topics with the lowest total weight. An MST always has exactly V−1 = 7 edges.

**How it works in code:**
1. Start with any vertex (we use vertex 0 — Machine Learning). Keep a `key[]` array tracking the cheapest known edge to each vertex not yet in the MST.
2. Repeat V−1 times: pick the vertex with the lowest `key` value not yet added to the MST, add it, then update the `key` values of its neighbors if a cheaper connection was found.
3. Print each added edge and accumulate the total weight.

**Analogy:** Growing a tree one branch at a time, always choosing the cheapest available branch.

---

### 5. Kruskal's Algorithm (MST)

**What it does:** Also builds a **Minimum Spanning Tree**, but uses a different greedy approach — sort all edges by weight and keep adding edges as long as they don't create a cycle.

**How it works in code:**
1. Extract all edges from the adjacency matrix into an array. Sort them by ascending weight using **bubble sort**.
2. Use **Union-Find (Disjoint Set Union)**: each topic starts in its own set. Two topics are in the same set if they are already connected in the partial MST.
3. For each edge (cheapest first): if the two endpoints are in **different sets**, add the edge to the MST and merge the sets. If they are in the **same set**, adding the edge would create a cycle — skip it.
4. Stop when V−1 edges are added.

**Analogy:** Choosing the cheapest available road between any two cities, as long as it doesn't create a loop in your road network.

**Union-Find with path compression:** `find(x)` returns the representative (root) of x's component. Path compression flattens the tree for efficiency, though the dominant O(E²) cost here is from bubble sort.

---

## Time & Space Complexity

| Algorithm | Time Complexity | Space Complexity | Notes |
|-----------|----------------|-----------------|-------|
| BFS | O(V²) | O(V) | Adjacency matrix row scan; V=8 here |
| DFS | O(V²) | O(V) | Recursive, call stack depth ≤ V |
| Dijkstra | O(V²) | O(V) | Simple array, no priority queue |
| Prim | O(V²) | O(V) | Simple array, no priority queue |
| Kruskal | O(E²) | O(E + V) | Bubble sort dominates; E=15 here |
| Floyd-Warshall | O(V³) | O(V²) | 8³=512 iterations; dist matrix |
| Connectivity Check | O(V²) | O(V) | BFS-based, runs once at startup |

> **Note:** With V=8 and E=15, all algorithms run essentially instantaneously. The complexity matters more as graphs grow larger.

For Kruskal: the union-find operations add O(E · α(V)) where α is the inverse Ackermann function (nearly constant), but bubble sort's O(E²) is the dominant term. The total is **O(E²)** as documented in the code comments.

---

## Requirements

- A C compiler supporting **C11** — recommended: **GCC 9.0 or later**
- Windows, Linux, or macOS terminal

To check if GCC is installed:
```bash
gcc --version
```

On Windows, install GCC via [MinGW-w64](https://www.mingw-w64.org/) or [MSYS2](https://www.msys2.org/).

---

## Compilation & Execution

### Compile

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c -o project
```

Explanation of flags:
- `-std=c11` — use the C11 standard
- `-Wall` — enable all common warnings
- `-Wextra` — enable extra warnings
- `-pedantic` — enforce strict standard compliance
- `-o project` — name the output executable `project`

### Run

**Windows (PowerShell or Command Prompt):**
```
project.exe
```
or
```
.\project.exe
```

**Linux / macOS (Terminal):**
```bash
./project
```

---

## How to Run the Demo

Here is the recommended sequence to demonstrate all features to your instructor:

```
Step 1:  Choose option 1  → Show all 8 AI topics and their keywords
Step 2:  Choose option 2  → Display the adjacency matrix and edge list
Step 3:  Choose option 3  → Run BFS starting from topic 1 (Machine Learning)
Step 4:  Choose option 4  → Run DFS starting from topic 1 (Machine Learning)
Step 5:  Choose option 5  → Dijkstra: source = 1 (Machine Learning), dest = 5 (Medical AI)
                            Expected path: ML -> Deep Learning -> Medical AI, cost = 6
Step 6:  Choose option 6  → Run Prim's MST (total weight = 17)
Step 7:  Choose option 7  → Run Kruskal's MST (total weight = 17)
Step 8:  Choose option 8  → Compare both MSTs — confirm both show cost 17
Step 9:  Choose option 0  → Exit
```

### Testing Invalid Input

While running, you can also demonstrate input validation:
- Type `abc` at any menu prompt → message: `[!] Invalid input.`
- Type `9` at the main menu → message: `[!] Invalid choice.`
- Type `0` when asked for a topic → message: `[!] Choice out of range.`

---

## Example Output

```
========================================================
       AI DATASET EMBEDDING ORGANIZATION
       DSA Project  |  Graph Algorithms in C
========================================================
  Welcome! This project demonstrates graph algorithms
  on 8 AI research topics using an adjacency matrix.

--------------------------------------------------------
  MAIN MENU
--------------------------------------------------------
  1. Display AI Dataset Topics
  2. Display Graph / Adjacency Matrix
  3. Run BFS (Breadth-First Search)
  4. Run DFS (Depth-First Search)
  5. Run Dijkstra's Shortest Path
  6. Run Prim's MST
  7. Run Kruskal's MST
  8. Compare Prim and Kruskal MST Costs
  0. Exit
--------------------------------------------------------

--- BFS from Machine Learning ---
Machine Learning -> Deep Learning -> Computer Vision -> Natural Language Processing
  -> Recommendation Systems -> Medical AI -> Speech Recognition -> Robotics

--- DFS from Machine Learning ---
Machine Learning -> Deep Learning -> Computer Vision -> Medical AI -> Robotics
  -> Speech Recognition -> Natural Language Processing -> Recommendation Systems

--- Dijkstra: Machine Learning to Medical AI ---
Shortest path (cost = 6):
  Machine Learning --> Deep Learning --> Medical AI
  Machine Learning --[2]--> Deep Learning
  Deep Learning    --[4]--> Medical AI
  Total cost: 6

--- Prim's MST (Total Weight: 17) ---
  Machine Learning  --[2]--  Deep Learning
  Deep Learning     --[1]--  Computer Vision
  Deep Learning     --[2]--  Speech Recognition
  Machine Learning  --[3]--  Natural Language Processing
  NLP               --[2]--  Recommendation Systems
  Computer Vision   --[3]--  Robotics
  Deep Learning     --[4]--  Medical AI

--- Kruskal's MST (Total Weight: 17) ---
  [ADDED]  Deep Learning  --[1]--  Computer Vision
  [ADDED]  Machine Learning  --[2]--  Deep Learning
  [ADDED]  Deep Learning  --[2]--  Speech Recognition
  [ADDED]  NLP  --[2]--  Recommendation Systems
  [ADDED]  Machine Learning  --[3]--  NLP
  [SKIP]   Deep Learning  --[3]--  NLP  (would form cycle)
  [ADDED]  Computer Vision  --[3]--  Robotics
  [SKIP]   Speech Recognition  --[3]--  Recommendation Systems  (cycle)
  [SKIP]   Machine Learning  --[4]--  Computer Vision  (cycle)
  [ADDED]  Deep Learning  --[4]--  Medical AI

--- Compare MST ---
  Prim's    MST Total Weight : 17
  Kruskal's MST Total Weight : 17
  [OK] Both algorithms give the SAME MST cost: 17
```

---

## Why Prim and Kruskal Give the Same MST Cost

For any **connected, weighted, undirected graph** where all edge weights are **distinct** (no two edges have the same weight), there is **exactly one** Minimum Spanning Tree. Therefore:

- Prim's algorithm, which grows the MST vertex by vertex, and
- Kruskal's algorithm, which grows the MST edge by edge by weight,

**must arrive at the same set of edges** and therefore the same total weight.

In this project, all 15 edge weights are distinct (checked in the adjacency matrix), so the MST is unique and both algorithms find it. The verified cost is **17**.

Even if some weights were equal (non-unique), both algorithms would still produce MSTs with the same **total weight**, just possibly using different sets of edges.

---

## Limitations

1. **Weights are illustrative, not real embeddings.** The edge weights are predefined integers assigned to demonstrate the algorithms. They do NOT represent actual semantic distances computed from AI text embeddings, word vectors, or any machine learning model.

2. **Fixed dataset.** The 8 topics and adjacency matrix are hardcoded. The application does not load topics or weights from a file at runtime.

3. **No actual AI or NLP processing.** This is a pure DSA prototype. No text is analyzed, no model is trained, and no embeddings are generated.

4. **Small scale.** With V = 8, performance is trivially fast. The time complexities documented (O(V²), O(E²)) would matter for much larger graphs.

5. **Single-file design.** All functions are in `main.c` for simplicity. A larger project would separate code into multiple headers and source files.

---

## Future Improvements

1. **Load topics from CSV.** Read topic names and keywords from `sample_dataset.csv` at startup instead of hardcoding them.

2. **Load the adjacency matrix from a file.** Allow the graph weights to be defined in a text file so they can be changed without recompiling.

3. **Real embeddings.** Integrate a pre-computed embedding file (e.g., from Word2Vec or sentence-transformers) and compute actual cosine-similarity distances to use as edge weights.

4. **Adjacency list.** Implement an adjacency list representation for better performance on large, sparse graphs.

5. **Priority queue for Dijkstra and Prim.** Replace the O(V²) array-based minimum search with a min-heap to achieve O((V + E) log V) complexity.

6. **Graph visualization.** Output the graph in DOT format for rendering with Graphviz.

---

## Project Structure

```
DSA Project - 2.2/
├── main.c             <- Complete C source code (all 6 algorithms in one file)
├── README.md          <- Project documentation (this file)
├── ALGORITHMS.md      <- Beginner reference: pseudocode + complexity for all 6 algorithms
├── CHANGELOG.md       <- Project version history and planned features
├── sample_output.txt  <- Representative terminal output for all 9 menu options
├── sample_dataset.csv <- Dataset of AI topic names and keywords
└── .gitignore         <- Excludes compiled binaries (*.exe, *.o) and IDE artifacts
```

---

## Team Members

| Role | Name | Student ID |
|------|------|-----------|
| Member 1 | *(Your name here)* | *(Your ID here)* |
| Member 2 | *(Your name here)* | *(Your ID here)* |
| Member 3 | *(Your name here)* | *(Your ID here)* |

> Replace the placeholders above with your actual team member names and IDs.

---

## License

This project is submitted as a university coursework assignment. All code was written by the team members listed above.
