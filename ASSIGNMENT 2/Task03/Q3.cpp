#include <iostream>
using namespace std;

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
};

double charge(double weightKg, bool express = false, bool insured = false) {
    double amount = weightKg * 50.0;
    if (express) amount += 30.0;
    if (insured) amount += 20.0;
    return amount;
}
double charge(int weightGrams) {
    return charge(weightGrams / 1000.0, false, false);
}
double charge(const Parcel& p) {
    return charge(p.weightKg, p.express, p.insured);
}

int main() {
    cout << "charge(2) = " << charge(2) << '\n';
    cout << "charge(2.0f) = " << charge(static_cast<double>(2.0f)) << '\n';
    cout << "charge(2.0) = " << charge(2.0) << '\n';
    cout << "charge(2.0, true) = " << charge(2.0, true) << '\n';
    cout << "charge(2.0, true, true) = " << charge(2.0, true, true) << '\n';
    Parcel p{3.0, false, true};
    cout << "charge(Parcel) = " << charge(p) << '\n';
}