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
    vector<vector<size_t>> graph;
    int flights;

public:
    explicit FlowGraph(size_t n, size_t m, int f): graph(n), flights(f) {
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

    int get_flight_count() {
        return flights;
    };

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


FlowGraph read_data()
{
    int flightCount, crewCount;
    std::cin >> flightCount >> crewCount;

    FlowGraph graph(flightCount + crewCount + 2, flightCount + crewCount + 2, flightCount);
    for(int i = 0; i < flightCount; ++i) {
        graph.add_edge(0, i + 1, 1);
    }

    for (int i = 0; i < flightCount; ++i) {
        for(int j = 0; j < crewCount; ++j) {
            int available;
            std::cin >> available;
            if(available) {
                graph.add_edge(i + 1, flightCount + j + 1, 1);
            }
        }
    }

    for(int i = flightCount; i < crewCount + flightCount; ++i) {
        graph.add_edge(i + 1, flightCount + crewCount + 1, 1);
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
            if (minFlow) {
                for (int id = previousNodes[to]; id != -1; id = previousNodes[graph.get_edge(id).from]) {
                    graph.add_flow(id, minFlow);
                }
            }
            flow += minFlow;
        }
    } while (previousNodes[to] != -1);
    return flow;
}

void maximumBipartiteMatching(FlowGraph& graph, const size_t flights)
{
    for(int i = 0; i < flights; ++i) {
        int assignedCrew = -1;
        for(auto id: graph.get_ids(i+1)) {
            const FlowGraph::Edge &edge = graph.get_edge(id);
            if(edge.flow == 1) {
                assignedCrew = edge.to - flights;
                break;
            }
        }
        cout << assignedCrew << " ";
    }
    cout << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    FlowGraph graph = read_data();

    max_flow(graph, 0, graph.size() - 1);
    maximumBipartiteMatching(graph, graph.get_flight_count());
    return 0;
}