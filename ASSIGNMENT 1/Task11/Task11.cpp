#include <iostream>
#include <array>
using namespace std;
enum class Zone { Inner, Middle, Outer };
constexpr array<long long, 3> FARES = {1000, 2000, 3000};
constexpr long long MIN_BALANCE = 5000;

long long calculateFare(Zone zone, int hour) {
    if (hour < 0 || hour > 23) return -1;
    int index = static_cast<int>(zone);
    if (index < 0 || index >= static_cast<int>(FARES.size())) return -1;
    long long amount = FARES[index];
    if ((hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)) amount = amount * 120 / 100;
    return amount;
}
class SmartCard {
private:
    long long balancePaise;
public:
    SmartCard(long long initial) : balancePaise(initial) {}
    bool pay(long long fare) {
        if (fare < 0 || balancePaise - fare < MIN_BALANCE) return false;
        balancePaise -= fare;
        return true;
    }
    long long balance() const { return balancePaise; }
};
int main() {
    SmartCard card(10000);
    long long amount = calculateFare(Zone::Inner, 8);
    cout << "Calculated peak fare (paise): " << amount << '\n';
    cout << "Journey accepted: " << boolalpha << card.pay(amount) << '\n';
    cout << "Remaining balance (paise): " << card.balance() << '\n';
    return 0;
}
