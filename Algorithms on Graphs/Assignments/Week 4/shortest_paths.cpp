#include <iostream>
#include <climits>
#include <vector>
#include <queue>

using namespace std;

void shortest_paths(vector<vector<int>> &adj, vector<vector<int>> &cost, int s, vector<long long> &distance, vector<int> &reachable, vector<int> &shortest) {
    queue<int> negativeCycle;
    reachable[s] = 1;
    distance[s] = 0;
    for (int iterator = 0; iterator < adj.size(); ++iterator) {
        bool unchanged = true;
        for (int x = 0; x < adj.size(); ++x) {
            for (int y : adj[x]) {
                int index = 0;
                for (int i = 0; i < adj.size(); ++i) {
                    if (adj[x][i] == y) {
                        index = i;
                        break;
                    }
                }
                if (distance[x] != LLONG_MAX && distance[y] > distance[x] + cost[x][index]) {
                    unchanged = false;
                    distance[y] = distance[x] + cost[x][index];
                    reachable[y] = 1;
                    if (iterator == adj.size() - 1) {
                        negativeCycle.push(y);
                        shortest[y] = 0;
                    }
                }
            }
        }
        if (unchanged) {
            break;
        }
    }
    for (int iterator = 0; iterator < adj.size(); ++iterator) {
        if (distance[iterator] < LLONG_MAX) {
            reachable[iterator] = 1;
        }
    }
    vector<bool> visited(adj.size(), false);
    while (!negativeCycle.empty()) {
        int current = negativeCycle.front();
        negativeCycle.pop();
        visited[current] = true;
        shortest[current] = 0;
        for (int node : adj[current]) {
            if (!visited[node]) {
                negativeCycle.push(node);
                visited[node] = true;
                shortest[node] = 0;
            }
        }
    }
    distance[s] = 0;
}

int main() {
    int n, m, s;
    cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    vector<vector<int> > cost(n, vector<int>());
    for (int i = 0; i < m; i++) {
        int x, y, w;
        cin >> x >> y >> w;
        adj[x - 1].push_back(y - 1);
        cost[x - 1].push_back(w);
    }
    cin >> s;
    --s;
    vector<long long> distance(n, LLONG_MAX);
    vector<int> reachable(n, 0);
    vector<int> shortest(n, 1);
    shortest_paths(adj, cost, s, distance, reachable, shortest);
    for (int i = 0; i < n; i++) {
        if (!reachable[i]) {
            cout << "*\n";
        } else if (!shortest[i]) {
            cout << "-\n";
        } else {
            cout << distance[i] << "\n";
        }
    }
}
