#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using std::vector;
using std::queue;
using std::cin;
using std::cout;


class FlowGraph {
public:
    struct Edge {
        int from, to, capacity, flow;
    };

private:
    vector<Edge> edges;
    vector<vector<int>> graph;
    int stockCount;
public:
    explicit FlowGraph(int n): graph(n * 2 + 2), stockCount(n) {
        edges.reserve(n + 1);
    }

    void add_edge(int from, int to, int capacity) {
        Edge forward_edge = {from, to, capacity, 0};
        Edge backward_edge = {to, from, 0, 0};
        graph[from].push_back(edges.size());
        edges.push_back(forward_edge);
        graph[to].push_back(edges.size());
        edges.push_back(backward_edge);
    }

    int size() const {
        return graph.size();
    }

    const vector<int>& get_ids(int from) const {
        return graph[from];
    }

    const Edge& get_edge(int id) const {
        return edges[id];
    }

    void add_flow(int id, int flow) {
        edges[id].flow += flow;
        edges[id ^ 1].flow -= flow;
    }

    int getStockCount() const {
        return stockCount;
    }
};


FlowGraph read_data()
{
    int stockCount, pointCount;
    std::cin >> stockCount >> pointCount;
    vector<vector<int>> stockPoints(stockCount, vector<int>(pointCount));
    for (int i = 0; i < stockCount; ++i) {
        for (int j = 0; j < pointCount; ++j) {
            cin >> stockPoints[i][j];
        }
    }
    FlowGraph graph(stockCount);
    for(int i = 1; i <= stockCount; ++i) {
        graph.add_edge(0, i, 1);
    }
    for (int i = 0; i < stockCount; ++i) {
        for (int j = 0; j < stockCount; ++j) {
            if (i != j) {
                bool compatible = true;
                for (int k = 0; k < pointCount; ++k) {
                    if (stockPoints[i][k] <= stockPoints[j][k]) {
                        compatible = false;
                        break;
                    }
                }
                if (compatible) {
                    graph.add_edge(i + 1, stockCount + j + 1, 1);
                }
            }
        }
    }
    for (int i = stockCount + 1; i <= stockCount * 2; ++i) {
        graph.add_edge(i, stockCount * 2 + 1, 1);
    }
    return graph;
}

bool BFS(FlowGraph& graph, int source, int sink, vector<int>& previousNodes) {
    // BFS for Edmonds-Karp
    queue<int> explored;
    explored.push(source);
    bool sourceAndSink = false;
    std::fill(previousNodes.begin(), previousNodes.end(), -1);
    while(!explored.empty()) {
        int current = explored.front();
        explored.pop();
        for (u_long id : graph.get_ids(current)) {
            FlowGraph::Edge edge = graph.get_edge(id);
            if (previousNodes[edge.to] == -1 && edge.capacity > edge.flow && edge.to != source) {
                previousNodes[edge.to] = id;
                if (edge.to == sink) {
                    sourceAndSink = true;
                }
                explored.push(edge.to);
            }
        }
    }
    return sourceAndSink;
}

void max_flow(FlowGraph& graph, int from, int to) {
    int flow = 0;
    vector<int> previousNodes(graph.size());
    // Run BFS while the graph has not been fully searched
    do {
        if(BFS(graph, from, to, previousNodes)) {
            int minFlow = INT_MAX;  // Max value for comparison until reassigned
            // Get minimum flow from the smallest cut that hasn't already been traversed
            for (int id = previousNodes[to]; id != -1; id = previousNodes[graph.get_edge(id).from]) {
                minFlow = std::min(minFlow, graph.get_edge(id).capacity - graph.get_edge(id).flow);
            }
            // Update the flow and residual graph
            if (minFlow) {
                for (int id = previousNodes[to]; id != -1; id = previousNodes[graph.get_edge(id).from]) {
                    graph.add_flow(id, minFlow);
                }
            }
            flow += minFlow;
        }
    } while (previousNodes[to] != -1);
}

int maximumBipartiteMatching(FlowGraph& graph)
{
    int graphCount = 0;
    for (int i = 1; i <= graph.getStockCount(); ++i) {
        for (auto id: graph.get_ids(i)) {
            const FlowGraph::Edge& edge = graph.get_edge(id);
            if (edge.flow > 0) {
                ++graphCount;
                break;
            }
        }
    }
    return graph.getStockCount() - graphCount;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    FlowGraph graph = read_data();
    max_flow(graph, 0, graph.size() - 1);
    cout << maximumBipartiteMatching(graph) << std::endl;
    return 0;
}
