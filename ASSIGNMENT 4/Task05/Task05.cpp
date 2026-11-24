#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

class Cart {
    string items[10];
    double prices[10]{};
    int n = 0;
    double discount = 0;
public:
    Cart& add(const string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }
    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }
    double total() const {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100;
    }
    void show() const {
        for (int i = 0; i < n; ++i) cout << items[i] << ": " << prices[i] << '\n';
    }
};

int main() {
    // (a) direct object
    Cart c;
    c.add("Rice", 60).add("Dal", 120).setDiscount(10);

    // (b) pointer using ->
    Cart* p = &c;
    p->add("Oil", 180);

    // (c) reference
    Cart& ref = c;
    ref.setDiscount(10);

    // (d) dynamically allocated object
    Cart* dynamicCart = new Cart;
    dynamicCart->add("Bread", 40);
    cout << "Dynamic cart total: " << dynamicCart->total() << '\n';
    delete dynamicCart;

    cout << fixed << setprecision(2) << p->total() << '\n';

    // Array of pointers; find highest total
    Cart c2, c3;
    c2.add("Milk", 50);
    c3.add("Fruit", 200);
    Cart* carts[] = {&c, &c2, &c3};
    int count = sizeof(carts) / sizeof(carts[0]);
    int best = 0;
    for (int i = 1; i < count; ++i)
        if (carts[i]->total() > carts[best]->total()) best = i;
    cout << "Highest cart total: " << carts[best]->total() << '\n';
}
