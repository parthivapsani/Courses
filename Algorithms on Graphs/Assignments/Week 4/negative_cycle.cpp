#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<int>> Matrix;

int bellmanFord(Matrix &adj, Matrix &cost, const int vertices) {
    vector<int> distance(vertices, 0);
    distance[0] = 0;
    for (int iterator = 0; iterator < vertices; ++iterator) {
        for (int x = 0; x < vertices; ++x) {
            vector<int> current = adj[x];
            for (int y : current) {
                int index = 0;
                for (int i = 0; i < vertices; ++i) {
                    if (current[i] == y) {
                        index = i;
                        break;
                    }
                }
                if (distance[y] > distance[x] + cost[x][index]) {
                    distance[y] = distance[x] + cost[x][index];
                    if (iterator == vertices - 1) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int main() {
    int n, m;
    cin >> n >> m;
    Matrix adj(n, vector<int>());
    Matrix cost(n, vector<int>());
    for (int i = 0; i < m; i++) {
        int x, y, w;
        cin >> x >> y >> w;
        adj[x - 1].push_back(y - 1);
        cost[x - 1].push_back(w);
    }
    cout << bellmanFord(adj, cost, n);
}
