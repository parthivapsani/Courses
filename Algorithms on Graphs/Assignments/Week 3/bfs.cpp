#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int distance(vector<vector<int> > &adj, int s, int t, int vertices) {
    vector<int> distance(vertices, -1);
    distance[s] = 0;
    queue<int> bfs;
    bfs.push(s);
    while (!bfs.empty()) {
        int current = bfs.front();
        bfs.pop();
        for (int node : adj[current]) {
            if (distance[node] == -1) {
                distance[node] = distance[current] + 1;
                if (node == t) {
                    return distance[node];
                }
                bfs.push(node);
            }
        }
    }
    return -1;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj[x - 1].push_back(y - 1);
        adj[y - 1].push_back(x - 1);
    }
    int s, t;
    cin >> s >> t;
    --s, --t;
    cout << distance(adj, s, t, n);
}
