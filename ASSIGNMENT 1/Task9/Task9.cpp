#include <iostream>
#include <iomanip>
using namespace std;
enum class Zone { Inner, Middle, Outer };
long long calculateFare(Zone zone, int hour) {
    if (hour < 0 || hour > 23) return -1;
    long long amount;
    switch (zone) {
        case Zone::Inner: amount = 1000; break;
        case Zone::Middle: amount = 2000; break;
        case Zone::Outer: amount = 3000; break;
        default: return -1;
    }
    bool peak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    if (peak) amount = amount * 120 / 100;
    return amount;
}
int main() {
    struct Test { const char* name; Zone zone; int hour; long long expected; };
    Test tests[] = {
        {"Inner 07:59", Zone::Inner, 7, 1000},
        {"Inner 08:00", Zone::Inner, 8, 1200},
        {"Middle 10:00", Zone::Middle, 10, 2000},
        {"Middle 17:00", Zone::Middle, 17, 2400},
        {"Outer 19:59", Zone::Outer, 19, 3600},
        {"Outer 20:00", Zone::Outer, 20, 3000}
    };
    for (const Test& test : tests) {
        long long actual = calculateFare(test.zone, test.hour);
        cout << test.name << " -> Rs " << actual / 100 << '.' << setw(2) << setfill('0') << actual % 100
             << " : " << (actual == test.expected ? "PASS" : "FAIL") << setfill(' ') << '\n';
    }
    return 0;
}
