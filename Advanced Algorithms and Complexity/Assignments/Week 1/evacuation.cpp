#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using std::vector;
using std::queue;

/* This class implements a bit unusual scheme for storing edges of the graph,
 * in order to retrieve the backward edge for a given edge quickly. */
class FlowGraph {
public:
    struct Edge {
        int from, to, capacity, flow;
    };

private:
    /* List of all - forward and backward - edges */
    vector<Edge> edges;

    /* These adjacency lists store only indices of edges in the edges list */
    vector<vector<size_t> > graph;

public:
    explicit FlowGraph(size_t n, size_t m): graph(n) {
        edges.reserve(m * 2);  // Presize vector with edge count * 2 for both edge directions
    }

    void add_edge(int from, int to, int capacity) {
        /* Note that we first append a forward edge and then a backward edge,
         * so all forward edges are stored at even indices (starting from 0),
         * whereas backward edges are stored at odd indices in the list edges */
        Edge forward_edge = {from, to, capacity, 0};
        Edge backward_edge = {to, from, 0, 0};
        graph[from].push_back(edges.size());
        edges.push_back(forward_edge);
        graph[to].push_back(edges.size());
        edges.push_back(backward_edge);
    }

    size_t size() const {
        return graph.size();
    }

    const vector<size_t>& get_ids(int from) const {
        return graph[from];
    }

    const Edge& get_edge(size_t id) const {
        return edges[id];
    }

    void add_flow(size_t id, int flow) {
        /* To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
         * due to the described above scheme. On the other hand, when we have to get a "backward"
         * edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
         * should be taken.
         *
         * It turns out that id ^ 1 works for both cases. Think this through! */
        edges[id].flow += flow;
        edges[id ^ 1].flow -= flow;
    }
};

FlowGraph read_data() {
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    FlowGraph graph(vertex_count, edge_count);
    for (int i = 0; i < edge_count; ++i) {
        int u, v, capacity;
        std::cin >> u >> v >> capacity;
        graph.add_edge(u - 1, v - 1, capacity);
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

int max_flow(FlowGraph& graph, int from, int to) {
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
            for (int id = previousNodes[to]; id != -1; id = previousNodes[graph.get_edge(id).from]) {
                graph.add_flow(id, minFlow);
            }
            flow += minFlow;
        }
    } while (previousNodes[to] != -1);
    return flow;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    FlowGraph graph = read_data();

    std::cout << max_flow(graph, 0, int(graph.size() - 1)) << "\n";
    return 0;
}
