#include <iostream>
#include <array>
using namespace std;
enum class Zone { Inner, Middle, Outer, Airport };
enum class DayType { Weekday, Weekend };
constexpr array<long long, 4> FARES = {1000, 2000, 3000, 6000};
constexpr long long MIN_BALANCE = 5000;

long long revisedFare(Zone zone, int hour, DayType day) {
    int index = static_cast<int>(zone);
    if (index < 0 || index >= static_cast<int>(FARES.size()) || hour < 0 || hour > 23) return -1;
    long long amount = FARES[index];
    if (day == DayType::Weekend) amount = amount * 90 / 100;
    else if ((hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)) amount = amount * 120 / 100;
    return amount;
}
class Pass {
private:
    bool active;
    array<bool, 4> allowed;
public:
    Pass(bool enabled, array<bool, 4> zones) : active(enabled), allowed(zones) {}
    bool covers(Zone zone) const {
        int index = static_cast<int>(zone);
        return active && index >= 0 && index < 4 && allowed[index];
    }
};
class SmartCard {
private:
    long long balancePaise;
public:
    SmartCard(long long initial) : balancePaise(initial) {}
    bool deduct(long long fare) {
        if (fare < 0 || balancePaise - fare < MIN_BALANCE) return false;
        balancePaise -= fare;
        return true;
    }
    long long balance() const { return balancePaise; }
};
int main() {
    cout << "Airport weekday peak fare: Rs " << revisedFare(Zone::Airport, 8, DayType::Weekday) / 100 << '\n';
    cout << "Airport weekend fare: Rs " << revisedFare(Zone::Airport, 12, DayType::Weekend) / 100 << '\n';
    Pass monthlyPass(true, {false, true, false, false});
    cout << "Pass covers Middle: " << boolalpha << monthlyPass.covers(Zone::Middle) << '\n';
    cout << "Pass covers Airport: " << monthlyPass.covers(Zone::Airport) << '\n';
    SmartCard card(10000);
    cout << "Airport fare paid from card: " << card.deduct(6000) << '\n';
    cout << "Remaining balance (paise): " << card.balance() << '\n';
    return 0;
}
