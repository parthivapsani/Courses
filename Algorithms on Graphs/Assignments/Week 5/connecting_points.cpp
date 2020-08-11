#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;
typedef pair<int, int> node;

struct edge {
    int u;
    int v;
    double weight;

    edge(int a, int b, double c) {
        u = a;
        v = b;
        weight = c;
    }
};

bool cmp(edge a, edge b) {
    return a.weight < b.weight;
}

double weight(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int Find(int i, vector<node> &nodes) {
    if (i != nodes[i].first) {
        nodes[i].first = Find(nodes[i].first, nodes);
    }
    return nodes[i].first;
}

double minimum_distance(vector<int> x, vector<int> y) {
    double result = 0.;
    int n = x.size();
    vector<node> nodes(n, node(0, 0));
    for (int i = 0; i < n; i++) {
        nodes[i] = node(i, 0);
    }
    vector<edge> edges;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            edges.emplace_back(i, j, weight(x[i], y[i], x[j], y[j]));
        }
    }
    sort(edges.begin(), edges.end(), cmp);
    for (auto current_edge : edges) {
        int r1 = Find(current_edge.u, nodes);
        int r2 = Find(current_edge.v, nodes);
        if (r1 != r2) {
            result += current_edge.weight;
            if (nodes[r1].second > nodes[r2].second) {
                nodes[r2].first = r1;
            }
            else {
                nodes[r1].first = r2;
                if (nodes[r1].second == nodes[r2].second) {
                    ++nodes[r2].second;
                }
            }
        }
    }
    return result;
}

int main() {
    size_t n;
    cin >> n;
    vector<int> x(n), y(n);
    for (size_t i = 0; i < n; i++) {
        cin >> x[i] >> y[i];
    }
    cout << setprecision(10) << minimum_distance(x, y) << endl;
}
