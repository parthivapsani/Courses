#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int bipartite(vector<vector<int>> &adj) {
    vector<int> color(adj.size(), 0);
    vector<bool> visited(adj.size(), false);
    queue<int> bfs;
    color[0] = 1;
    visited[0] = true;
    bfs.push(0);
    while (!bfs.empty()) {
        int current = bfs.front();
        bfs.pop();
        for (int node : adj[current]) {
            if (!visited[node]) {
                visited[node] = true;
                color[node] = 1 - color[current];
                bfs.push(node);
            }
            if (color[node] == color[current]) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n, vector<int>());
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj[x - 1].push_back(y - 1);
        adj[y - 1].push_back(x - 1);
    }
    cout << bipartite(adj);
}
