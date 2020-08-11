#include <iostream>
#include <vector>

using namespace std;

struct Vertex {
    int weight;
    vector <int> children;
};
typedef vector<Vertex> Graph;
typedef vector<int> Sum;

Graph ReadTree() {
    int vertices_count;
    cin >> vertices_count;
    Graph tree(vertices_count);
    for (int i = 0; i < vertices_count; ++i) {
        cin >> tree[i].weight;
    }
    for (int i = 1; i < vertices_count; ++i) {
        int from, to;
        cin >> from >> to;
        tree[from - 1].children.emplace_back(to - 1);
        tree[to - 1].children.emplace_back(from - 1);
    }
    return tree;
}

int dfs(const Graph &tree, int vertex, Sum &sum, int parent) {
    if (sum[vertex] == -1) {
        if (tree[vertex].children.empty()) {
            sum[vertex] = tree[vertex].weight;
        }
        else {
            int subTreeWeight = tree[vertex].weight;
            for (int node : tree[vertex].children) {
                if (node == parent) {
                    continue;
                }
                for (int subTreeNode : tree[node].children) {
                    if (subTreeNode != vertex) {
                        subTreeWeight += dfs(tree, subTreeNode, sum, node);
                    }
                }
            }
            int currentWeight = 0;
            for (int node : tree[vertex].children) {
                if (node != parent) {
                    currentWeight += dfs(tree, node, sum, vertex);
                }
            }
            sum[vertex] = max(currentWeight, subTreeWeight);
        }
    }
    return sum[vertex];
}

int MaxWeightIndependentTreeSubset(const Graph &tree) {
    size_t size = tree.size();
    if (size == 0) {
        return 0;
    }
    Sum sum = Sum(size, -1);
    return dfs(tree, 0, sum, -1);
}

int main() {
    Graph tree = ReadTree();
    cout << MaxWeightIndependentTreeSubset(tree) << endl;
    return 0;
}
