#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

const int PRECISION = 6;

typedef vector<double> Column;
typedef vector<double> Row;
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

Equation ReadEquation() {
    int size;
    cin >> size;
    Matrix a(size, vector<double>(size, 0.0));
    Column b(size, 0.0);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column)
            cin >> a[row][column];
        cin >> b[row];
    }
    return Equation(a, b);
}

Position SelectPivotElement(const Matrix &a, vector<bool> &used_rows, vector<bool> &used_columns) {
    // Finds the row with the leftmost nonzero value
    Position pivot_element(0, 0);
    while (used_rows[pivot_element.row])
        ++pivot_element.row;
    while (used_columns[pivot_element.column])
        ++pivot_element.column;

    // Finds the pivot with the largest absolute value
    double maxVal = 0.0;
    for (int x = pivot_element.row, size = a.size(); x < size; ++x) {
        if (fabs(a[x][pivot_element.column]) > fabs(maxVal)) {
            maxVal = a[x][pivot_element.column];
            pivot_element.row = x;
        }
    }
    return pivot_element;
}

void SwapLines(Matrix &a, Column &b, vector<bool> &used_rows, Position &pivot_element) {
    std::swap(a[pivot_element.column], a[pivot_element.row]);
    std::swap(b[pivot_element.column], b[pivot_element.row]);
    std::swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
    pivot_element.row = pivot_element.column;
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element) {
    // Scale the pivot
    const double divisor = a[pivot_element.row][pivot_element.column];
    const int size = a.size();
    for (int y = pivot_element.column; y < size; ++y) {
        a[pivot_element.row][y] /= divisor;
    }
    b[pivot_element.row] /= divisor;

    // Reduce the pivot
    for (int x = pivot_element.row + 1; x < size; ++x) {
        double scalar = a[x][pivot_element.column];
        for (int y = pivot_element.column; y < size; ++y) {
            a[x][y] -= (a[pivot_element.row][y] * scalar);
        }
        b[x] -= (b[pivot_element.row] * scalar);
    }
}

void MarkPivotElementUsed(const Position &pivot_element, vector<bool> &used_rows, vector<bool> &used_columns) {
    used_rows[pivot_element.row] = true;
    used_columns[pivot_element.column] = true;
}

Column SolveEquation(Equation equation) {
    Matrix &a = equation.a;
    Column &b = equation.b;
    int size = a.size();

    vector<bool> used_columns(size, false);
    vector<bool> used_rows(size, false);
    for (int step = 0; step < size; ++step) {
        Position pivot_element = SelectPivotElement(a, used_rows, used_columns);
        SwapLines(a, b, used_rows, pivot_element);
        ProcessPivotElement(a, b, pivot_element);
        MarkPivotElementUsed(pivot_element, used_rows, used_columns);
    }

    for (int x = size - 1; x; --x) {
        for (int y = 0; y != x; ++y) {
            b[y] -= a[y][x] * b[x];
            a[y][x] = 0;
        }
    }

    return b;
}

void PrintColumn(const Column &column) {
    int size = column.size();
    cout.precision(PRECISION);
    for (int row = 0; row < size; ++row)
        cout << fixed << column[row] << " ";
    cout << endl;
}

int main() {
    Equation equation = ReadEquation();
    if (equation.a.size()) {
        Column solution = SolveEquation(equation);
        PrintColumn(solution);
    }
    return 0;
}
