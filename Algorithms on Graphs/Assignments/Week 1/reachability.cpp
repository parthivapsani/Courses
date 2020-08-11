#include <iostream>
#include <vector>
#include <queue>

using namespace std;


int reach(vector<vector<int>> &adj, int x, int y) {
    vector<bool> visited(adj.size(), false);
    visited[x] = true;
    queue<int> bfs;
    bfs.push(x);
    while (!bfs.empty()) {
        int current = bfs.front();
        bfs.pop();
        if (current == y) {
            break;
        }
        for (int node : adj[current]) {
            if (!visited[node]) {
                visited[node] = true;
                bfs.push(node);
            }
        }
    }
    return visited[y] ? 1 : 0;
}

int main() {
    size_t n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n, vector<int>());
    for (size_t i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj[x - 1].push_back(y - 1);
        adj[y - 1].push_back(x - 1);
    }
    int x, y;
    cin >> x >> y;
    cout << reach(adj, x - 1, y - 1);
}
