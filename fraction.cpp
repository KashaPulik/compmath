#include <iostream>

class Fraction {
public:
    int numerator;
    int ratio;

    Fraction() : numerator(1), ratio(1){};
    Fraction(int a) : numerator(a), ratio(1){};
    Fraction(int a, int b) : numerator(a), ratio(b){};

    Fraction operator+(const Fraction& other)
    {
        int new_ratio = NOK(ratio, other.ratio);
        return Fraction(
                numerator * (new_ratio / ratio)
                        + other.numerator * (new_ratio / other.ratio),
                new_ratio);
    }

    Fraction operator-(const Fraction& other)
    {
        return *this + Fraction(-other.numerator, other.ratio);
    }

    Fraction operator*(const Fraction& other)
    {
        return Fraction(numerator * other.numerator, ratio * other.ratio);
    }

    Fraction operator/(const Fraction& other)
    {
        return Fraction(numerator * other.ratio, ratio * other.numerator);
    }

    void print()
    {
        int nod = NOD(numerator, ratio);
        numerator /= nod;
        ratio /= nod;
        if(ratio < 0)
        {
            numerator = -numerator;
            ratio = -ratio;
        }
        std::cout << numerator << "/" << ratio << std::endl;
    }

private:
    int NOD(int a, int b)
    {
        a = a > 0 ? a : -a;
        b = b > 0 ? b : -b;
        int div;
        if (a == b)
            return a;
        int d = a - b;
        if (d < 0) {
            d = -d;
            div = NOD(a, d);
        } else
            div = NOD(b, d);
        return div;
    }
    int NOK(const int a, const int b)
    {
        return a * b / NOD(a, b);
    }
};

int main()
{
    Fraction a(6, -24), b(-8, 16);
    a.print();
    (a / b).print();
}