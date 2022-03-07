#include <iostream>
#include "Polynomial.h"
using namespace std;

int main() {

    int n = 8;
    Polynomial<double> sin(n);      // sin and cos represented with a taylor polynomial
    Polynomial<double> cos(n);
    int fact = 1;
    for (int i = 0; i < n; i++) {
        int sign = (1 - 2 * ((i / 2) % 2));
        if (i % 2)
            cos.coefficient(i) = sign / (double) fact;
        else sin.coefficient(i) = sign / (double) fact;
        fact *= (i + 1);
    }

    // output is very close to 1, which is proof that sin^2(x) + cos^2(x) = 1
    cout << sin * sin + cos * cos << endl;
}
