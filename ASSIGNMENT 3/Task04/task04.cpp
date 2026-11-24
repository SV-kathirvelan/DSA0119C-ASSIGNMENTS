#include <iomanip>
#include <iostream>

class Money {
    long long paise;

public:
    Money(long long rupees = 0, int p = 0)
        : paise(rupees * 100 + p) {}

    Money operator+(const Money& other) const {
        return Money(0, static_cast<int>(paise + other.paise));
    }

    Money& operator+=(const Money& other) {
        paise += other.paise;
        return *this;
    }

    bool operator==(const Money& other) const {
        return paise == other.paise;
    }

    bool operator<(const Money& other) const {
        return paise < other.paise;
    }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long whole = m.paise / 100;
        long long fraction = m.paise % 100;

        if (fraction < 0) fraction = -fraction;
        if (m.paise < 0 && whole == 0) os << "-";

        os << "Rs " << (whole < 0 ? -whole : whole) << '.'
           << std::setw(2) << std::setfill('0') << fraction
           << std::setfill(' ');

        return os;
    }
};

int main() {
    Money a(120, 75), b(79, 25);
    Money c = a + b;

    std::cout << c << '\n'
              << std::boolalpha << (c == Money(200)) << '\n';

    std::cout << (Money(0, 75) + Money(0, 50)) << '\n';
}
