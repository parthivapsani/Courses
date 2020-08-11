#include <ios>
#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
#include <algorithm>

using namespace std;

using Row = vector<int>;

struct ConvertILPToSat {
    vector<Row> A;
    Row b;

    ConvertILPToSat(int n, int m) : A(n, vector<int>(m)), b(n)
    {}

    void printEquisatisfiableSatFormula() const {
        int counter = 0, sum, cIndex;
        bitset<3> combinations;
        string clauses;
        for (int x = 0; x < A.size(); ++x) {
            const Row &row = A[x];
            int exp = count_if(row.begin(), row.cend(), [](const int val) {return val != 0;});
            for (int y = 0, bound = pow(2, exp); y < bound; ++y) {
                combinations = y;
                sum = 0, cIndex = 0;
                for (const int val : row) {
                    if (val != 0 && combinations[cIndex++]) {
                        sum += val;
                    }
                }
                if (sum > b[x]) {
                    bool clause = false;
                    cIndex = 0;
                    for (int z = 0; z < row.size(); ++z) {
                        if (row[z] != 0) {
                            clauses += combinations[cIndex++] ? (to_string(-(z + 1)) + " ") : (to_string(z + 1) + " ");
                            clause = true;
                        }
                    }
                    if (clause) {
                        clauses += "0 \n";
                        ++counter;
                    }
                }
            }
        }
        if (counter == 0) {
            ++counter;
            clauses += "1 -1 0\n";
        }
        printf("%d %lu\n%s", counter, A[0].size(), clauses.c_str());
    }
};

int main() {
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    ConvertILPToSat converter(n, m);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        cin >> converter.A[i][j];
      }
    }
    for (int i = 0; i < n; i++) {
      cin >> converter.b[i];
    }
    converter.printEquisatisfiableSatFormula();
    return 0;
}
