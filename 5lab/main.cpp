#include <cmath>
#include <iostream>

using namespace std;

double f(double x)
{
    return exp(-x * x);
}

double integral(double a, double b, unsigned long long n)
{
    double h = (b - a) / n;
    double s = 0;
    for (unsigned long long i = 1; i <= n; i++)
        s += f(a + i * h);
    return s * h;
}

pair<double, unsigned long long> eps_integral(double a, double b, double eps)
{
    unsigned long long n = 100;
    double s1 = integral(a, b, n);
    n *= 2;
    double s2 = integral(a, b, n);
    double r = fabs(s1 - s2) / 3;
    while (r > eps) {
        n *= 2;
        s1 = s2;
        s2 = integral(a, b, n);
        r = fabs(s1 - s2) / 3;
    }
    return make_pair(s2, n);
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        cout << "Not enough arguments!\n";
        cout << "Must be something like:\n";
        cout << "./app {a} {b} {epsylon}\n";
        return 1;
    }
    auto [s, n] = eps_integral(stod(argv[1]), stod(argv[2]), stod(argv[3]));
    cout << "Integral counted on [" << stod(argv[1]) << ", " << stod(argv[2])
         << "] interval with " << stod(argv[3]) << " accuracy\n";
    cout << "For this accuracy requiered " << n << " cuts\n";
    cout << "Approximate value equals " << s << endl;
}