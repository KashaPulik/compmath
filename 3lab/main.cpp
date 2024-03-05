#include <cmath>
#include <iostream>

long double f(long double x)
{
    return 3.0 - sqrt(x) / 2.0 - exp(x * x / (-2.0));
}

long double stef_method(long double a, long double b, long double eps)
{
    long double x0 = (a + b) / 2;
    long double x1 = x0 - (f(x0) * f(x0)) / (f(x0 + f(x0)) - f(x0));
    int n = 1;
    while (fabs(x1 - x0) > eps) {
        n++;
        x0 = x1;
        x1 = x0 - (f(x0) / (f(x0 + f(x0)) - f(x0))) * f(x0);
    }
    std::cout << "n = " << n << "; x = " << x1 << std::endl;
    return x1;
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cout << "It's not enough arguments\n";
        return 1;
    }
    long double a = std::stod(argv[1]);
    long double b = std::stod(argv[2]);
    long double eps = std::stod(argv[3]);
    stef_method(a, b, eps);
}