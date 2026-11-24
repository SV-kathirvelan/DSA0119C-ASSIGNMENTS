#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Account {
public:
    double rate() const { return 3.5; }
    virtual double vrate() const { return 3.5; }
    virtual ~Account() = default;
};
class SeniorAccount : public Account {
    double* history;
public:
    SeniorAccount() : history(new double[100]{}) {}
    double rate() const { return 4.0; } // hides non-virtual base function
    double vrate() const override { return 4.0; }
    ~SeniorAccount() override { delete[] history; cout << "SeniorAccount destroyed\n"; }
};

int main() {
    SeniorAccount s;
    Account* p = &s;
    Account copy = s; // object slicing: copy contains only Account part
    cout << p->rate() << ' ' << p->vrate() << ' ' << copy.vrate() << '\n';

    vector<unique_ptr<Account>> accounts;
    accounts.push_back(make_unique<Account>());
    accounts.push_back(make_unique<SeniorAccount>());
    for (const auto& a : accounts)
        cout << a->vrate() << '\n';
}
