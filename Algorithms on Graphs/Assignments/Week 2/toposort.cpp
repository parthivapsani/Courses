#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

void dfs(vector<vector<int>> &adj, vector<bool> &visited, vector<int> &order, int vertex) {
    visited[vertex] = true;
    for (int node : adj[vertex]) {
        if (!visited[node]) {
            dfs(adj, visited, order, node);
        }
    }
    order.emplace_back(vertex);
}

vector<int> toposort(vector<vector<int>>& adj) {
    vector<bool> visited(adj.size(), false);
    vector<int> order;
    for (int node = 0; node < adj.size(); ++node) {
        if (!visited[node]) {
            dfs(adj, visited, order, node);
        }
    }
    reverse(order.begin(), order.end());
    return order;
}

int main() {
    size_t n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n, vector<int>());
    for (size_t i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj[x - 1].push_back(y - 1);
    }
    vector<int> order = toposort(adj);
    for (int i : order) {
        cout << i + 1 << " ";
    }
}
