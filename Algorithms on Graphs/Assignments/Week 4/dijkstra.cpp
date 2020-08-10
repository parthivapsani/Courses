#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

typedef vector<vector<int>> Matrix;
typedef pair<int, long long> Path;

const long long inf = LLONG_MAX;

struct comparator {
    bool operator()(Path a, Path b) {
        return a.first > b.first;
    }
};

long long distance(Matrix &adj, Matrix &cost, int s, int t) {
    vector<long long> dist(adj.size(), inf);
    priority_queue<Path, vector<Path>, comparator> dijkstra;
    dist[s] = 0;
    dijkstra.push(Path(dist[s], s));
    while(!dijkstra.empty()) {
        Path current = dijkstra.top();
        dijkstra.pop();
        int index = current.second;
        for (int node = 0; node < adj[index].size(); node++) {
            int vertex = adj[index][node];
            if(dist[vertex] > dist[index] + cost[index][node]) {
                dist[vertex] = dist[index] + cost[index][node];
                dijkstra.push(Path(dist[vertex], vertex));
            }
        }
    }
    return dist[t] == inf ? -1 : dist[t];
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
    int s, t;
    cin >> s >> t;
    --s, --t;
    cout << distance(adj, cost, s, t);
}
