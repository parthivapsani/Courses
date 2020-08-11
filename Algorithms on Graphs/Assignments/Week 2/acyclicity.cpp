#include <iostream>
#include <vector>

using namespace std;

bool cyclic(int vertex, vector<bool> &visited, vector<bool> &callStack, vector<vector<int>> &adj) {
    if (!visited[vertex]) {
        visited[vertex] = true;
        callStack[vertex] = true;
        for (int node : adj[vertex]) {
            if ((!visited[node] && cyclic(node, visited, callStack, adj)) || callStack[node]) {
                return true;
            }
        }
    }
    callStack[vertex] = false;
    return false;
}

int acyclic(vector<vector<int>> &adj) {
    vector<bool> visited(adj.size(), false);
    vector<bool> callStack(adj.size(), false);
    for (int vertex = 0; vertex < adj.size(); ++vertex) {
        if (cyclic(vertex, visited, callStack, adj)) {
            return 1;
        }
    }
    return 0;
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
    cout << acyclic(adj);
}
