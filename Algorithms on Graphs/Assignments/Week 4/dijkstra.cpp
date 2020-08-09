#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef vector<vector<int>> Matrix;

long long distance(Matrix &adj, Matrix &cost, int s, int t) {
    //write your code her
    return -1;
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
    s--, t--;
    cout << distance(adj, cost, s, t);
}
