#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void bfs(vector<vector<int>> &adj, vector<bool> &visited, int start) {
    visited[start] = true;
    queue<int> bfs;
    bfs.push(start);
    while (!bfs.empty()) {
        int current = bfs.front();
        bfs.pop();
        for (int node : adj[current]) {
            if (!visited[node]) {
                visited[node] = true;
                bfs.push(node);
            }
        }
    }
}

int number_of_components(vector<vector<int>> &adj, int size) {
    int connectedComponents = 0;
    vector<bool> visited(size, false);
    for (int x = 0; x < size; ++x) {
        if (!visited[x]) {
            bfs(adj, visited, x);
            ++connectedComponents;
        }
    }
    return connectedComponents;
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
    cout << number_of_components(adj, n);
}
