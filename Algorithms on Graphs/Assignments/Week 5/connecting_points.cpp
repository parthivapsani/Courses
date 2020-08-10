#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

double minimum_distance(vector<int> x, vector<int> y) {
    double result = 0.;
    //write your code here
    return result;
}

int main() {
    size_t n;
    cin >> n;
    vector<int> x(n), y(n);
    for (size_t i = 0; i < n; i++) {
        cin >> x[i] >> y[i];
    }
    cout << setprecision(10) << minimum_distance(x, y) << endl;
}
