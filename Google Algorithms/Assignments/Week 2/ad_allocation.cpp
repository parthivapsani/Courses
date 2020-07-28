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
#include <functional>
#include <valarray>
#include <iomanip>

const long double EPS = std::numeric_limits<long double>::epsilon();

enum class state { optimal,
    infeasible,
    unbounded };
enum class method_phase { one,
    two };

using namespace std;

const long double precision = 1.0e-3;
const long double upperBound = 1.0e+9;

typedef vector<long double> Row;
typedef vector<Row> Matrix;

struct Position {
    int column;
    int row;
    bool optimal() const {return row == -1 || column == -1;}
};

struct simplex_method {

    void handle_artficial_vars()
    {
        for (std::size_t i = 0, j = m + n; i < b.size() - 1; ++i, ++j) {
            if (b[i] < 0.0) {

                solusion_vars[i] = -2;
                A[i][j] = -1;

                b.back() += b[i];
                b[i] = -b[i];

                std::transform(A[i].begin(), A[i].end(), A[i].begin(), std::negate<long double>());

                for (int k = 0; k < n + m; ++k) {
                    w[k] += A[i][k];
                }
            }
        }
        transform(w.begin(), w.end(), w.begin(), std::negate<long double>());
    }

    void handle_slack_vars()
    {
        for (std::size_t i = 0; i < A.size(); ++i) {
            A[i][i + m] = 1;
        }
    }

    // find pivots and run eliminations to get optimal objective
    void Gauss_Jordam_eliminations(row& obj)
    {
        while (true) {
            position p = find_pivot(obj);

            if (p.is_optimal() || cur_solution == state::unbounded) {
                break;
            }

            solusion_vars[p.column] = p.row;

            scale_pivot(p);

            process_pivot(p, obj);
        }
    }

    void trim_table_from_avars()
    {
        c.resize(c.size() - n);
        b.pop_back();

        for (auto& r : A) {
            r.resize(r.size() - n);
        }
    }

    void phase_two()
    {
        phase = method_phase::two;
        trim_table_from_avars();
        Gauss_Jordam_eliminations(c);
    }

    void phase_one()
    {
        phase = method_phase::one;
        Gauss_Jordam_eliminations(w);
        cur_solution = double_equals_zero(b.back()) ? state::optimal : state::infeasible;
    }

    void prepare_table()
    {
        solusion_vars = std::vector<int>(A[0].size(), -1);
        std::transform(c.begin(), c.end(), c.begin(), std::negate<double>());
        handle_slack_vars();
        if (mc) {
            w = row(c.size());
            handle_artficial_vars();
        }
    }

    bool double_equals(double a, double b, double epsilon = 0.001)
    {
        return std::abs(a - b) < epsilon;
    }

    bool mixed_constraints() const
    {
        auto it = std::find_if(b.cbegin(), b.cend(), [](auto j) { return j < 0.0; });
        return it == b.cend() ? false : true;
    }

    std::pair<int, vector<long double> > read_optimal_solution()
    {
        vector<long double> result(m);

        for (int i = 0; i < m; ++i) {
            long double sum = 0.0;
            int k = 0;
            for (std::size_t j = 0; j < A.size(); ++j) {
                if (solusion_vars[j] >= 0.0)
                    sum += fabs(A[j][i]);
                if (double_equals(A[j][i], 1.0)) {
                    k = j;
                }
            }

            result[i] = (sum - 1.0 > EPS) ? 0.0 : b[k];
        }

        return { 0, std::move(result) };
    }

    std::pair<int, vector<long double> > solve()
    {
        mc = mixed_constraints();

        prepare_table();

        if (mc) {
            phase_one();
            if (cur_solution == state::infeasible) {
                return { -1, {} };
            }
        }

        phase_two();

        if (cur_solution == state::unbounded) {
            return { 1, {} };
        }

        return read_optimal_solution();
    }

    bool double_equals_zero(long double a, long double epsilon = 0.001)
    {
        return std::abs(a - 0.0) < epsilon;
    }

    position find_pivot(row& cw)
    {
        short i = 0, j = distance(cw.begin(), min_element(cw.begin(), cw.end()));
        long double ratio = numeric_limits<long double>::max() - 1;

        if (cw[j] >= 0.0) {
            return { -1, -1 };
        }

        for (std::size_t k = 0; k < A.size(); ++k) {
            long double r = b[k] / A[k][j];
            if ((A[k][j] > 0.0 || A[k][j] < 0.0) && r - ratio < EPS && r > 0.0) {
                ratio = r;
                i = k;
            }
        }

        if (ratio == numeric_limits<long double>::max() - 1) {
            cur_solution = state::unbounded;
        }

        return { i, j };
    }

    void process_pivot(position p, row& w)
    {
        long double m{ 0.0 };

        for (int i = 0, s = A.size(); i < s; ++i) {
            if (p.row != i && !double_equals_zero(A[i][p.column], EPS)) {

                m = A[i][p.column];

                for (std::size_t j = 0; j < A[i].size(); ++j) {
                    A[i][j] -= A[p.row][j] * m;
                }

                b[i] -= b[p.row] * m;
            }
        }

        if (phase == method_phase::one) {
            b[b.size() - 2] -= b[p.row] * c[p.column];
            b[b.size() - 1] -= b[p.row] * w[p.column];
            auto mw = w[p.column];
            auto cw = c[p.column];
            for (std::size_t i = 0; i < w.size(); ++i) {
                w[i] -= A[p.row][i] * mw;
                c[i] -= A[p.row][i] * cw;
            }
        }
        else {
            b[b.size() - 1] -= b[p.row] * c[p.column];
            auto cw = c[p.column];
            for (std::size_t i = 0; i < w.size(); ++i) {
                c[i] -= A[p.row][i] * cw;
            }
        }
    }

    void scale_pivot(position p)
    {
        auto d = A[p.row][p.column];
        b[p.row] /= d;
        for (auto& n : A[p.row]) {
            n /= d;
        }
    }

    int n, m;
    matrix A;
    vector<long double> b, c, w;
    vector<int> solusion_vars;
    state cur_solution;
    method_phase phase;
    bool mc;
};

int main()
{
    std::ios_base::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    matrix A(n, vector<long double>(n + m + n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> A[i][j];
        }
    }
    vector<long double> b(n + 2);
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }

    vector<long double> c(n + m + n);
    for (int i = 0; i < m; i++) {
        cin >> c[i];
    }

    simplex_method sm{ n, m, std::move(A), std::move(b), std::move(c) };
    pair<int, vector<long double> > ans = sm.solve();

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
