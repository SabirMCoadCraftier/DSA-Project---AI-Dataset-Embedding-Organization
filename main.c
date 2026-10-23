/*
 * ============================================================
 *  PROJECT : AI Dataset Embedding Organization
 *  COURSE  : Data Structures and Algorithms (DSA)
 *  LANG    : C (C11 standard)
 *
 *  DESCRIPTION:
 *    Represents 8 AI research topics as a weighted, undirected
 *    graph using an adjacency matrix. Implements BFS, DFS,
 *    Dijkstra's shortest path, Prim's MST, and Kruskal's MST.
 *
 *  NOTE ON WEIGHTS:
 *    Edge weights are predefined illustrative "connection costs"
 *    to show topic relationships. They are NOT real AI embeddings
 *    or outputs of any machine learning model.
 *
 *  COMPILE:
 *    gcc -std=c11 -Wall -Wextra -pedantic main.c -o project
 *
 *  RUN (Windows):  project.exe
 *  RUN (Linux):    ./project
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>   /* for strtol error checking */

/* ── Constants ──────────────────────────────────────────── */
#define V        8          /* Number of vertices (AI topics) */
#define INF      99999      /* Represents "no path / infinity" */
#define MAX_EDGES ((V * (V - 1)) / 2)   /* Max edges in undirected graph */

/* ── Topic Names & Keywords ──────────────────────────────── */
const char *topics[V] = {
    "Machine Learning",
    "Deep Learning",
    "Computer Vision",
    "Natural Language Processing",
    "Medical AI",
    "Robotics",
    "Speech Recognition",
    "Recommendation Systems"
};

const char *keywords[V] = {
    "supervised, unsupervised, regression, classification",
    "neural networks, backpropagation, CNN, RNN, transformers",
    "image recognition, object detection, segmentation, OpenCV",
    "text processing, tokenization, BERT, GPT, sentiment analysis",
    "disease prediction, medical imaging, drug discovery, EHR",
    "autonomous navigation, sensors, control systems, manipulation",
    "ASR, phonemes, acoustic models, language models, CTC",
    "collaborative filtering, matrix factorization, user preferences"
};

/*
 * ── Adjacency Matrix ──────────────────────────────────────
 *
 *  Topics are numbered 0..7 (displayed as 1..8 to the user).
 *  graph[i][j] > 0  → edge between topic i and j with that weight.
 *  graph[i][j] == 0 → no direct edge.
 *  The matrix is symmetric (undirected): graph[i][j] == graph[j][i].
 *
 *  Weights represent illustrative "connection costs" — lower means
 *  the two topics are more closely related in this prototype.
 *
 *  Index mapping:
 *   0 = Machine Learning        4 = Medical AI
 *   1 = Deep Learning           5 = Robotics
 *   2 = Computer Vision         6 = Speech Recognition
 *   3 = Natural Language Proc.  7 = Recommendation Systems
 */
int graph[V][V] = {
    /*  ML  DL  CV  NLP  MAI  ROB  SR   RS  */
    {  0,  2,  4,  3,   0,   0,   0,   5  },  /* 0: Machine Learning     */
    {  2,  0,  1,  3,   4,   0,   2,   0  },  /* 1: Deep Learning        */
    {  4,  1,  0,  0,   5,   3,   0,   0  },  /* 2: Computer Vision      */
    {  3,  3,  0,  0,   0,   0,   4,   2  },  /* 3: NLP                  */
    {  0,  4,  5,  0,   0,   6,   0,   0  },  /* 4: Medical AI           */
    {  0,  0,  3,  0,   6,   0,   5,   0  },  /* 5: Robotics             */
    {  0,  2,  0,  4,   0,   5,   0,   3  },  /* 6: Speech Recognition   */
    {  5,  0,  0,  2,   0,   0,   3,   0  }   /* 7: Recommendation Sys.  */
};

/* ============================================================
 *  UTILITY FUNCTIONS
 * ============================================================ */

/* Print a divider line for readability */
void print_divider(void) {
    printf("--------------------------------------------------------\n");
}

/* Print the main project banner */
void print_banner(void) {
    printf("\n");
    printf("========================================================\n");
    printf("       AI DATASET EMBEDDING ORGANIZATION                \n");
    printf("       DSA Project  |  Graph Algorithms in C            \n");
    printf("========================================================\n");
}

/*
 * check_graph_connected()
 * ──────────────────────
 * Uses an internal BFS from vertex 0 to verify that every vertex in the
 * graph is reachable. Returns 1 if connected, 0 if any vertex is isolated.
 *
 * This is called once at program startup so the user is immediately
 * informed if the hardcoded adjacency matrix has a disconnected vertex
 * (which would make Prim/Kruskal produce incomplete spanning trees).
 *
 * Time Complexity: O(V^2) — same BFS with adjacency matrix.
 */
int check_graph_connected(void) {
    int visited[V];
    int queue[V];
    int front = 0, rear = 0;

    for (int i = 0; i < V; i++) visited[i] = 0;

    visited[0] = 1;
    queue[rear++] = 0;

    while (front < rear) {
        int cur = queue[front++];
        for (int j = 0; j < V; j++) {
            if (graph[cur][j] != 0 && !visited[j]) {
                visited[j] = 1;
                queue[rear++] = j;
            }
        }
    }

    /* Count reachable vertices */
    int reachable = 0;
    for (int i = 0; i < V; i++) reachable += visited[i];
    return reachable == V;  /* 1 = connected, 0 = disconnected */
}

/*
 * Safe integer input using fgets + strtol.
 * Returns  1  on success (valid integer read).
 * Returns  0  on bad/non-numeric input (show error, keep looping).
 * Returns -1  on EOF / stdin closed   (caller should exit cleanly).
 */
int read_int(int *out) {
    char buf[64];
    if (fgets(buf, sizeof(buf), stdin) == NULL) return -1;  /* EOF or error */

    /* Remove trailing newline */
    buf[strcspn(buf, "\n")] = '\0';

    if (buf[0] == '\0') return 0;   /* empty input */

    char *endptr;
    errno = 0;
    long val = strtol(buf, &endptr, 10);

    /* endptr must point to '\0', no leftovers */
    if (*endptr != '\0' || errno != 0) return 0;

    *out = (int)val;
    return 1;
}

/*
 * Ask the user to select a topic (1..V).
 * Returns the 0-based index on success.
 * Returns -1 on invalid/out-of-range input (caller shows menu again).
 * Returns -2 on EOF (Ctrl+D / Ctrl+Z) so the caller can exit cleanly.
 *
 * Before this fix, select_topic used !read_int() which treated both
 * bad-format (rc==0) and EOF (rc==-1) identically, potentially
 * printing a confusing "invalid input" message on EOF.
 */
int select_topic(const char *prompt) {
    printf("%s (1-%d): ", prompt, V);
    int choice = 0;
    int rc = read_int(&choice);

    if (rc == -1) {
        /* EOF — signal the caller to exit */
        printf("\n  [EOF] Input closed. Returning to menu.\n");
        return -2;
    }
    if (rc == 0) {
        printf("  [!] Invalid input. Please enter a number.\n");
        return -1;
    }
    if (choice < 1 || choice > V) {
        printf("  [!] Choice out of range. Enter 1 to %d.\n", V);
        return -1;
    }
    return choice - 1;   /* convert to 0-based index */
}

/* ============================================================
 *  MENU OPTION 1 : Display AI Dataset Topics
 * ============================================================ */
void display_topics(void) {
    printf("\n");
    print_divider();
    printf("  AI RESEARCH TOPICS IN THIS DATASET\n");
    print_divider();
    for (int i = 0; i < V; i++) {
        printf("  %d. %-32s\n", i + 1, topics[i]);
        printf("     Keywords: %s\n\n", keywords[i]);
    }
    print_divider();
}

/* ============================================================
 *  MENU OPTION 2 : Display Adjacency Matrix
 * ============================================================ */
void display_graph(void) {
    printf("\n");
    print_divider();
    printf("  WEIGHTED ADJACENCY MATRIX  (0 = no edge)\n");
    print_divider();

    /* Column headers (abbreviated) */
    printf("       ");
    for (int j = 0; j < V; j++) {
        printf(" [%d]", j + 1);
    }
    printf("\n");

    /* Rows */
    for (int i = 0; i < V; i++) {
        printf("  [%d]  ", i + 1);
        for (int j = 0; j < V; j++) {
            printf("  %2d ", graph[i][j]);
        }
        printf("  <- %s\n", topics[i]);
    }

    printf("\n  Edge list (topic A -- weight -- topic B):\n");
    print_divider();
    int edge_count = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (graph[i][j] != 0) {
                printf("  %s  --[%d]--  %s\n",
                       topics[i], graph[i][j], topics[j]);
                edge_count++;
            }
        }
    }
    printf("\n  Total edges: %d\n", edge_count);

    /* ── Vertex Degree Statistics ─────────────────────────────
     * Degree of a vertex = number of edges connected to it.
     * For a weighted graph we also show the sum of weights
     * (total connection cost) for each topic.
     */
    printf("\n  Vertex degree statistics:\n");
    print_divider();
    printf("  %-32s  Degree  Total Weight\n", "Topic");
    print_divider();
    int total_graph_weight = 0;
    for (int i = 0; i < V; i++) {
        int degree = 0;
        int weight_sum = 0;
        for (int j = 0; j < V; j++) {
            if (graph[i][j] != 0) {
                degree++;
                weight_sum += graph[i][j];
            }
        }
        printf("  %-32s     %d       %d\n", topics[i], degree, weight_sum);
        total_graph_weight += weight_sum;
    }
    /* Each edge is counted twice in the adjacency matrix */
    printf("\n  Sum of all edge weights (graph total): %d\n",
           total_graph_weight / 2);
    print_divider();
}

/* ============================================================
 *  MENU OPTION 3 : BFS (Breadth-First Search)
 *
 *  Algorithm idea:
 *    1. Start at the chosen vertex. Mark it visited. Add to queue.
 *    2. While the queue is not empty:
 *       a. Dequeue the front vertex, print it.
 *       b. For each vertex check the row in the adjacency matrix.
 *          If there is an edge AND the neighbor is not visited,
 *          mark it visited and enqueue it.
 *    BFS explores all immediate neighbors before going deeper.
 *
 *  Time Complexity : O(V^2)  — due to adjacency matrix row scan.
 *  Space Complexity: O(V)    — visited array + queue.
 * ============================================================ */
void bfs(int start) {
    int visited[V];          /* visited[i] = 1 if topic i was visited */
    int queue[V];            /* simple array-based circular queue      */
    int front = 0, rear = 0;

    /* Initialise */
    for (int i = 0; i < V; i++) visited[i] = 0;

    /* Step 1: mark start, enqueue */
    visited[start] = 1;
    queue[rear++] = start;

    printf("\n  BFS Traversal starting from: %s\n\n", topics[start]);
    printf("  Traversal order:\n  ");

    int first = 1;  /* flag to avoid printing leading arrow */

    /* Step 2: process queue */
    while (front < rear) {
        int current = queue[front++];   /* dequeue */

        if (!first) printf(" -> ");
        printf("%s", topics[current]);
        first = 0;

        /* Check each possible neighbor in the adjacency matrix row */
        for (int j = 0; j < V; j++) {
            if (graph[current][j] != 0 && !visited[j]) {
                visited[j] = 1;
                queue[rear++] = j;
            }
        }
    }
    printf("\n\n  [BFS explores neighbors level by level -- like ripples in water.]\n");
    printf("  [Time Complexity: O(V^2) with adjacency matrix.]\n");
}

/* ============================================================
 *  MENU OPTION 4 : DFS (Depth-First Search) -- recursive helper
 *
 *  Algorithm idea:
 *    1. Mark the current vertex as visited. Print it.
 *    2. For each neighbor in the adjacency matrix:
 *       If there is an edge AND the neighbor is not visited,
 *       recursively call DFS on that neighbor.
 *    DFS goes as deep as possible before backtracking.
 *
 *  Time Complexity : O(V^2)  -- adjacency matrix row scan at each call.
 *  Space Complexity: O(V)    -- call stack depth + visited array.
 * ============================================================ */
static int dfs_visited[V];  /* shared visited array for DFS */

void dfs_recursive(int current, int *first) {
    dfs_visited[current] = 1;

    if (!(*first)) printf(" -> ");
    printf("%s", topics[current]);
    *first = 0;

    for (int j = 0; j < V; j++) {
        if (graph[current][j] != 0 && !dfs_visited[j]) {
            dfs_recursive(j, first);
        }
    }
}

void dfs(int start) {
    /* Initialise visited array */
    for (int i = 0; i < V; i++) dfs_visited[i] = 0;

    printf("\n  DFS Traversal starting from: %s\n\n", topics[start]);
    printf("  Traversal order:\n  ");

    int first = 1;
    dfs_recursive(start, &first);

    printf("\n\n  [DFS goes as deep as possible before backtracking.]\n");
    printf("  [Time Complexity: O(V^2) with adjacency matrix.]\n");
}

/* ============================================================
 *  MENU OPTION 5 : Dijkstra's Shortest Path Algorithm
 *
 *  Algorithm idea (array-based, no priority queue):
 *    1. Set dist[src] = 0, all others = INF.
 *    2. Repeat V times:
 *       a. Pick the unvisited vertex u with the smallest dist[u].
 *       b. Mark u as visited.
 *       c. For each neighbor v of u:
 *          If dist[u] + graph[u][v] < dist[v], update dist[v]
 *          and record parent[v] = u (for path reconstruction).
 *    3. Print dist[dest] as the shortest cost, then trace back
 *       through parent[] to print the path.
 *
 *  Requirement: all edge weights must be positive (they are here).
 *  Time Complexity : O(V^2)  -- two nested loops over V vertices.
 *  Space Complexity: O(V)    -- dist, visited, parent arrays.
 * ============================================================ */
void dijkstra(int src, int dest) {
    int dist[V];      /* shortest distance from src to each vertex */
    int visited[V];   /* 1 if vertex is finalised                  */
    int parent[V];    /* parent vertex for path reconstruction      */

    /* Step 1: Initialise */
    for (int i = 0; i < V; i++) {
        dist[i]    = INF;
        visited[i] = 0;
        parent[i]  = -1;
    }
    dist[src] = 0;

    /* Step 2: Repeat V times */
    for (int iter = 0; iter < V; iter++) {

        /* 2a: Find unvisited vertex with minimum distance */
        int u = -1;
        for (int i = 0; i < V; i++) {
            if (!visited[i] && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }

        if (u == -1 || dist[u] == INF) break;  /* remaining unreachable */

        /* 2b: Mark as visited */
        visited[u] = 1;

        /* 2c: Relax neighbours */
        for (int v = 0; v < V; v++) {
            if (graph[u][v] != 0 && !visited[v]) {
                int new_dist = dist[u] + graph[u][v];
                if (new_dist < dist[v]) {
                    dist[v]   = new_dist;
                    parent[v] = u;
                }
            }
        }
    }

    /* Step 3: Print result */
    printf("\n");
    print_divider();
    printf("  DIJKSTRA'S SHORTEST PATH\n");
    printf("  Source     : %s\n", topics[src]);
    printf("  Destination: %s\n", topics[dest]);
    print_divider();

    if (dist[dest] == INF) {
        printf("  No path found between the two topics.\n");
        print_divider();
        return;
    }

    /* Reconstruct path using parent array (store in reverse, then print) */
    int path[V];
    int path_len = 0;
    int cur = dest;
    while (cur != -1) {
        path[path_len++] = cur;
        cur = parent[cur];
    }

    printf("  Shortest path (cost = %d):\n  ", dist[dest]);
    for (int i = path_len - 1; i >= 0; i--) {
        printf("%s", topics[path[i]]);
        if (i > 0) printf("  -->  ");
    }
    printf("\n");

    /* Show step-by-step edge costs */
    printf("\n  Step-by-step edge costs:\n");
    for (int i = path_len - 1; i > 0; i--) {
        int from = path[i], to = path[i - 1];
        printf("    %s  --[%d]-->  %s\n",
               topics[from], graph[from][to], topics[to]);
    }
    printf("    Total cost: %d\n", dist[dest]);
    print_divider();
    printf("  [Time Complexity: O(V^2) -- simple array-based Dijkstra.]\n");
}

/* ============================================================
 *  MENU OPTION 6 : Prim's MST Algorithm
 *
 *  Algorithm idea (array-based):
 *    1. Start with vertex 0. Mark it in the MST set.
 *       key[i]  = cheapest edge connecting vertex i to the MST.
 *       parent[i] = the MST vertex that offers that cheap edge.
 *    2. Repeat V-1 times:
 *       a. Pick the vertex u NOT yet in MST with minimum key[u].
 *       b. Add u to MST. Record the edge (parent[u]--u).
 *       c. For each neighbor v of u: if graph[u][v] < key[v],
 *          update key[v] and parent[v].
 *    3. Print all V-1 MST edges and the total weight.
 *
 *  Time Complexity : O(V^2)  -- two nested loops.
 *  Space Complexity: O(V)    -- key, parent, inMST arrays.
 * ============================================================ */
int prim_mst(int print_output) {
    int key[V];     /* cheapest edge weight to connect vertex i to MST */
    int parent[V];  /* MST parent of each vertex                       */
    int inMST[V];   /* 1 if vertex is already in the MST set           */

    /* Step 1: Initialise */
    for (int i = 0; i < V; i++) {
        key[i]   = INF;
        inMST[i] = 0;
        parent[i] = -1;
    }
    key[0] = 0;   /* start from vertex 0 (Machine Learning) */

    int total_weight = 0;

    if (print_output) {
        printf("\n");
        print_divider();
        printf("  PRIM'S MINIMUM SPANNING TREE\n");
        print_divider();
        printf("  MST Edges selected:\n");
    }

    /* Step 2: Repeat V-1 times to add V-1 edges */
    for (int iter = 0; iter < V; iter++) {

        /* 2a: Pick vertex with minimum key NOT yet in MST */
        int u = -1;
        for (int i = 0; i < V; i++) {
            if (!inMST[i] && (u == -1 || key[i] < key[u])) {
                u = i;
            }
        }

        /* 2b: Add u to MST */
        inMST[u] = 1;

        if (parent[u] != -1) {   /* skip the very first vertex (no parent) */
            total_weight += graph[parent[u]][u];
            if (print_output) {
                printf("    %s  --[%d]--  %s\n",
                       topics[parent[u]], graph[parent[u]][u], topics[u]);
            }
        }

        /* 2c: Update keys for neighbours of u */
        for (int v = 0; v < V; v++) {
            if (graph[u][v] != 0 && !inMST[v] && graph[u][v] < key[v]) {
                key[v]    = graph[u][v];
                parent[v] = u;
            }
        }
    }

    if (print_output) {
        printf("\n  Total MST Weight (Prim): %d\n", total_weight);
        print_divider();
        printf("  [Prim grows the MST one vertex at a time from the start node.]\n");
        printf("  [Time Complexity: O(V^2) with adjacency matrix.]\n");
    }

    return total_weight;
}

/* ============================================================
 *  MENU OPTION 7 : Kruskal's MST Algorithm
 *
 *  Algorithm idea:
 *    1. Extract all edges from the adjacency matrix into an array.
 *    2. Sort edges by ascending weight (using bubble sort here).
 *    3. Use a Union-Find (Disjoint Set Union) structure:
 *         find(x) -- returns the root/representative of x's set,
 *                   using path compression for efficiency.
 *         union_sets(a,b) -- merges the sets of a and b.
 *    4. Iterate sorted edges. For each edge (u,v):
 *         If u and v belong to DIFFERENT sets, add the edge to
 *         the MST and union their sets.
 *         Stop once V-1 edges are selected.
 *
 *  Time Complexity : O(E^2) -- bubble sort dominates.
 *                   O(E * alpha(V)) -- for union-find operations.
 *                   Overall: O(E^2) because bubble sort dominates.
 *  Space Complexity: O(E + V) -- edge array + parent array.
 * ============================================================ */

/* --- Edge struct for Kruskal --- */
typedef struct {
    int u, v, weight;
} Edge;

/* --- Union-Find: find with path compression --- */
int uf_find(int parent_arr[], int x) {
    if (parent_arr[x] != x) {
        parent_arr[x] = uf_find(parent_arr, parent_arr[x]);  /* path compression */
    }
    return parent_arr[x];
}

/* --- Union-Find: union two sets --- */
void uf_union(int parent_arr[], int a, int b) {
    int ra = uf_find(parent_arr, a);
    int rb = uf_find(parent_arr, b);
    if (ra != rb) {
        parent_arr[ra] = rb;   /* attach root of a under root of b */
    }
}

/* --- Bubble sort edges by ascending weight --- */
void bubble_sort_edges(Edge edges[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp    = edges[j];
                edges[j]     = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
}

int kruskal_mst(int print_output) {
    /* Step 1: Extract edges from upper triangle of adjacency matrix */
    Edge edges[MAX_EDGES];
    int edge_count = 0;

    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (graph[i][j] != 0) {
                edges[edge_count].u      = i;
                edges[edge_count].v      = j;
                edges[edge_count].weight = graph[i][j];
                edge_count++;
            }
        }
    }

    /* Step 2: Sort edges by weight (bubble sort) */
    bubble_sort_edges(edges, edge_count);

    /* Step 3: Initialise Union-Find */
    int uf_parent[V];
    for (int i = 0; i < V; i++) uf_parent[i] = i;  /* each vertex is its own root */

    int total_weight = 0;
    int mst_edges    = 0;

    if (print_output) {
        printf("\n");
        print_divider();
        printf("  KRUSKAL'S MINIMUM SPANNING TREE\n");
        print_divider();
        printf("  Sorted edges considered:\n");
    }

    /* Step 4: Greedily pick edges */
    for (int i = 0; i < edge_count && mst_edges < V - 1; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;

        int ru = uf_find(uf_parent, u);
        int rv = uf_find(uf_parent, v);

        if (ru != rv) {
            /* Different sets -- no cycle -- add to MST */
            uf_union(uf_parent, u, v);
            total_weight += w;
            mst_edges++;
            if (print_output) {
                printf("    [ADDED]  %s  --[%d]--  %s\n",
                       topics[u], w, topics[v]);
            }
        } else {
            if (print_output) {
                printf("    [SKIP]   %s  --[%d]--  %s  (would form cycle)\n",
                       topics[u], w, topics[v]);
            }
        }
    }

    if (print_output) {
        printf("\n  Total MST Weight (Kruskal): %d\n", total_weight);
        print_divider();
        printf("  [Kruskal greedily picks the cheapest edge that doesn't form a cycle.]\n");
        printf("  [Time Complexity: O(E^2) due to bubble sort.]\n");
    }

    return total_weight;
}

/* ============================================================
 *  MENU OPTION 8 : Compare Prim and Kruskal MST Costs
 * ============================================================ */
void compare_mst(void) {
    printf("\n");
    print_divider();
    printf("  COMPARING PRIM vs KRUSKAL MST\n");
    print_divider();

    /* Run both silently to get their costs */
    int prim_cost    = prim_mst(0);    /* 0 = silent run  */
    int kruskal_cost = kruskal_mst(0); /* 0 = silent run  */

    printf("  Prim's    MST Total Weight : %d\n", prim_cost);
    printf("  Kruskal's MST Total Weight : %d\n", kruskal_cost);
    printf("\n");

    if (prim_cost == kruskal_cost) {
        printf("  [OK] Both algorithms give the SAME MST cost: %d\n", prim_cost);
        printf("  This is expected! For a connected graph with unique edge\n");
        printf("  weights, there is exactly one MST and both algorithms find it.\n");
    } else {
        printf("  [!] Costs differ -- check the graph for issues.\n");
    }
    print_divider();
}

/* ============================================================
 *  MENU OPTION 9 : Floyd-Warshall All-Pairs Shortest Path
 *
 *  Algorithm idea:
 *    Dijkstra finds the shortest path FROM one source to all
 *    others. Floyd-Warshall finds shortest paths between EVERY
 *    pair of vertices in a single pass.
 *
 *    Idea: for each intermediate vertex k (0..V-1),
 *          for each source i and destination j:
 *            if going i->k->j is shorter than the known i->j
 *            distance, update dist[i][j].
 *
 *    After V rounds, dist[i][j] holds the shortest path cost
 *    between every pair (i, j).
 *
 *  Time Complexity : O(V^3) — three nested loops, each 0..V-1.
 *  Space Complexity: O(V^2) — the distance matrix.
 *
 *  For V=8: 8^3 = 512 operations. Runs instantly.
 * ============================================================ */
void floyd_warshall(void) {
    /* Step 1: Initialise dist matrix from the adjacency matrix */
    int dist[V][V];

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
                dist[i][j] = 0;           /* distance to self = 0 */
            } else if (graph[i][j] != 0) {
                dist[i][j] = graph[i][j]; /* direct edge cost     */
            } else {
                dist[i][j] = INF;         /* no direct edge       */
            }
        }
    }

    /* Step 2: Try every vertex k as an intermediate "relay" */
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                /* Can we go i -> k -> j cheaper than i -> j directly? */
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    /* Step 3: Print the full shortest-path distance table */
    printf("\n");
    print_divider();
    printf("  FLOYD-WARSHALL: ALL-PAIRS SHORTEST PATH TABLE\n");
    printf("  (Each cell = minimum cost between row-topic and column-topic)\n");
    print_divider();

    /* Column header (topic numbers) */
    printf("      ");
    for (int j = 0; j < V; j++) printf(" [%d] ", j + 1);
    printf("\n");
    print_divider();

    /* Rows */
    for (int i = 0; i < V; i++) {
        printf("  [%d] ", i + 1);
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF) {
                printf("  INF ");
            } else {
                printf("  %2d  ", dist[i][j]);
            }
        }
        printf(" <- %s\n", topics[i]);
    }

    print_divider();
    printf("  How to read: row = source topic, column = destination topic.\n");
    printf("  Example: row 1, col 5 = shortest cost from\n");
    printf("           %s to %s = %d\n",
           topics[0], topics[4], dist[0][4]);
    printf("\n  [Floyd-Warshall explores ALL pairs simultaneously.]\n");
    printf("  [Time Complexity: O(V^3). Space Complexity: O(V^2).]\n");
    printf("  [For V=8: %d^3 = %d iterations total.]\n", V, V * V * V);
    print_divider();
}

/* ============================================================
 *  MAIN MENU
 * ============================================================ */
void print_menu(void) {
    printf("\n");
    print_divider();
    printf("  MAIN MENU\n");
    print_divider();
    printf("  1. Display AI Dataset Topics\n");
    printf("  2. Display Graph / Adjacency Matrix\n");
    printf("  3. Run BFS (Breadth-First Search)\n");
    printf("  4. Run DFS (Depth-First Search)\n");
    printf("  5. Run Dijkstra's Shortest Path\n");
    printf("  6. Run Prim's MST\n");
    printf("  7. Run Kruskal's MST\n");
    printf("  8. Compare Prim and Kruskal MST Costs\n");
    printf("  9. Floyd-Warshall All-Pairs Shortest Path\n");
    printf("  0. Exit\n");
    print_divider();
    printf("  Enter your choice: ");
}

/* ============================================================
 *  MAIN FUNCTION
 * ============================================================ */
int main(void) {
    print_banner();
    printf("  Welcome! This project demonstrates graph algorithms\n");
    printf("  on 8 AI research topics using an adjacency matrix.\n");

    /* Startup sanity check: confirm all 8 vertices are connected */
    if (check_graph_connected()) {
        printf("  [OK] Graph connectivity verified: all %d topics reachable.\n", V);
    } else {
        printf("  [WARNING] Graph is DISCONNECTED. Some algorithms may be incorrect.\n");
    }

    int running = 1;

    while (running) {
        print_menu();

        int choice = 0;
        int rc = read_int(&choice);
        if (rc == -1) {
            /* EOF: user closed stdin (Ctrl+D / Ctrl+Z) -- exit cleanly */
            printf("\n  [EOF] Exiting. Goodbye!\n\n");
            break;
        }
        if (rc == 0) {
            printf("  [!] Invalid input. Please enter a number (0-8).\n");
            continue;
        }

        switch (choice) {

            case 1:
                display_topics();
                break;

            case 2:
                display_graph();
                break;

            case 3: {
                /* BFS -- ask for starting topic */
                display_topics();
                int start = select_topic("  Enter starting topic number");
                if (start < 0) break;  /* -1 = bad input, -2 = EOF */
                bfs(start);
                break;
            }

            case 4: {
                /* DFS -- ask for starting topic */
                display_topics();
                int start = select_topic("  Enter starting topic number");
                if (start < 0) break;  /* -1 = bad input, -2 = EOF */
                dfs(start);
                break;
            }

            case 5: {
                /* Dijkstra -- ask for source and destination */
                display_topics();
                int src = select_topic("  Enter SOURCE topic number");
                if (src < 0) break;  /* -1 = bad input, -2 = EOF */
                int dest = select_topic("  Enter DESTINATION topic number");
                if (dest < 0) break;
                if (src == dest) {
                    printf("  [!] Source and destination are the same. Cost = 0.\n");
                    break;
                }
                dijkstra(src, dest);
                break;
            }

            case 6:
                /* Prim -- no extra input needed */
                prim_mst(1);   /* 1 = print output */
                break;

            case 7:
                /* Kruskal -- no extra input needed */
                kruskal_mst(1);   /* 1 = print output */
                break;

            case 8:
                compare_mst();
                break;

            case 9:
                /* Floyd-Warshall -- no extra input needed */
                floyd_warshall();
                break;

            case 0:
                printf("\n  Thank you for using the AI Dataset Embedding Organizer!\n");
                printf("  Goodbye.\n\n");
                running = 0;
                break;


            default:
                printf("  [!] Invalid choice. Please enter 0 to 9.\n");
                break;
        }
    }

    return 0;
}
