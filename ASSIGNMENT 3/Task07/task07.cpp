#include <cmath>
#include <iostream>
#include <stdexcept>

class Complex {
    double re, im;

public:
    Complex() : re(0), im(0) {}
    Complex(double r) : re(r), im(0) {}
    Complex(double r, double i) : re(r), im(i) {}

    Complex operator+(const Complex& z) const {
        return {re + z.re, im + z.im};
    }

    Complex operator-(const Complex& z) const {
        return {re - z.re, im - z.im};
    }

    Complex operator*(const Complex& z) const {
        return {re * z.re - im * z.im,
                re * z.im + im * z.re};
    }

    Complex operator/(const Complex& z) const {
        double d = z.re * z.re + z.im * z.im;

        if (d == 0.0)
            throw std::domain_error("Division by zero impedance");

        return {(re * z.re + im * z.im) / d,
                (im * z.re - re * z.im) / d};
    }

    bool operator==(const Complex& z) const {
        return std::abs(re - z.re) < 1e-9 &&
               std::abs(im - z.im) < 1e-9;
    }

    double magnitude() const {
        return std::sqrt(re * re + im * im);
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const Complex& z) {
        os << z.re;

        if (z.im >= 0)
            os << " + " << z.im << "j";
        else
            os << " - " << -z.im << "j";

        return os;
    }
};

int main() {
    try {
        Complex R(10, 0), L(0, 5), C(0, -2);

        Complex series = R + L + C;
        Complex parallel = (R * L) / (R + L);

        std::cout << "Series: " << series << '\n';
        std::cout << "Parallel (R and L): " << parallel << '\n';
        std::cout << "Magnitude: " << series.magnitude() << '\n';

        Complex zero;
        Complex test = R / zero;
        std::cout << test << '\n';
    }
    catch (const std::domain_error& e) {
        std::cout << e.what() << '\n';
    }
}
