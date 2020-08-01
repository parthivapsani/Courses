//#include <iostream>
//#include <vector>
//#include <unordered_map>
//#include <stack>
//#include <limits>
//
//using namespace std;
//
//const int MIN = numeric_limits<int>::min();
//
//struct TwoSatisfiability {
//    struct Clause {
//        Clause():
//            index(MIN),
//            link(MIN),
//            id(-1),
//            inStack(false)
//        {}
//        int index, id, link;
//        vector<int> edges;
//        bool inStack;
//    };
//
//    int numVars, top = 0, vertexID = 0, t = 0;
//    bool satisfiable = true;
//    stack<int> edgeStack;
//    vector<Clause> clauses;
//    unordered_map<int, Clause> adjacencyMatrix;
//
//    TwoSatisfiability(int n, int m) :
//        numVars(n),
//        clauses(m),
//        adjacencyMatrix(n * 2)
//    {}
//
//    void processVertex(int vertexIndex, vector<bool> &result) {
//        Clause &clause = adjacencyMatrix[vertexIndex];
//        clause.link = clause.index = t++;
//        clause.inStack = true;
//        edgeStack.push(vertexIndex);
//        for (int w : clause.edges) {
//            if (adjacencyMatrix[w].index == MIN) {
//                processVertex(w, result);
//                clause.link = min(clause.link, adjacencyMatrix[w].link);
//            }
//            else if (adjacencyMatrix[w].inStack) {
//                clause.link = min(clause.link, adjacencyMatrix[w].index);
//            }
//        }
//        if (clause.link == clause.index) {
//            do {
//                top = edgeStack.top();
//                edgeStack.pop();
//                if (vertexID == adjacencyMatrix[-top].id) {
//                    satisfiable = false;
//                    return;
//                }
//                Clause &currentClause = adjacencyMatrix[top];
//                currentClause.inStack = false;
//                currentClause.id = vertexID;
//                result[abs(top) - 1] = top < 0;
//            } while (top != vertexIndex);
//            ++vertexID;
//        }
//    }
//
//    bool isSatisfiable(vector<bool> &result) {
//        for (int i = -numVars; i <= numVars; ++i) {
//            if (adjacencyMatrix[i].index == MIN && i != 0) {
//                processVertex(i, result);
//            }
//        }
//        return satisfiable;
//    }
//};
//
//int main() {
//    ios::sync_with_stdio(false);
//    int n, m, a, b;
//    cin >> n >> m;
//    TwoSatisfiability twoSat(n, m);
//    for (int i = 0; i < m; ++i) {
//        cin >> a >> b;
//        twoSat.adjacencyMatrix[-a].edges.emplace_back(b);
//        twoSat.adjacencyMatrix[-b].edges.emplace_back(a);
//    }
//    vector<bool> result(n, false);
//    if (twoSat.isSatisfiable(result)) {
//        cout << "SATISFIABLE" << endl;
//        for (int i = 1; i <= result.size(); ++i) {
//            if (result[i - 1]) {
//                cout << -i;
//            }
//            else {
//                cout << i;
//            }
//            if (i < n) {
//                cout << " ";
//            }
//            else {
//                cout << endl;
//            }
//        }
//    }
//    else {
//        cout << "UNSATISFIABLE" << endl;
//    }
//    return 0;
//}

struct solver_2SAT {
    struct util {
        util()
                : index{ MIN }
                , lowlink{ MIN }
                , id{ -1 }
                , on_stack{ false }
        {
        }
        int index, lowlink, id;
        bool on_stack;
        std::vector<int> edges;
    };

    solver_2SAT(std::unordered_map<int, util> adj_lst_graph, int num_of_vars)
            : n{ num_of_vars }
            , graph{ std::move(adj_lst_graph) }
            , sol{ std::vector<bool>(n, false) }
    {
    }

    void process_vertex(int v)
    {
        auto& utl = graph[v];

        utl.lowlink = utl.index = t++;
        utl.on_stack = true;
        stk.push(v);

        for (auto w : utl.edges) {
            if (graph[w].index == MIN) {
                process_vertex(w);
                utl.lowlink = std::min(utl.lowlink, graph[w].lowlink);
            }
            else if (graph[w].on_stack) {
                utl.lowlink = std::min(utl.lowlink, graph[w].index);
            }
        }

        if (utl.lowlink == utl.index) {
            do {
                w = stk.top();
                stk.pop();

                if (id == graph[-w].id) {
                    sat = false;
                    return;
                }

                auto& w_utl = graph[w];
                w_utl.on_stack = false;
                w_utl.id = id;

                sol[abs(w) - 1] = w < 0 ? true : false;

            } while (w != v);
            ++id;
        }
    }

    void solve()
    {
        for (int i = -n; i <= n; ++i) {
            if (graph[i].index == MIN && i != 0) {
                process_vertex(i);
            }
        }
    }

    void print_solution() const noexcept
    {
        if (!sat) {
            printf("%s\n", "UNSATISFIABLE\0");
            return;
        }

        printf("%s\n", "SATISFIABLE\0");
        for (unsigned int i = 0, s = sol.size(); i < s; ++i) {
            printf("%d ", (sol[i] ? i + 1 : -(i + 1)));
        }

        printf("\n");
    }

    int t{ 0 }, id{ 0 }, w{ 0 }, n{ 0 };

    std::unordered_map<int, util> graph;
    std::vector<bool> sol;
    std::stack<int> stk;

    bool sat{ true };

    static constexpr int MIN = std::numeric_limits<int>::min();
};

int main()
{
    std::ios::sync_with_stdio(false);

    int n, m;
    std::cin >> n >> m;

    std::unordered_map<int, solver_2SAT::util> adj_lst_graph(n * 2);

    for (int i = 0, x, y; i < m; ++i) {
        std::cin >> x >> y;
        adj_lst_graph[-x].edges.emplace_back(y);
        adj_lst_graph[-y].edges.emplace_back(x);
    }

    solver_2SAT solver(std::move(adj_lst_graph), n);
    solver.solve();
    solver.print_solution();
}
