#include <cmath>
#include <fstream>
#include <iostream>

void print_slau(double** a, double* x, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << fabs(a[i][j]);
            std::cout << "*x" << j + 1;
            if (a[i][j] >= 0 && j < n - 1)
                std::cout << " + ";
            if (a[i][j] < 0 && j < n - 1)
                std::cout << " - ";
        }
        std::cout << " = " << x[i];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int check_slau(double** a, int n)
{
    int sum;
    for (int i = 0; i < n; i++) {
        sum = 0;
        for (int j = 0; j < n; j++) {
            sum += fabs(a[i][j]);
        }
        sum -= fabs(a[i][i]);
        if (sum > fabs(a[i][i]))
            return -1;
    }
    return 0;
}

void divide_slau(double** a, double* x, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j != i)
                a[i][j] /= a[i][i];
        }
        x[i] /= a[i][i];
        a[i][i] = 0;
    }
}

void swap(double* x, double* y)
{
    double tmp = *x;
    *x = *y;
    *y = tmp;
}

void convert_slau(double** a, double* x, int n)
{
    for (int i = 0; i < n; i++) {
        a[i][i] = x[i];
        for (int j = i; j > 0; j--)
            swap(&a[i][j], &a[i][j - 1]);
    }
    for (int i = 0; i < n; i++)
        for (int j = 1; j < n; j++)
            a[i][j] = -a[i][j];

    for (int i = 0; i < n; i++)
        x[i] = 0;
}

double norma(double* x, int n)
{
    double max = fabs(x[0]);
    for (int i = 1; i < n; i++)
        if (x[i] > max)
            max = fabs(x[i]);
    return max;
}

int MPI(double** a, double* x, int n, double eps, int* n_iterations)
{
    int tmp_n_iterations = 0;
    double previous[n] = {0};
    double delta = 1000000;
    if (check_slau(a, n))
        return -1;
    divide_slau(a, x, n);
    convert_slau(a, x, n);
    while (delta > eps) {
        for (int i = 0; i < n; i++) {
            previous[i] = x[i];
            x[i] = 0;
        }
        tmp_n_iterations++;
        int g = 0;
        for (int i = 0; i < n; i++) {
            g = 0;
            x[i] += a[i][0];
            for (int j = 1; j < n; j++) {
                if (g == i)
                    ++g;
                x[i] += a[i][j] * previous[g++];
            }
        }
        delta = fabs(norma(x, n) - norma(previous, n));
    }
    *n_iterations = tmp_n_iterations;
    return 0;
}

int file_input_MPI(char* filename, double eps)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Invalid file\n";
        return -1;
    }
    int n;
    double tmp;
    file >> n;
    double** a = new double*[n];
    double* x = new double[n];
    for (int i = 0; i < n; i++) {
        a[i] = new double[n];
        for (int j = 0; j <= n; j++) {
            file >> tmp;
            if (j == n) {
                x[i] = tmp;
                continue;
            }
            a[i][j] = tmp;
        }
    }
    int n_iterations;
    if (MPI(a, x, n, eps, &n_iterations))
        return 1;
    std::cout << "Count iterations: " << n_iterations << "; eps = " << eps
              << '\n';
    for (int i = 0; i < n; i++) {
        std::cout << "x" << i + 1 << " = " << x[i];
        std::cout << "; ";
    }
    std::cout << std::endl;
    for (int i = 0; i < n; i++)
        delete[] a[i];
    delete[] a;
    delete[] x;
    file.close();
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Need more arguments\n";
        return 1;
    }
    file_input_MPI(argv[1], std::stod(argv[2]));
}