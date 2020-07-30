#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

struct Edge {
    int from;
    int to;
};

struct ConvertHampathToSat {
    int i, j;
    vector<Edge> graph;

    ConvertHampathToSat(int n, int m) :
        i(n),
        j(m),
        graph(m)
    {  }

    void printEquisatisfiableSatFormula() {
        string clauses, clauses2, clauses3, clauses4, clauses5;
        clauses.reserve(120000 * 3 + 1);
        clauses2.reserve(120000 * 3 + 1);
        clauses3.reserve(120000 * 3 + 1);
        clauses4.reserve(120000 * 3 + 1);
        clauses5.reserve(120000 * 3 + 1);
        int clauseCount = 0;
        int vertexCount = j;
        vector<vector<int>> edges(i, vector<int>(i, 0)), path(i, vector<int>(i));
        int x, y, z, counter = 0;
        for (x = 0; x < j; ++x) {
            edges[graph[x].from - 1][graph[x].to - 1] = 1;
            edges[graph[x].to - 1][graph[x].from - 1] = 1;
        }
        for (x = 0; x < vertexCount; ++x, ++clauseCount) {
            for (y = 0; y < vertexCount; ++y) {
                path[x][y] = ++counter;
                clauses += to_string(path[x][y]) + " ";
            }
            clauses += "0\n";
        }
        for (x = 0; x < vertexCount; ++x, ++clauseCount) {
            for (y = 0; y < vertexCount; ++y) {
                clauses3 += to_string(path[y][x]) + " ";
                for (z = x + 1; z < vertexCount; ++z, ++clauseCount) {
                    clauses2 += to_string(-path[x][y]) + " " + to_string(-path[z][y]) + " 0\n";
                }
                for (z = y + 1; z < vertexCount; ++z, ++clauseCount) {
                    clauses4 += to_string(-path[x][y]) + " " + to_string(-path[x][z]) + " 0\n";
                }
                if (!edges[x][y] && y != 1) {
                    for (z = 0; z < vertexCount - 1; ++z, ++clauseCount) {
                        clauses5 += to_string(-path[x][z]) + " " + to_string(-path[y][z + 1]) + " 0\n";
                    }
                }
            }
            clauses3 += "0\n";
        }
        clauses += clauses2 + clauses3 + clauses4 + clauses5;
        printf("%d %d \n%s", clauseCount, vertexCount * vertexCount, clauses.c_str());
    }
};

int main() {
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    ConvertHampathToSat converter(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> converter.graph[i].from >> converter.graph[i].to;
    }
    converter.printEquisatisfiableSatFormula();
    return 0;
}

//#include <iostream>
//#include <vector>
//#include <cstdio>
//
//using std::cin;
//using std::vector;
//using std::ios;
//
//struct ConvertHampathToSat {
//
//    ConvertHampathToSat(int n, int m)
//            : clauses_num(0)
//            , vertices_num(n)
//            , matrix(n, vector<bool>(n, false))
//            , data(n, vector<int>(n))
//    {
//        for (int i = 0, cnt = 0; i < vertices_num; ++i) {
//            for (int j = 0; j < vertices_num; ++j) {
//                data[i][j] = ++cnt;
//            }
//        }
//    }
//
//    void print_SAT_formula(const int max_clauses_num) noexcept
//    {
//        clauses_stream.reserve(max_clauses_num * 3);
//
//        each_vertext_in_path();
//        each_vertext_in_path_only_once();
//        each_path_position_occupied();
//        verteces_occupy_diff_positions();
//        nonadjacent_vertices_nonadjacent_in_path();
//
//        printf("%d %d \n%s", clauses_num, vertices_num * vertices_num, clauses_stream.c_str());
//    }
//
//    void each_vertext_in_path() noexcept
//    {
//        for (int i = 0; i < vertices_num; ++i, ++clauses_num) {
//            for (int j = 0; j < vertices_num; ++j) {
//                clauses_stream += std::to_string(data[i][j]) + " ";
//            }
//            clauses_stream += "0\n";
//        }
//    }
//
//    void each_vertext_in_path_only_once() noexcept
//    {
//        for (int i = 0; i < vertices_num; ++i) {
//            for (int j = 0; j < vertices_num; ++j) {
//                for (int k = i + 1; k < vertices_num; ++k, ++clauses_num) {
//                    clauses_stream += std::to_string(-data[i][j]) + " " + std::to_string(-data[k][j]) + " 0\n";
//                }
//            }
//        }
//    }
//
//    void each_path_position_occupied() noexcept
//    {
//        for (int i = 0; i < vertices_num; ++i, ++clauses_num) {
//            for (int j = 0; j < vertices_num; ++j) {
//                clauses_stream += std::to_string(data[j][i]) + " ";
//            }
//            clauses_stream += "0\n";
//        }
//    }
//
//    void verteces_occupy_diff_positions() noexcept
//    {
//        for (int i = 0; i < vertices_num; ++i) {
//            for (int j = 0; j < vertices_num; ++j) {
//                for (int k = j + 1; k < vertices_num; ++k, ++clauses_num) {
//                    clauses_stream += std::to_string(-data[i][j]) + " " + std::to_string(-data[i][k]) + " 0\n";
//                }
//            }
//        }
//    }
//
//    void nonadjacent_vertices_nonadjacent_in_path() noexcept
//    {
//        for (int i = 0; i < vertices_num; ++i) {
//            for (int j = 0; j < vertices_num; ++j) {
//                if (!matrix[i][j] && j != i) {
//                    for (int k = 0; k < vertices_num - 1; ++k, ++clauses_num) {
//                        clauses_stream += std::to_string(-data[i][k]) + " " + std::to_string(-data[j][k + 1]) + " 0\n";
//                    }
//                }
//            }
//        }
//    }
//
//    unsigned int clauses_num;
//    const unsigned int vertices_num;
//    vector<vector<bool> > matrix;
//    vector<vector<int> > data;
//    std::string clauses_stream;
//};
//
//int main()
//{
//    ios::sync_with_stdio(false);
//
//    int n, m;
//    cin >> n >> m;
//
//    ConvertHampathToSat converter(n, m);
//
//    for (int k = 0; k < m; ++k) {
//        int i, j;
//        cin >> i >> j;
//        converter.matrix[i - 1][j - 1] = true;
//        converter.matrix[j - 1][i - 1] = true;
//    }
//
//    converter.print_SAT_formula(120000);
//
//    return 0;
//}
