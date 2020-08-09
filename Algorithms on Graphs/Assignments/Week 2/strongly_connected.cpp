#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> reverseGraph(const vector<vector<int>> &graph) {
    vector<vector<int>> reversedGraph(graph.size(), vector<int>());
    for (int node = 0; node < graph.size(); ++node) {
        for (int vertex : graph[node]) {
            reversedGraph[vertex].emplace_back(node);
        }
    }
    return reversedGraph;
}

void dfs(vector<vector<int>> &adj, vector<bool> &visited, vector<int> &order, int vertex, bool append) {
    visited[vertex] = true;
    for (int node : adj[vertex]) {
        if (!visited[node]) {
            dfs(adj, visited, order, node, append);
        }
    }
    if (append) {
        order.emplace_back(vertex);
    }
}

int stronglyConnectedComponentsCount(vector<vector<int>> &adj) {
    int result = 0;
    vector<bool> visited(adj.size(), false);
    vector<int> order;
    for (int node = 0; node < adj.size(); ++node) {
        if (!visited[node]) {
            dfs(adj, visited, order, node, true);
        }
    }
    adj = reverseGraph(adj);
    fill(visited.begin(), visited.end(), false);
    while (!order.empty()) {
        int node = order.back();
        order.pop_back();
        if (!visited[node]) {
            dfs(adj, visited, order, node, false);
            ++result;
        }
    }
    return result;
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
    cout << stronglyConnectedComponentsCount(adj);
}
