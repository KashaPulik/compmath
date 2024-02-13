#include <cstdlib>
#include <fstream>
#include <iostream>

class Fraction {
public:
    long long numerator;
    long long ratio;

    Fraction(int a = 1, int b = 1) : numerator(a), ratio(b){};

    Fraction operator=(const int& other)
    {
        numerator = other;
        ratio = 1;
        return *this;
    }

    Fraction operator+(const Fraction& other)
    {
        reduction();
        int new_ratio = NOK(ratio, other.ratio);
        return Fraction(
                numerator * (new_ratio / ratio)
                        + other.numerator * (new_ratio / other.ratio),
                new_ratio);
    }

    Fraction operator-(const Fraction& other)
    {
        reduction();
        return *this + Fraction(-other.numerator, other.ratio);
    }

    Fraction operator*(const Fraction& other)
    {
        reduction();
        return Fraction(numerator * other.numerator, ratio * other.ratio);
    }

    Fraction operator/(const Fraction& other)
    {
        reduction();
        return Fraction(numerator * other.ratio, ratio * other.numerator);
    }

    bool operator>(const Fraction& other)
    {
        reduction();
        int nok = NOK(ratio, other.ratio);
        return numerator * (nok / ratio)
                > other.numerator * (nok / other.ratio);
    }

    Fraction abs()
    {
        return Fraction(
                numerator > 0 ? numerator : -numerator,
                ratio > 0 ? ratio : -ratio);
    }

    void reduction()
    {
        int nod = NOD(numerator, ratio);
        numerator /= nod;
        ratio /= nod;
        if (ratio < 0) {
            numerator = -numerator;
            ratio = -ratio;
        }
    }

    void print()
    {
        reduction();
        std::cout << numerator << "/" << ratio << std::endl;
    }

private:
    int NOD(int a, int b)
    {
        a = a > 0 ? a : -a;
        b = b > 0 ? b : -b;
        while (a > 0 && b > 0)
            if (a > b)
                a %= b;
            else
                b %= a;
        return a + b;
    }
    int NOK(const int a, const int b)
    {
        return a * b / NOD(a, b);
    }
};

void rsly_gauss(Fraction** a, Fraction* x, int n)
{
    long long imax, i, j, k;
    Fraction amax, c;
    //------------------------
    // Пpямой ход
    //------------------------
    for (k = 0; k < n; k++) {
        //-------------------------------------------------------
        // Поиcк макcимального элемента по абcолютной величине
        //-------------------------------------------------------
        imax = k;
        amax = a[k][k].abs();
        for (i = k + 1; i < n; i++)
            if (a[i][k].abs() > amax) {
                amax = a[i][k].abs();
                imax = i;
            }
        //------------------------------------
        // Пеpеcтановка cтpок k и imax
        //------------------------------------
        if (k != imax) {
            Fraction* tmp = a[k];
            a[k] = a[imax];
            a[imax] = tmp;
            c = x[k];
            x[k] = x[imax];
            x[imax] = c;
        }
        c = Fraction() / a[k][k];
        for (i = k; i < n; i++)
            a[k][i] = a[k][i] * c;
        x[k] = x[k] * c;
        for (i = k + 1; i < n; i++) {
            for (j = k + 1; j < n; j++)
                a[i][j] = a[i][j] - a[i][k] * a[k][j];
            x[i] = x[i] - a[i][k] * x[k];
        }
    }
    //--------------------------
    // Сокращение дробей
    //--------------------------
    for (int m = 0; m < n; m++)
        x[m].reduction();
    //--------------------------
    // Обpатный ход
    //--------------------------
    for (i = n - 2; i >= 0; i--)
        for (j = i + 1; j < n; j++)
            x[i] = x[i] - a[i][j] * x[j];
    //--------------------------
    // Сокращение дробей
    //--------------------------
    for (int m = 0; m < n; m++)
        x[m].reduction();
} // rsly_gauss

int file_input_rsly_gauss(char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Invalid file\n";
        return -1;
    }
    int n, tmp;
    file >> n;
    Fraction** a = new Fraction*[n];
    Fraction* x = new Fraction[n];
    for (int i = 0; i < n; i++) {
        a[i] = new Fraction[n];
        for (int j = 0; j <= n; j++) {
            file >> tmp;
            if (j == n) {
                x[i] = tmp;
                continue;
            }
            a[i][j] = tmp;
        }
    }
    rsly_gauss(a, x, n);
    for (int i = 0; i < n; i++) {
        std::cout << "x" << i + 1 << " = " << x[i].numerator;
        if (x[i].ratio > 1)
            std::cout << "/" << x[i].ratio;
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
    if(argc < 2) {
        std::cerr << "Need more arguments\n";
        return 1;
    }
    file_input_rsly_gauss(argv[1]);
}