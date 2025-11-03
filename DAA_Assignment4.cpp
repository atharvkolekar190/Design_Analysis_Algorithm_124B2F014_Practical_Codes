#include <bits/stdc++.h>
using namespace std;

void dijkstra(int source, vector<vector<pair<int,int>>> &graph, vector<int> &dist, vector<int> &parent) {
    int V = graph.size();
    dist.assign(V, INT_MAX);
    parent.assign(V, -1);
    dist[source] = 0;

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto &edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;

            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

vector<int> reconstructPath(int source, int target, vector<int> &parent) {
    vector<int> path;
    for (int v = target; v != -1; v = parent[v])
        path.push_back(v);
    reverse(path.begin(), path.end());
    if (path[0] != source) path.clear(); // no valid path
    return path;
}

int main() {
    int V, E;
    cout << "Enter number of intersections (vertices): ";
    cin >> V;
    cout << "Enter number of roads (directed edges): ";
    cin >> E;

    vector<vector<pair<int,int>>> graph(V);

    cout << "Enter edges (u v w):\n";
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w}); // ✅ directed edge (u → v)
    }

    int source;
    cout << "Enter ambulance start location (source): ";
    cin >> source;

    int H;
    cout << "Enter number of hospitals: ";
    cin >> H;
    vector<int> hospitals(H);
    cout << "Enter hospital nodes: ";
    for (int i = 0; i < H; i++) {
        cin >> hospitals[i];
    }

    vector<int> dist, parent;
    dijkstra(source, graph, dist, parent);

    cout << "\n--- Distance from Source (" << source << ") to Each Hospital ---\n";
    for (int h : hospitals) {
        if (dist[h] == INT_MAX)
            cout << "Hospital at node " << h << ": Not reachable\n";
        else
            cout << "Hospital at node " << h << ": " << dist[h] << " minutes\n";
    }

    int minTime = INT_MAX, nearestHospital = -1;
    for (int h : hospitals) {
        if (dist[h] < minTime) {
            minTime = dist[h];
            nearestHospital = h;
        }
    }

    cout << "\n--------------------------------------------------\n";
    if (nearestHospital == -1) {
        cout << "No hospital reachable.\n";
    } else {
        cout << "Nearest hospital is at node " << nearestHospital 
             << " with travel time " << minTime << " minutes.\n";

        vector<int> path = reconstructPath(source, nearestHospital, parent);

        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i != path.size() - 1) cout << " -> ";
        }
        cout << "\n";
    }

    return 0;
}

