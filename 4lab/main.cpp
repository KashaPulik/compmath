#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

double C(double x, int N, int number, std::vector<double>& xk)
{
    double tmp = 1;
    for (int i = 0; i <= N; i++) {
        if (i == number)
            continue;
        tmp *= (x - xk[i]) / (xk[number] - xk[i]);
    }
    return tmp;
}

double L(int N, double x, std::vector<double>& xk, std::vector<double>& uk)
{
    double tmp = 0;
    for (int i = 0; i <= N; i++) {
        tmp += C(x, N, i, xk) * uk[i];
    }
    return tmp;
}

int main(int argc, char* argv[])
{
    double tmp;
    std::fstream file(argv[1]);
    std::vector<double> xk;
    std::vector<double> uk;
    std::string line;

    std::getline(file, line);
    std::stringstream stream(line);
    while (stream >> tmp)
        xk.push_back(tmp);

    std::getline(file, line);
    stream.clear();
    stream.str(line);
    while (stream >> tmp)
        uk.push_back(tmp);

    int N = xk.size() - 1;

    std::cout << "L(" << std::stod(argv[2]);
    std::cout << ") = " << L(N, std::stod(argv[2]), xk, uk) << std::endl;
}