//#include <algorithm>
//#include <iostream>
//#include <vector>
//#include <cstdio>
//#include <cstdlib>
//#include <cmath>
//
//using namespace std;
//
//const long double precision = 1.0e-3;
//const long double upperBound = 1.0e+9;
//
//typedef vector<long double> Row;
//typedef vector<Row> Matrix;
//
//struct Position {
//    int column;
//    int row;
//    bool optimal() const {return row == -1 || column == -1;}
//};
//
//Position findPivot(Row& row, Matrix& A, Row& b) {
//    int x = 0;
//    int y = distance(row.begin(), min_element(row.begin(), row.end()));
//    long double ratio = numeric_limits<long double>::max() - 1;
//    if (row[y] >= 0.0) {
//        return {-1, -1};
//    }
//    for (int i = 0; i < A.size(); ++i) {
//        long double currentRatio = b[i] / A[i][y];
//        if (A[i][y] && ratio - currentRatio < precision && currentRatio > 0.0) {
//            ratio = currentRatio;
//            x = i;
//        }
//    }
//    if (ratio == numeric_limits<long double>::max() - 1) {
//        cout << "Line 37 unbounded" << endl;
//        return {INT_MAX, -INT_MAX};
//    }
//    return {x, y};
//}
//
//Position processPivot(Position p, Row& row, Matrix& A, Row& b, Row& c, Row& feasible, const bool feasibilityCheck) {
//    // Scale pivot by dividing
//    cout << "Start scaling" << endl;
//    cout << p.row << "  " << p.column << endl;
//    auto divisor = A[p.row][p.column];
//    cout << "Divisor scaling" << endl;
//    b[p.row] /= divisor;
//    cout << "B scaling" << endl;
//    cout << divisor << endl;
//    for (auto& val : A[p.row]) {
//        cout << val << "  " << divisor << endl;
//        val /= divisor;
//    }
//    cout << "Finish scaling" << endl;
//    // Process pivot
//    long double scalar;
//    for (int x = 0; x < A.size(); ++x) {
//        if (p.row != x && abs(A[x][p.column] - 0.0) < precision) {
//            scalar = A[x][p.column];
//            for (int y = 0; y < A[x].size(); ++y) {
//                A[x][y] -= A[p.row][y] * scalar;
//            }
//            b[x] -= b[p.row] * scalar;
//        }
//    }
//    cout << "Process pivot" << endl;
//    long double cScalar = c[p.column];
//    if (feasibilityCheck) {
//        long double feasibilityScalar = feasible[p.column];
//        b[b.size() - 1] -= b[p.row] * feasibilityScalar;
//        b[b.size() - 2] -= b[p.row] * cScalar;
//        for (int y = 0; y < feasible.size(); ++y) {
//            feasible[y] -= A[p.row][y] * feasibilityScalar;
//            c[y] -= A[p.row][y] * cScalar;
//        }
//    }
//    else {
//        b[b.size() - 1] -= b[p.row] * c[p.column];
//        for (int y = 0; y < feasible.size(); ++y) {
//            c[y] -= A[p.row][y] * cScalar;
//        }
//    }
//    cout << "Processed pivot" << endl;
//    // Find new pivot
//    p = findPivot(row, A, b);
//    return p;
//}
//
//bool reducedRowEchelonForm(Row& row, Matrix& A, Row& b, Row& c, Row& feasible, const bool feasibilityCheck) {
//    Position p = findPivot(row, A, b);
//    cout << p.column << " " << p.row << " " << p.optimal() << endl;
//    cout << "Enter rref while" << endl;
//    while (p.optimal()) {
//        row[p.column] = p.row;
//        cout << "Changed row val" << endl;
//        p = processPivot(p, row, A, b, c, feasible, feasibilityCheck);
//    }
//    cout << "Exit rref while" << endl;
//    return p.row == INT_MAX && p.column == -INT_MAX;
//}
//
//int verifyFeasibility(int m, int n, Matrix& A, Row& b, Row& c, Row& feasible, vector<int>& possibleSolutions) {
//    cout << "Enter feasibility" << endl;
//    for (int x = 0, y = m + n; x < b.size() - 1; ++x, ++y) {
//        if (b[x] < 0.0) {
//            A[x][y] = -1.0;
//            transform(A[x].begin(), A[x].end(), A[x].begin(), negate<long double>());
//            b.back() += b[x];
//            b[x] *= -1.0;
//            possibleSolutions[x] = -2;
//            for (int i = 0; i < m + n; ++i) {
//                feasible[i] += A[x][y];
//            }
//        }
//    }
//    transform(feasible.begin(), feasible.end(), feasible.begin(), negate<long double>());
//    cout << "Enter reducedRow" << endl;
//    if (reducedRowEchelonForm(feasible, A, b, c, feasible, true)){
//        return 1;  // Unbounded
//    }
//    if (abs(b.back() - 0.0) < precision) {
//        return 0;  // Optimal
//    }
//    return -1;  // Infeasible
//}
//
//pair<int, vector<long double>> allocate_ads(int n, int m, Matrix& A, Row& b, Row& c) {
//    bool check = b.end() != find_if(b.begin(), b.end(), [](long double val) {return val < 0.0;});  // Check if values are negative
//    vector<int> possibleSolutions(A[0].size(), -1);
//    transform(c.begin(), c.end(), c.begin(), negate<long double>());
//    for (int i = 0; i < A.size(); ++i) {
//        A[i][i + m] = 1;
//    }
//    // Check if current input is feasible
//    int feasibility;  // feasibility will stay 1 if unbounded
//    Row feasible(c.size());
//    cout << "Pre-feasibility check" << endl;
//    if (check) {
//        feasibility = verifyFeasibility(m, n, A, b, c, feasible, possibleSolutions);
//        if (feasibility == -1) {
//            return {-1, {}};  // Invalid or unbounded framework
//        }
//    }
//    cout << "Finished allocate phase 1" << endl;
//    // Resize and check if current input is unbounded
//    c.resize(c.size() - n);
//    b.pop_back();
//    for (Row& row : A) {
//        row.resize(row.size() - n);
//    }
//    if (reducedRowEchelonForm(c, A, b, c, feasible, false) || feasibility) {
//        return {1, {}};  // Unbounded
//    }
//    Row allocations(m);
//    for (int y = 0; y < m; ++y) {
//        long double sum = 0.0;
//        int bIndex = 0;
//        for (Row& x : A) {
//            sum += fabs(x[y]);
//            if (abs(x[y] - 0.0) < 1.0) {
//                bIndex = y;
//            }
//        }
//        if (sum - 1.0 <= precision) {
//            allocations[y] = b[bIndex];
//        }
//        else {
//            allocations[y] = 0.0;
//        }
//    }
//    return {0, move(allocations)};
//}
//
//int main() {
//    int n, m;
//    cin >> n >> m;
//    Matrix A(n, Row(m));
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < m; j++) {
//            cin >> A[i][j];
//        }
//    }
//    Row b(n);
//    for (int i = 0; i < n; i++) {
//        cin >> b[i];
//    }
//    Row c(m);
//    for (int i = 0; i < m; i++) {
//        cin >> c[i];
//    }
//
//    pair<int, Row> ans = allocate_ads(n, m, A, b, c);
//
//    switch (ans.first) {
//        case -1:
//            printf("No solution\n");
//            break;
//        case 0:
//            printf("Bounded solution\n");
//            for (int i = 0; i < m; i++) {
//                printf("%.18Lf%c", ans.second[i], " \n"[i + 1 == m]);
//            }
//            break;
//        case 1:
//            printf("Infinity\n");
//            break;
//    }
//    return 0;
//}

#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <functional>
#include <limits>
#include <valarray>

const long double EPS = std::numeric_limits<long double>::epsilon();

enum class state { optimal,
    infeasible,
    unbounded };
enum class method_phase { one,
    two };

using namespace std;

using Row = vector<long double>;
using Matrix = vector<Row>;

struct Position {
    short row;
    short column;
    bool optimal() const { return row == -1 || column == -1; }
};

struct simplex_method {
    void handle_artficial_vars() {
        for (int x = 0, y = m + n; x < b.size() - 1; ++x, ++y) {
            if (b[x] < 0.0) {
                possibleSolutions[x] = -2;
                A[x][y] = -1;
                b.back() += b[x];
                b[x] *= -1;
                transform(A[x].begin(), A[x].end(), A[x].begin(), negate<long double>());
                for (int y2 = 0; y2 < n + m; ++y2) {
                    w[y2] += A[x][y2];
                }
            }
        }
        transform(w.begin(), w.end(), w.begin(), negate<long double>());
    }

    void reducedRowEchelonForm(Row& obj) {
        Position p = find_pivot(obj);
        while (!(p.optimal() || cur_solution == state::unbounded)) {
            possibleSolutions[p.column] = p.row;
            scale_pivot(p);
            process_pivot(p, obj);
            p = find_pivot(obj);
        }
    }

    void trim_table_from_avars() {
        c.resize(c.size() - n);
        b.pop_back();
        for (Row r : A) {
            r.resize(r.size() - n);
        }
    }

    void phase_two() {
        phase = method_phase::two;
        trim_table_from_avars();
        reducedRowEchelonForm(c);
    }

    bool phase_one() {
        phase = method_phase::one;
        reducedRowEchelonForm(w);
        return abs(b.back() - 0.0) >= 0.001;
    }

    bool double_equals(double a, double b, double epsilon = 0.001)
    {
        return abs(a - b) < epsilon;
    }

    pair<int, Row> read_optimal_solution() {
        Row result(m);
        for (int i = 0; i < m; ++i) {
            long double sum = 0.0;
            int k = 0;
            for (int j = 0; j < A.size(); ++j) {
                if (possibleSolutions[j] >= 0.0)
                    sum += fabs(A[j][i]);
                if (double_equals(A[j][i], 1.0)) {
                    k = j;
                }
            }
            result[i] = (sum - 1.0 > EPS) ? 0.0 : b[k];
        }
        return {0, move(result)};
    }

    pair<int, Row> solve() {
        possibleSolutions = vector<int>(A[0].size(), -1);
        transform(c.begin(), c.end(), c.begin(), negate<double>());
        for (int x = 0; x < A.size(); ++x) {
            A[x][x + m] = 1;
        }
        if (b.end() != find_if(b.begin(), b.end(), [](long double val) {return val < 0.0;})) { // Check if values are negative
            w = Row(c.size());
            handle_artficial_vars();
            if (phase_one()) {  // Infeasible
                return { -1, {} };
            }
        }
        phase_two();
        if (cur_solution == state::unbounded) {
            return { 1, {} };
        }
        return read_optimal_solution();
    }

    bool double_equals_zero(long double a, long double epsilon = 0.001) {
        return abs(a - 0.0) < epsilon;
    }

    Position find_pivot(Row& cw) {
        int i = 0, j = distance(cw.begin(), min_element(cw.begin(), cw.end()));
        long double ratio = numeric_limits<long double>::max() - 1;
        if (cw[j] >= 0.0) {
            return { -1, -1 };
        }
        for (int k = 0; k < A.size(); ++k) {
            long double r = b[k] / A[k][j];
            if ((A[k][j] > 0.0 || A[k][j] < 0.0) && r - ratio < EPS && r > 0.0) {
                ratio = r;
                i = k;
            }
        }
        if (ratio == numeric_limits<long double>::max() - 1) {
            cur_solution = state::unbounded;
        }
        return { static_cast<short>(i), static_cast<short>(j) };
    }

    void process_pivot(Position p, Row& row) {
        long double scalar;
        for (int x = 0, s = A.size(); x < s; ++x) {
            if (p.row != x && !double_equals_zero(A[x][p.column], EPS)) {
                scalar = A[x][p.column];
                for (int y = 0; y < A[x].size(); ++y) {
                    A[x][y] -= A[p.row][y] * scalar;
                }
                b[x] -= b[p.row] * scalar;
            }
        }
        if (phase == method_phase::one) {
            b[b.size() - 2] -= b[p.row] * c[p.column];
            b[b.size() - 1] -= b[p.row] * row[p.column];
            auto mw = row[p.column];
            auto cw = c[p.column];
            for (size_t i = 0; i < w.size(); ++i) {
                row[i] -= A[p.row][i] * mw;
                c[i] -= A[p.row][i] * cw;
            }
        }
        else {
            b[b.size() - 1] -= b[p.row] * c[p.column];
            auto cw = c[p.column];
            for (size_t i = 0; i < w.size(); ++i) {
                c[i] -= A[p.row][i] * cw;
            }
        }
    }

    void scale_pivot(Position p)
    {
        long double divisor = A[p.row][p.column];
        b[p.row] /= divisor;
        for (long double &val : A[p.row]) {
            val /= divisor;
        }
    }

    int n, m;
    Matrix A;
    Row b, c, w;
    vector<int> possibleSolutions;
    state cur_solution;
    method_phase phase;
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

    simplex_method sm{ n, m, move(A), move(b), move(c) };
    pair<int, Row> ans = sm.solve();

    switch (ans.first) {
        case -1:
            printf("No solution\n");
            break;
        case 0:
            printf("Bounded solution\n");
            for (int i = 0; i < m; i++) {
                printf("%.18Lf%c", ans.second[i], " \n"[i + 1 == m]);
            }
            break;
        case 1:
            printf("Infinity\n");
            break;
    }
    return 0;
}
