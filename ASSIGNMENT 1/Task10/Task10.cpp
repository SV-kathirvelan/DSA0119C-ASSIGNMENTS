#include <iostream>
using namespace std;
int main() {
    const long long maximumFarePaise = 3600;
    long long journeys[] = {20000, 500000, 3000000};
    for (long long count : journeys) {
        long long totalPaise = count * maximumFarePaise;
        cout << "Journeys: " << count << '\n';
        cout << "Total rupees: Rs " << totalPaise / 100 << '\n';
        cout << "Total paise: " << totalPaise << '\n';
        cout << "Fits signed 32-bit int: " << boolalpha
             << (totalPaise <= 2147483647LL) << "\n\n";
    }
    return 0;
}
