// tsp_bnb.cpp
// Compile: g++ -std=c++17 -O2 tsp_bnb.cpp -o tsp_bnb
// Run: ./tsp_bnb < input.txt   (or run interactively)

#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9;

struct Node {
    vector<vector<int>> mat;
    int lb;               // lower bound
    vector<int> path;     // visited cities in order
    int level;            // path.size()-1
};

struct PQCmp {
    bool operator()(const Node &a, const Node &b) const {
        return a.lb > b.lb; // min-heap
    }
};

// Reduce matrix in-place and return reduction cost
int reduceMatrix(vector<vector<int>>& mat) {
    int n = mat.size();
    int red = 0;
    // Row reduction
    for (int i = 0; i < n; ++i) {
        int mn = INF;
        for (int j = 0; j < n; ++j) if (mat[i][j] < mn) mn = mat[i][j];
        if (mn == INF || mn == 0) continue;
        for (int j = 0; j < n; ++j) if (mat[i][j] < INF) mat[i][j] -= mn;
        red += mn;
    }
    // Column reduction
    for (int j = 0; j < n; ++j) {
        int mn = INF;
        for (int i = 0; i < n; ++i) if (mat[i][j] < mn) mn = mat[i][j];
        if (mn == INF || mn == 0) continue;
        for (int i = 0; i < n; ++i) if (mat[i][j] < INF) mat[i][j] -= mn;
        red += mn;
    }
    return red;
}

pair<int, vector<int>> tsp_branch_and_bound(const vector<vector<int>>& cost, int start = 0) {
    int n = cost.size();
    // prepare root
    vector<vector<int>> rootMat = cost;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (rootMat[i][j] < 0) rootMat[i][j] = INF; // treat negative as no-edge

    int lb0 = reduceMatrix(rootMat);
    Node root{rootMat, lb0, {start}, 0};

    int bestCost = INF;
    vector<int> bestPath;

    priority_queue<Node, vector<Node>, PQCmp> pq;
    pq.push(root);

    while (!pq.empty()) {
        Node node = pq.top(); pq.pop();
        if (node.lb >= bestCost) continue;
        if (node.level == n - 1) {
            int last = node.path.back();
            int extra = cost[last][start] < 0 ? INF : cost[last][start];
            if (extra >= INF) continue;
            int total = node.lb + extra;
            if (total < bestCost) {
                bestCost = total;
                bestPath = node.path;
                bestPath.push_back(start);
            }
            continue;
        }
        int u = node.path.back();
        // try extending to every city not in path
        vector<char> inPath(n, 0);
        for (int x : node.path) inPath[x] = 1;

        for (int v = 0; v < n; ++v) {
            if (inPath[v]) continue;
            if (cost[u][v] < 0) continue; // no direct edge

            // Create child matrix
            vector<vector<int>> childMat = node.mat;

            // add actual cost of edge u->v (before reductions)
            int edgeCost = cost[u][v];
            // block row u and column v
            for (int j = 0; j < n; ++j) childMat[u][j] = INF;
            for (int i = 0; i < n; ++i) childMat[i][v] = INF;
            // prevent returning v->u
            childMat[v][u] = INF;

            int reduction = reduceMatrix(childMat);
            int childLB = node.lb + edgeCost + reduction;

            if (childLB >= bestCost) continue;

            Node child;
            child.mat = move(childMat);
            child.lb = childLB;
            child.path = node.path;
            child.path.push_back(v);
            child.level = node.level + 1;

            pq.push(child);
        }
    }

    if (bestCost >= INF) return {-1, {}};
    return {bestCost, bestPath};
}

int main() {
 //   ios::sync_with_stdio(false);
   // cin.tie(nullptr);

    int n;
    cout << "Enter N (number of cities): ";
    if (!(cin >> n)) return 0;
    vector<vector<int>> cost(n, vector<int>(n));
    cout << "Enter cost matrix (use -1 for no direct path). " << "\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> cost[i][j];

    int start = 0;
    cout << "Enter start city index (0-based, default 0): ";
    if (!(cin >> start)) start = 0;

    auto res = tsp_branch_and_bound(cost, start);
    if (res.first < 0) {
        cout << "No Hamiltonian tour exists.\n";
    } else {
        cout << "Optimal cost: " << res.first << "\n";
        cout << "Path: ";
        for (size_t i = 0; i < res.second.size(); ++i) {
            cout << res.second[i] << (i + 1 < res.second.size() ? " -> " : "\n");
        }
    }
    return 0;
}

