#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <limits>
#include <cmath>
#include <numeric>
#include <bitset>

using namespace std;

const long double precision = 1.0e-3;
const long double upperBound = 1.0e+9;

typedef vector<long double> Column;
typedef vector<long double> Row;
typedef vector<Row> Matrix;

struct Equation {
    Equation(Matrix a, Column b) :
            a(move(a)),
            b(move(b)) {}

    Matrix a;
    Column b;
};

struct Position {
    Position(int column, int row) :
            column(column),
            row(row) {}

    int column;
    int row;
};

Position SelectPivotElement(const Matrix &a, vector<bool> &used_rows, vector<bool> &used_columns) {
    // Finds the row with the leftmost nonzero value
    Position pivot_element(0, 0);
    while (used_rows[pivot_element.row])
        ++pivot_element.row;
    while (used_columns[pivot_element.column])
        ++pivot_element.column;

    // Finds the pivot with the largest absolute value
    long double maxVal = 0.0;
    for (int x = pivot_element.row, size = a.size(); x < size; ++x) {
        if (fabs(maxVal) - fabs(a[x][pivot_element.column]) < precision) {
            maxVal = a[x][pivot_element.column];
            pivot_element.row = x;
        }
    }
    return pivot_element;
}

void SwapLines(Matrix &a, Column &b, vector<bool> &used_rows, Position &pivot_element) {
    swap(a[pivot_element.column], a[pivot_element.row]);
    swap(b[pivot_element.column], b[pivot_element.row]);
    swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
    pivot_element.row = pivot_element.column;
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element) {
    // Scale the pivot
    const long double divisor = a[pivot_element.row][pivot_element.column];
    const int size = a.size();
    for (int y = pivot_element.column; y < size; ++y) {
        a[pivot_element.row][y] /= divisor;
    }
    b[pivot_element.row] /= divisor;

    // Reduce the pivot
    for (int x = pivot_element.row + 1; x < size; ++x) {
        long double scalar = a[x][pivot_element.column];
        for (int y = pivot_element.column; y < size; ++y) {
            a[x][y] -= a[pivot_element.row][y] * scalar;
        }
        b[x] -= b[pivot_element.row] * scalar;
    }
}

void MarkPivotElementUsed(const Position &pivot_element, vector<bool> &used_rows, vector<bool> &used_columns) {
    used_rows[pivot_element.row] = true;
    used_columns[pivot_element.column] = true;
}

Column SolveEquation(Equation equation) {
    Matrix &A = equation.a;
    Column &b = equation.b;
    if (A.empty()) {
        return {};
    }
    int size = A.size();

    vector<bool> used_columns(size, false);
    vector<bool> used_rows(size, false);
    for (int step = size; step; --step) {
        Position pivot_element = SelectPivotElement(A, used_rows, used_columns);
        if (!A[pivot_element.row][pivot_element.column]) {
            break;
        }
        SwapLines(A, b, used_rows, pivot_element);
        ProcessPivotElement(A, b, pivot_element);
        MarkPivotElementUsed(pivot_element, used_rows, used_columns);
    }

    for (int x = size - 1; x; --x) {
        long double scalar = b[x];
        for (int y = 0; y != x; ++y) {
            b[y] -= A[y][x] * scalar;
        }
    }
    return move(b);
}

vector<vector<int>> generateSubsets(const vector<int>& set, int m) {
    const int size = pow(2, set.size());
    vector<vector<int>> subsets;
    bitset<32> bitSet;
    for (int i = 0; i < size; ++i) {
        bitSet = bitSet.to_ulong() + 1l;
        vector<int> subset;
        int counter = 0;
        for (int j = 0; j < set.size(); ++j) {
            if (bitSet[set.size() - 1 - j]) {
                subset.push_back(set[j]);
                if (++counter > m) {
                    break;
                }
            }
        }
        if (counter == m) {
            subsets.emplace_back(move(subset));
        }
    }
    return subsets;
}

vector<Column> solveMatrix(int m, const Matrix& A, const vector<long double>& b) {
    vector<Column> solutions;
    vector<int> nums(A.size());
    int s = 0;
    generate(nums.begin(), nums.end(), [&s] { return s++; });
    vector<vector<int>> subsets = generateSubsets(nums, m);
    if (A.size() == 1) {
        subsets.emplace_back(0);
    }
    for (const vector<int>& subset : subsets) {
        Matrix matrix;
        Column column;
        for (int i : subset) {
            matrix.push_back(A[i]);
            column.push_back(b[i]);
        }
        Equation equation(move(matrix), move(column));
        Column solution = SolveEquation(equation);
        if (!solution.empty()) {
            solutions.emplace_back(move(solution));
        }
    }
    return solutions;
}

void validate(int& n, const int m,  Matrix &A, vector<long double>& b) {
    // Convert N*M to M*M matrix
    while (n < m) {
        A.emplace_back(m, 0);
        b.emplace_back(0);
        ++n;
    }
    // Check for infinite solutions
    A.emplace_back(vector<long double>(m, 1));
    b.emplace_back(upperBound);
    ++n;
    // Check for positive coefficients
    for (int i = 0; i < m; ++i) {
        vector<long double> negative(m, 0.0);
        negative[i] = -1;
        A.emplace_back(move(negative));
        b.emplace_back(-0.0);
        ++n;
    }
}

pair<int, vector<long double>> solve_diet_problem(int n, int m, Matrix A, vector<long double> b, vector<long double> c) {
    validate(n, m, A, b);
    vector<Column> solutions = solveMatrix(m, A, b);
    // If no solutions calculated
    if (solutions.empty()) {
        return {-1, {}};
    }
    int solutionIndex = -1;
    long double maxPleasure = -(numeric_limits<long double>::max() / 2);
    for (int i = 0; i < solutions.size(); ++i) {
        Column& solution = solutions[i];
        bool valid = true;
        for (int j = 0; j < n; ++j) {
            long double bound = b[j];
            long double sum = 0.0;
            for (int k = 0; k < m; ++k) {
                sum += A[j][k] * solution[k];
            }
            if (sum - bound > precision) {
                valid = false;
                break;
            }
        }
        long double pleasure = 0.0;
        for (int j = 0; j < m; ++j) {
            pleasure += solution[j] * c[j];
        }
        if (valid && pleasure > maxPleasure) {
            maxPleasure = pleasure;
            solutionIndex = i;
        }
    }
    // If there are no solutions found
    if (solutionIndex == -1) {
        return {-1, {}};
    }
    Column& solution = solutions[solutionIndex];
    long double sum = 0.0;
    // If there are infinite solutions found
    if (accumulate(solution.begin(), solution.end(), sum) + precision >= upperBound) {
        return {1, {}};
    }
    return {0, move(solution)};
}

int main() {
    int n, m;
    cin >> n >> m;
    Matrix A(n, vector<long double>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> A[i][j];
        }
    }
    vector<long double> b(n);
    for (int i = 0; i < n; ++i) {
        cin >> b[i];
    }
    vector<long double> c(m);
    for (int i = 0; i < m; ++i) {
        cin >> c[i];
    }
    pair<int, vector<long double>> ans = solve_diet_problem(n, m, move(A), move(b), move(c));
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
