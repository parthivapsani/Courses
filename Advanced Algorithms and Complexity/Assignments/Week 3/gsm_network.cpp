#include <ios>
#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int from;
    int to;
};

struct ConvertGSMNetworkProblemToSat {
    const int frequencyCount = 3;
    int numVertices;
    vector<Edge> edges;

    ConvertGSMNetworkProblemToSat(int n, int m) :
        numVertices(n),
        edges(m)
    {  }

    void printEquisatisfiableSatFormula() const {
        printf("%lu %d\n", frequencyCount * edges.size() + numVertices, frequencyCount * numVertices);  // Classes and variables
        for (int x = 0, bandCount = 1; x < numVertices; ++x, bandCount += frequencyCount) {
            printf("%d %d %d 0\n", bandCount, bandCount + 1, bandCount + 2);
        }
        for (const Edge edge : edges) {
            for (int x = 1; x <= frequencyCount; ++x) {
                printf("%d %d 0\n", -((edge.from - 1) * frequencyCount + x), -((edge.to - 1) * frequencyCount + x));
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    ConvertGSMNetworkProblemToSat converter(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> converter.edges[i].from >> converter.edges[i].to;
    }
    converter.printEquisatisfiableSatFormula();
    return 0;
}
