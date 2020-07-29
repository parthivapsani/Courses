#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <functional>
#include <limits>
#include <valarray>

const long double EPS = std::numeric_limits<long double>::epsilon();

using namespace std;

using Row = vector<long double>;
using Matrix = vector<Row>;

struct Position {
    short row;
    short column;
    bool optimal() const { return row == -1 || column == -1; }
};

struct simplex {
    Position findPivot(Row& row) {
        int x = 0, y = distance(row.begin(), min_element(row.begin(), row.end()));
        long double ratio = numeric_limits<long double>::max() - 1;
        if (row[y] >= 0.0) {
            return {-1, -1};
        }
        for (int x2 = 0; x2 < A.size(); ++x2) {
            long double newRatio = b[x2] / A[x2][y];
            if (A[x2][y] && newRatio - ratio < EPS && newRatio > 0.0) {
                ratio = newRatio;
                x = x2;
            }
        }
        if (ratio == numeric_limits<long double>::max() - 1) {
            unbounded = true;
        }
        return {static_cast<short>(x), static_cast<short>(y)};
    }

    void processPivot(Position p, Row& row, const bool phase) {
        long double scalar = A[p.row][p.column];
        b[p.row] /= scalar;
        for (long double &val : A[p.row]) {
            val /= scalar;
        }
        for (int x = 0, s = A.size(); x < s; ++x) {
            if (p.row != x && abs(A[x][p.column] - 0.0) >= EPS) {
                scalar = A[x][p.column];
                for (int y = 0; y < A[x].size(); ++y) {
                    A[x][y] -= A[p.row][y] * scalar;
                }
                b[x] -= b[p.row] * scalar;
            }
        }
        if (phase) {
            b[b.size() - 2] -= b[p.row] * c[p.column];
            b[b.size() - 1] -= b[p.row] * row[p.column];
            long double mScalar = row[p.column];
            long double cScalar = c[p.column];
            for (int y = 0; y < possibles.size(); ++y) {
                row[y] -= A[p.row][y] * mScalar;
                c[y] -= A[p.row][y] * cScalar;
            }
        }
        else {
            b[b.size() - 1] -= b[p.row] * c[p.column];
            long double cScalar = c[p.column];
            for (int y = 0; y < possibles.size(); ++y) {
                c[y] -= A[p.row][y] * cScalar;
            }
        }
    }

    void reducedRowEchelonForm(Row& obj, const bool phase) {
        Position p = findPivot(obj);
        while (!(p.optimal() || unbounded)) {
            possibleSolutions[p.column] = p.row;
            processPivot(p, obj, phase);
            p = findPivot(obj);
        }
    }

    pair<int, Row> solve() {
        unbounded = false;
        possibleSolutions = vector<int>(A[0].size(), -1);
        transform(c.begin(), c.end(), c.begin(), negate<double>());
        for (int x = 0; x < A.size(); ++x) {
            A[x][x + m] = 1;
        }
        if (b.end() != find_if(b.begin(), b.end(), [](long double val) {return val < 0.0;})) { // Check if values are negative
            possibles = Row(c.size());
            for (int x = 0, y = m + n; x < b.size() - 1; ++x, ++y) {
                if (b[x] < 0.0) {
                    possibleSolutions[x] = -2;
                    A[x][y] = -1;
                    b.back() += b[x];
                    b[x] *= -1;
                    transform(A[x].begin(), A[x].end(), A[x].begin(), negate<long double>());
                    for (int y2 = 0; y2 < n + m; ++y2) {
                        possibles[y2] += A[x][y2];
                    }
                }
            }
            transform(possibles.begin(), possibles.end(), possibles.begin(), negate<long double>());
            reducedRowEchelonForm(possibles, true);
            if (abs(b.back() - 0.0) >= 0.001) {  // Infeasible
                return {-1, {}};
            }
        }
        c.resize(c.size() - n);
        b.pop_back();
        for (Row &r : A) {
            r.resize(r.size() - n);
        }
        reducedRowEchelonForm(c, false);
        if (unbounded) {
            return {1, {}};
        }
        Row result(m);
        for (int y = 0; y < m; ++y) {
            long double sum = 0.0;
            int index = 0;
            for (int x = 0; x < A.size(); ++x) {
                if (possibleSolutions[x] >= 0.0) {
                    sum += fabs(A[x][y]);
                }
                if (abs(A[x][y] - 1.0) < 0.001) {
                    index = x;
                }
            }
            result[y] = (sum - 1.0 > EPS) ? 0.0 : b[index];
        }
        return {0, move(result)};
    }

    int n, m;
    Matrix A;
    Row b, c, possibles;
    vector<int> possibleSolutions;
    bool unbounded;
};

int main() {
    ios_base::sync_with_stdio(false);
    int n, m, x;
    cin >> n >> m;
    Matrix A(n, Row (2 * n + m, 0.0));
    for (x = 0; x < n; ++x) {
        for (int y = 0; y < m; ++y) {
            cin >> A[x][y];
        }
    }
    Row b(n + 2);
    for (x = 0; x < n; ++x) {
        cin >> b[x];
    }
    Row c(2 * n + m);
    for (x = 0; x < m; ++x) {
        cin >> c[x];
    }
    
    simplex instance{n, m, move(A), move(b), move(c)};
    pair<int, Row> result = instance.solve();

    switch (result.first) {
        case -1:
            printf("No solution\n");
            break;
        case 0:
            printf("Bounded solution\n");
            for (int i = 0; i < m; i++) {
                printf("%.18Lf%c", result.second[i], " \n"[i + 1 == m]);
            }
            break;
        case 1:
            printf("Infinity\n");
            break;
    }
    return 0;
}
